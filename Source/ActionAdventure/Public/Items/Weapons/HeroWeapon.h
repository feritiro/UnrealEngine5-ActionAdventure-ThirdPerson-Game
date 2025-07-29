// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Weapons/BaseWeapon.h"
#include "Types/BaseStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "HeroWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API AHeroWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FHeroWeaponData HeroWeaponData;

    UFUNCTION(BlueprintCallable)
    void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

    UFUNCTION(BlueprintPure)
    TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
    TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
	
};
