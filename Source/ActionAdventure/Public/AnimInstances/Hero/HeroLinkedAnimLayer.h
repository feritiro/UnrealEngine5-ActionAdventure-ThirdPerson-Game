// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "HeroLinkedAnimLayer.generated.h"

class UHeroAnimInstance;
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UHeroLinkedAnimLayer : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UHeroAnimInstance* GetHeroAnimInstance() const;
	
};
