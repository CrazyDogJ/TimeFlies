// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTimeFlies, Log, All);

class FTimeFliesModule : public IModuleInterface
{
public:
	
#if ALLOW_CONSOLE
	IConsoleCommand* Command;
#endif
	
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
