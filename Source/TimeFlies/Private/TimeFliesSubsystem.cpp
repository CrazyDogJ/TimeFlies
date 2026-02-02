// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeFliesSubsystem.h"

#include "TimeFlies.h"
#include "TimeFliesActorInterface.h"
#include "TimeFliesReplicatedObject.h"
#include "TimeFliesSettings.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

void UTimeFliesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// Refresh the tick type after initialization
	SetTickableTickType(GetTickableTickType());
}

void UTimeFliesSubsystem::Deinitialize()
{
	Super::Deinitialize();
	
	// Always cancel tick as this is about to be destroyed
	SetTickableTickType(ETickableTickType::Never);
}

UWorld* UTimeFliesSubsystem::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

ETickableTickType UTimeFliesSubsystem::GetTickableTickType() const
{
	// If this is a template or has not been initialized yet, set to never tick and it will be enabled when it is initialized
	if (IsTemplate() || !GetWorld())
	{
		return ETickableTickType::Never;
	}

	// Otherwise default to conditional
	return ETickableTickType::Conditional;
}

bool UTimeFliesSubsystem::IsAllowedToTick() const
{
	if (!GetWorld())
	{
		return false;
	}
	
	return true;
}

void UTimeFliesSubsystem::Tick(float DeltaTime)
{
	if (!GetWorld()) return;
	
	for (auto& Task : TimeFliesTasks)
	{
#if ALLOW_CONSOLE
		if (bShowDebug && GEngine)
		{
			FString Message;
			Message = "Guid: " + Task.TimeFliesTaskGUID.ToString()
			+ "\n" + "Tag: " + Task.TimeFliesIdentityTag.ToString()
			+ "\n" + "Include Time Dilation: " + (Task.Setting.bIncludeTimeDilation ? "Yes" : "No")
			+ "\n" + "Custom Time Dilation: " + FString::SanitizeFloat(Task.Setting.CustomTimeDilation)
			+ "\n" + "Include Pause: " + (Task.Setting.bIncludePauseTime ? "Yes" : "No")
			+ "\n" + "Time: " + FString::SanitizeFloat(Task.LastSavedTotalTime);
			
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Blue, Message);
		}
#endif
		
		Task.AddDeltaTime(GetWorld());
	}

	for (const auto CustomTask : CustomTasks)
	{
		CustomTask.Value->NativeTick(DeltaTime);
	}
}

TStatId UTimeFliesSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimeFliesSubsystem, STATGROUP_Tickables);
}

bool UTimeFliesSubsystem::IsTickableWhenPaused() const
{
	return true;
}

int UTimeFliesSubsystem::NewTimeFliesTask(FGuid NewGuid, FGameplayTag IdentityTag, FTimeFliesSetting TimeCountSettings)
{
	if (NewGuid.IsValid())
	{
		return TimeFliesTasks.Add(FTimeFliesTask(NewGuid, IdentityTag, TimeCountSettings));
	}

	return INDEX_NONE;
}

bool UTimeFliesSubsystem::RemoveTimeFliesTask(FGuid Guid)
{
	if (Guid.IsValid())
	{
		return TimeFliesTasks.Remove(FTimeFliesTask(Guid)) > 0;
	}

	return false;
}

TArray<int> UTimeFliesSubsystem::FindTimeFliesTasksByTag(FGameplayTag IdTag) const
{
	TArray<int> Result;

	const auto Found = TimeFliesTasks.IndexOfByPredicate([&IdTag](const FTimeFliesTask& Task)
	{
		return Task.TimeFliesIdentityTag == IdTag;
	});

	if (Found)
	{
		Result.Add(Found);
	}

	return Result;
}

int UTimeFliesSubsystem::GetTimeFliesTaskByGuid(FGuid Guid) const
{
	const auto Found = TimeFliesTasks.IndexOfByPredicate([&Guid](const FTimeFliesTask& Task)
	{
		return Task.TimeFliesTaskGUID == Guid;
	});

	return Found;
}

void UTimeFliesSubsystem::OnSaveGameLoaded(const TArray<FTimeFliesTask> SavedTimeFliesTasks)
{
	if (SavedTimeFliesTasks.IsEmpty())
	{
		// New played time task when new game.
		if (const auto TimeFliesSettings = Cast<UTimeFliesSettings>(GetMutableDefault<UTimeFliesSettings>()))
		{
			NewTimeFliesTask(FGuid::NewGuid(), TimeFliesTags::DefaultPlayedTimeTag, TimeFliesSettings->PlayedTimeFliesSetting);
		}
	}
	else
	{
		TimeFliesTasks = SavedTimeFliesTasks;
	}
}

void UTimeFliesSubsystem::ClearTasks()
{
	TimeFliesTasks.Empty();
}

