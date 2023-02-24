// Copyright 2023 Jeonghyeon Ha. All Rights Reserved.

#include "RedirectAll.h"

#include "AssetToolsModule.h"
#include "RedirectAllStyle.h"
#include "RedirectAllCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "AssetRegistry/AssetRegistryModule.h"

static const FName RedirectAllTabName("RedirectAll");

#define LOCTEXT_NAMESPACE "FRedirectAllModule"

void FRedirectAllModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRedirectAllStyle::Initialize();
	FRedirectAllStyle::ReloadTextures();

	FRedirectAllCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRedirectAllCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FRedirectAllModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRedirectAllModule::RegisterMenus));
}

void FRedirectAllModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRedirectAllStyle::Shutdown();

	FRedirectAllCommands::Unregister();
}

void FRedirectAllModule::PluginButtonClicked()
{
	TArray<FAssetData> AllAssets;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	// Find all assets in the game folder
	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add("/Game");
	AssetRegistryModule.Get().GetAssets(Filter, AllAssets);
	
	// Find all redirectors
	TArray<FAssetData> Redirectors;
	for (auto Asset : AllAssets)
	{		
		if (Asset.AssetClassPath.GetAssetName() == "ObjectRedirector")
		{
			Redirectors.Add(Asset);
		}
	}
	
	TArray<UObjectRedirector*> ObjectRedirectors;
	for (const auto& Redirector : Redirectors)
	{
		ObjectRedirectors.Add(Cast<UObjectRedirector>(Redirector.GetAsset()));
	}
	
	// This will fix references to selected redirectors, except in the following cases:
	// Redirectors referenced by unloaded maps will not be fixed up, but any references to it that can be fixed up will
	// Redirectors referenced by code will not be completely fixed up
	// Redirectors that are not at head revision or checked out by another user will not be completely fixed up
	// Redirectors whose referencers are not at head revision, are checked out by another user, or are refused to be checked out will not be completely fixed up.

	if ( ObjectRedirectors.Num() > 0 )
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.FixupReferencers(ObjectRedirectors);
	}
}

void FRedirectAllModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRedirectAllCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("ContentBrowser.Toolbar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Save");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRedirectAllCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRedirectAllModule, RedirectAll)