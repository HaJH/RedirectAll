// Copyright Epic Games, Inc. All Rights Reserved.

#include "RedirectAllCommands.h"

#define LOCTEXT_NAMESPACE "FRedirectAllModule"

void FRedirectAllCommands::RegisterCommands()
{
	// UI_COMMAND(PluginAction, "Redirect All", "Execute RedirectAll action", EUserInterfaceActionType::Button, FInputChord());
	// UI_COMMAND(PluginAction, "Redirect All", "Perform a redirect on the entire content folder.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(PluginAction, "Redirect All", "Fix all redirectors in the content folder.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
