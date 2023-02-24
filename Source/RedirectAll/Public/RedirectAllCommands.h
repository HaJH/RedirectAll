// Copyright 2023 Jeonghyeon Ha. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RedirectAllStyle.h"

class FRedirectAllCommands : public TCommands<FRedirectAllCommands>
{
public:

	FRedirectAllCommands()
		: TCommands<FRedirectAllCommands>(TEXT("RedirectAll"), NSLOCTEXT("Contexts", "RedirectAll", "RedirectAll Plugin"), NAME_None, FRedirectAllStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