bool UTimeFliesSubsystem::RegisterReplicatedData(AActor* Actor)
{
	if (Actor && Actor->HasAuthority())
	{
		if (Actor->Implements<UTimeFliesActorInterface>())
		{
			const auto RepObjectClass = ITimeFliesActorInterface::Execute_GetReplicatedObjectClass(Actor);
			const auto Guid = ITimeFliesActorInterface::Execute_GetActorIdentityGuid(Actor);
			const auto NewRepObject = NewObject<UTimeFliesReplicatedObject>(Actor, RepObjectClass);
			Actor->AddReplicatedSubObject(NewRepObject);
			ITimeFliesActorInterface::Execute_SetReplicatedObject(Actor, Cast<UTimeFliesReplicatedObject>(NewRepObject));
			CustomTasks.Add(Guid, NewRepObject);
			return true;
		}
		else
		{
			UE_LOG(LogTimeFlies, Error, TEXT("Register data failed, Actor %s does not implement ITimeFliesActorInterface. "), *Actor->GetName())
		}
	}

	UE_LOG(LogTimeFlies, Error, TEXT("Register data failed, Actor not valid. "))
	return false;
}

bool UTimeFliesSubsystem::RegisterReplicatedActor(AActor* Actor)
{
	if (Actor && Actor->HasAuthority())
	{
		if (Actor->Implements<UTimeFliesActorInterface>())
		{
			const auto Guid = ITimeFliesActorInterface::Execute_GetActorIdentityGuid(Actor);
			
			if (const auto Found = CustomTasks.Find(Guid))
			{
				const auto Object = *Found;
				Object->Rename(nullptr, Actor);
				Actor->AddReplicatedSubObject(Object);
				ITimeFliesActorInterface::Execute_SetReplicatedObject(Actor, Cast<UTimeFliesReplicatedObject>(Object));
			}
			
			RegisteredTimeFliesActors.Add(Guid, Actor);
			return true;
		}
		else
		{
			UE_LOG(LogTimeFlies, Error, TEXT("Register actor failed, Actor %s does not implement ITimeFliesActorInterface. "), *Actor->GetName())
		}
	}

	UE_LOG(LogTimeFlies, Error, TEXT("Register actor failed, Actor not valid. "))
	return false;
}

bool UTimeFliesSubsystem::UnregisterReplicatedActor(AActor* Actor)
{
	if (Actor && Actor->HasAuthority())
	{
		if (Actor->Implements<UTimeFliesActorInterface>())
		{
			const auto Guid = ITimeFliesActorInterface::Execute_GetActorIdentityGuid(Actor);
			if (const auto Found = CustomTasks.Find(Guid))
			{
				const auto Object = *Found;
				Actor->RemoveReplicatedSubObject(Object);
				Object->Rename(nullptr, this);
			}
			return RegisteredTimeFliesActors.Remove(Guid) > 0;
		}
		else
		{
			UE_LOG(LogTimeFlies, Error, TEXT("Unregister actor failed, Actor %s does not implement ITimeFliesActorInterface. "), *Actor->GetName())
		}
	}

	UE_LOG(LogTimeFlies, Error, TEXT("Unregister actor failed, Actor not valid. "))
	return false;
}

TArray<FTimeFliesCustomTaskSaveGame> UTimeFliesSubsystem::GetCustomTasksSaveGame()
{
	TArray<FTimeFliesCustomTaskSaveGame> SaveGames;
	
	for (auto& Pair : CustomTasks)
	{
		FTimeFliesCustomTaskSaveGame Entry;
		Entry.Guid = Pair.Key;
		Entry.ObjectClass = Pair.Value->GetClass();

		// To byte
		FMemoryWriter MemoryWriter(Entry.Data, true);
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, false);
		Pair.Value->Serialize(Ar);

		SaveGames.Add(Entry);
	}

	return SaveGames;
}

void UTimeFliesSubsystem::LoadCustomTasks(TArray<FTimeFliesCustomTaskSaveGame> SaveGameData)
{
	for (auto Obj : CustomTasks)
	{
		// Remove actor rep subobject.
		if (auto Found = RegisteredTimeFliesActors.Find(Obj.Key))
		{
			auto FoundActor = *Found;
			FoundActor->RemoveReplicatedSubObject(Obj.Value);
		}
		Obj.Value->Rename(nullptr, this);
		Obj.Value->ConditionalBeginDestroy();
	}
	
	CustomTasks.Empty();

	for (const auto& Entry : SaveGameData)
	{
		// New object
		UTimeFliesReplicatedObject* NewObj = NewObject<UTimeFliesReplicatedObject>(this, Entry.ObjectClass);

		// serialize
		FMemoryReader MemoryReader(Entry.Data, true);
		FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
		NewObj->Serialize(Ar);

		CustomTasks.Add(Entry.Guid, NewObj);

		// Add actor rep subobject.
		if (auto Found = RegisteredTimeFliesActors.Find(Entry.Guid))
		{
			auto FoundActor = *Found;
			NewObj->Rename(nullptr, FoundActor);
			FoundActor->AddReplicatedSubObject(NewObj);
		}
	}
}
