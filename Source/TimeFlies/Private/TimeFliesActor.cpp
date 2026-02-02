// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeFliesActor.h"

#include "TimeFliesReplicatedObject.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ATimeFliesActor::ATimeFliesActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);
}

void ATimeFliesActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ReplicatedObject)
}

bool ATimeFliesActor::ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch,
                                          FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (ReplicatedObject && IsValid(ReplicatedObject))
	{
		WroteSomething |= Channel->ReplicateSubobject(ReplicatedObject, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void ATimeFliesActor::BeginReplication()
{
	Super::BeginReplication();
	
	if (ReplicatedObject)
	{
		AddReplicatedSubObject(ReplicatedObject);
	}
}
