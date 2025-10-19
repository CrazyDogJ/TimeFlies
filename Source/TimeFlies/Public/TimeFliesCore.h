// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"
#include "TimeFliesCore.generated.h"

/**
 * Native gameplay tags
 */
namespace TimeFliesTags
{
	TIMEFLIES_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(DefaultPlayedTimeTag)
}

/**
 * Define how time flies.
 */
USTRUCT(BlueprintType)
struct FTimeFliesSetting
{
	GENERATED_BODY()

	/** Should multiply with global time dilation. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bIncludeTimeDilation = false;

	/** Time flies custom time dilation multiplier.
	 * Will be multiplied with world global dilation.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	float CustomTimeDilation = 1.0f;

	/** Should count pause game time. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	bool bIncludePauseTime = false;

	/** If over end time, this task will be completed and not update any more.
	 * Leave to 0 means has not end.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame)
	float EndTime = 0.0f;
};

/**
 * Time flies task struct.
 */
USTRUCT(BlueprintType)
struct FTimeFliesTask
{
	GENERATED_BODY()

	/** Task guid, used to find time flies task.
	 * Can leave empty if we don't need to find it.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, SaveGame)
	FGuid TimeFliesTaskGUID;

	/** Identity tag for tasks finding. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, SaveGame)
	FGameplayTag TimeFliesIdentityTag;
	
	/** Define how this task calc time flies. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, SaveGame)
	FTimeFliesSetting Setting;

	/** Total time will be loaded in when load game
	 * and will be re-calculated and saved when trigger SaveGame.
	 * Notice: This time will be accumulated when trans to another map.
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, SaveGame)
	float LastSavedTotalTime = 0.0f;
	
	/** Whether this task is completed. */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, SaveGame)
	bool bCompleted = false;

	void AddDeltaTime(const UWorld* World);

	bool operator==(const FTimeFliesTask& Other) const
	{
		return TimeFliesTaskGUID == Other.TimeFliesTaskGUID;
	}
};
