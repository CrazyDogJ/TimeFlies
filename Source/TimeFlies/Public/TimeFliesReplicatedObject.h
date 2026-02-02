// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TimeFliesReplicatedObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TIMEFLIES_API UTimeFliesReplicatedObject : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Tick(float DeltaTime);
	
	virtual void NativeTick(float DeltaTime) { Tick( DeltaTime ); }

	/** Replicated object fixing property. */
	UPROPERTY(Replicated)
	uint8 bHasReplicatedMarker : 1;
	
protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	virtual bool ImplementsGetWorld() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
