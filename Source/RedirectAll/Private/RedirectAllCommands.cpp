// Copyright 2023 Jeonghyeon Ha. All Rights Reserved.

#include "RedirectAllCommands.h"

#define LOCTEXT_NAMESPACE "FRedirectAllModule"

void FRedirectAllCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Redirect All", "Fix Up All Redirectors in the content folder.", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
