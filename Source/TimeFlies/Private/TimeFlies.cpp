// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimeFlies.h"

#include "ISettingsModule.h"
#include "TimeFliesSettings.h"
#include "TimeFliesSubsystem.h"

#define LOCTEXT_NAMESPACE "FTimeFliesModule"

#if ALLOW_CONSOLE
namespace FTimeFliesCommands
{
	static void SetupDebugString(const TArray<FString>& Args, UWorld* World)
	{
		const auto Subsystem = World->GetGameInstance()->GetSubsystem<UTimeFliesSubsystem>();
		Subsystem->bShowDebug = !Subsystem->bShowDebug;
	}
}
#endif

void FTimeFliesModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "Time Flies",
			LOCTEXT("RuntimeSettingsName", "Time Flies"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Time Flies Plugin"),
			GetMutableDefault<UTimeFliesSettings>());
	}

#if ALLOW_CONSOLE
	Command = IConsoleManager::Get().RegisterConsoleCommand(
	TEXT("TimeFlies.ShowTasksInfo"),
	TEXT("Show time flies tasks info."),
	FConsoleCommandWithWorldAndArgsDelegate::CreateStatic(FTimeFliesCommands::SetupDebugString),
	ECVF_Default
	);
#endif
	
}

void FTimeFliesModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "Time Flies");
	}
	
#if ALLOW_CONSOLE
	IConsoleManager::Get().UnregisterConsoleObject(Command);
#endif
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTimeFliesModule, TimeFlies)