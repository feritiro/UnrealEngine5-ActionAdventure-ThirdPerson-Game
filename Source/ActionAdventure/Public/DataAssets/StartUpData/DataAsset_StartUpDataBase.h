// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UBaseGameplayAbility;
class UBaseAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
    virtual void GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
    TArray< TSubclassOf < UBaseGameplayAbility > > ActivateOnGivenAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
    TArray< TSubclassOf < UBaseGameplayAbility > > ReactiveAbilities;

    UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
    TArray< TSubclassOf < UGameplayEffect > > StartUpGameplayEffects;

    void GrantAbilities(const TArray< TSubclassOf < UBaseGameplayAbility > >& InAbilitiesToGive, UBaseAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
