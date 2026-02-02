// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeFliesActor.generated.h"

class UTimeFliesReplicatedObject;

UCLASS()
class TIMEFLIES_API ATimeFliesActor : public AActor
{
	GENERATED_BODY()

public:
	ATimeFliesActor();

	UPROPERTY(BlueprintReadOnly, Replicated)
	UTimeFliesReplicatedObject* ReplicatedObject = nullptr;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void BeginReplication() override;
};
