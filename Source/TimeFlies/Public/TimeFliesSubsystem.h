// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeFliesCore.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimeFliesSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeFliesTaskComplete, FGuid, CompletedTaskGuid);

/**
 * Global game instance subsystem for managing the save game played hours and other time flies stuff.
 */
UCLASS()
class TIMEFLIES_API UTimeFliesSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Properties ----------------------------------------------------------------------------------
	
	/** Time flies tasks. */
	UPROPERTY(BlueprintReadWrite)
	TArray<FTimeFliesTask> TimeFliesTasks;
	
	/** Task complete delegate. */
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnTimeFliesTaskComplete OnTimeFliesTaskCompleteEvent;
	
#if ALLOW_CONSOLE
	bool bShowDebug = false;
#endif
	
	// Functions ----------------------------------------------------------------------------------
	
	// Subsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// Subsystem implementation End
	
	// FTickableGameObject implementation Begin
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsAllowedToTick() const override final;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override;
	// FTickableGameObject implementation End
	
	/** New time flies task. */
	UFUNCTION(BlueprintCallable)
	int NewTimeFliesTask(FGuid NewGuid, FGameplayTag IdentityTag, FTimeFliesSetting TimeCountSettings);

	/** Remove time flies task by guid. */
	UFUNCTION(BlueprintCallable)
	bool RemoveTimeFliesTask(FGuid Guid);

	/** Find task by gameplay tag.
	 * Example: XXX.Plants or XXX.Forge category.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<int> FindTimeFliesTasksByTag(FGameplayTag IdTag) const;

	/** Find task index by guid. */
	UFUNCTION(BlueprintCallable)
	int GetTimeFliesTaskByGuid(FGuid Guid) const;
	
	/** Load game and set tasks and continue time counting.
	 * If you leave input empty, we will create a played time flies task for you.
	 */
	UFUNCTION(BlueprintCallable)
	void OnSaveGameLoaded(const TArray<FTimeFliesTask> SavedTimeFliesTasks);
	
	/** For exit to main menu. All tasks will be removed. */
	UFUNCTION(BlueprintCallable)
	void ClearTasks();
};
