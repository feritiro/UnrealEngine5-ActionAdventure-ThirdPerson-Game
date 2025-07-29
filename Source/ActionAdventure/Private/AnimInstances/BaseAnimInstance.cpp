// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/BaseAnimInstance.h"
#include "BaseFunctionLibrary.h"

bool UBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UBaseFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}

	return false;
}

