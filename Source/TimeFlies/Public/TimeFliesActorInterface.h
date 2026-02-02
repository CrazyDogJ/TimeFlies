// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TimeFliesReplicatedObject.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Interface.h"
#include "TimeFliesActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTimeFliesActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TIMEFLIES_API ITimeFliesActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Time Flies")
	FGuid GetActorIdentityGuid();

	virtual FGuid GetActorIdentityGuid_Implementation() const { return FGuid(); }

	UFUNCTION(BlueprintNativeEvent, Category = "Time Flies")
	void SetReplicatedObject(UTimeFliesReplicatedObject* RepObject);

	virtual void SetReplicatedObject_Implementation(UTimeFliesReplicatedObject* RepObject) const {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Time Flies")
	UTimeFliesReplicatedObject* GetReplicatedObject();

	virtual UTimeFliesReplicatedObject* GetReplicatedObject_Implementation() const { return nullptr; }
	
	UFUNCTION(BlueprintNativeEvent, Category = "Time Flies")
	TSubclassOf<UTimeFliesReplicatedObject> GetReplicatedObjectClass();

	virtual TSubclassOf<UTimeFliesReplicatedObject> GetReplicatedObjectClass_Implementation() const { return UTimeFliesReplicatedObject::StaticClass(); }
};
