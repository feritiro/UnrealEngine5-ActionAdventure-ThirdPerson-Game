// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "HeroGameplayAbility.generated.h"

class AHeroCharacter;
class AHeroController;
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UHeroGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    AHeroCharacter* GetHeroCharacterFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    AHeroController* GetHeroControllerFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    UHeroUIComponent* GetHeroUIComponentFromActorInfo();

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    FGameplayEffectSpecHandle MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

private:
    TWeakObjectPtr<AHeroCharacter> CachedHeroCharacter;
    TWeakObjectPtr<AHeroController> CachedHeroController;
};
