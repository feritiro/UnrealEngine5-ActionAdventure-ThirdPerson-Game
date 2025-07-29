// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "EnemyGameplayAbility.generated.h"
 
class AEnemyCharacter;
class UEnemyCombatComponent;

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UEnemyGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    AEnemyCharacter* GetEnemyCharacterFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat);

private:
    TWeakObjectPtr<AEnemyCharacter> CachedEnemyCharacter;
};
