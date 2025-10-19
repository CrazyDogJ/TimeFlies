// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeFliesCore.h"
#include "UObject/Object.h"
#include "TimeFliesSettings.generated.h"

/**
 * 
 */
UCLASS(config = TimeFliesSettings, DefaultConfig, NotPlaceable)
class TIMEFLIES_API UTimeFliesSettings : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Played Time Record")
	FTimeFliesSetting PlayedTimeFliesSetting;
};
