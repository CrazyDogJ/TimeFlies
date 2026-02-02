// Fill out your copyright notice in the Description page of Project Settings.


#include "TimeFliesReplicatedObject.h"

#include "Net/UnrealNetwork.h"

UWorld* UTimeFliesReplicatedObject::GetWorld() const
{
	return GetOuter()->GetWorld();
}

void UTimeFliesReplicatedObject::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass());
	if (BPClass != NULL)
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}

	DOREPLIFETIME(UTimeFliesReplicatedObject, bHasReplicatedMarker);
}
