// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeFliesCore.h"

#include "TimeFliesSubsystem.h"

namespace TimeFliesTags
{
	UE_DEFINE_GAMEPLAY_TAG(DefaultPlayedTimeTag, FName{TEXTVIEW("TimeFlies.Identity.PlayedTime")})
}

void FTimeFliesTask::AddDeltaTime(const UWorld* World)
{
	if (!World || bCompleted) return;

	// If world is paused now and not include pause time, we do nothing.
	if (!Setting.bIncludePauseTime && World->IsPaused())
	{
		return;
	}

	float DeltaTime;
	
	// Time dilation calc.
	if (Setting.bIncludeTimeDilation)
	{
		DeltaTime = World->DeltaRealTimeSeconds * World->GetWorldSettings()->GetEffectiveTimeDilation() * Setting.CustomTimeDilation;
	}
	else
	{
		DeltaTime = World->DeltaRealTimeSeconds;
	}

	// Add
	LastSavedTotalTime += DeltaTime;

	// End time clamp and set to completed for other usage.
	if (Setting.EndTime > 0.0f && LastSavedTotalTime >= Setting.EndTime)
	{
		bCompleted = true;
		LastSavedTotalTime = Setting.EndTime;
		if (const auto Subsystem = World->GetGameInstance()->GetSubsystem<UTimeFliesSubsystem>())
		{
			Subsystem->OnTimeFliesTaskCompleteEvent.Broadcast(TimeFliesTaskGUID);
		}
	}
}
