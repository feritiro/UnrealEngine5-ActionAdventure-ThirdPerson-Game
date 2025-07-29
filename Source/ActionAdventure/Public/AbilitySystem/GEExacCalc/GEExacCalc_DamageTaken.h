// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExacCalc_DamageTaken.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UGEExacCalc_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEExacCalc_DamageTaken();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
