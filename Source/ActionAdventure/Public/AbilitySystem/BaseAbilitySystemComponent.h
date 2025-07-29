// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Types/BaseStructTypes.h"
#include "BaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
 public:
 	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
 	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

    UFUNCTION(BlueprintCallable, Category = "Base|Ability", meta = (ApplyLevel = "1"))
    void GrantHeroWeaponAbilities(const TArray<FHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
    
    UFUNCTION(BlueprintCallable, Category = "Base|Ability")
    void RemovedGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

    UFUNCTION(BlueprintCallable, Category = "Base|Ability")
    bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);
};
