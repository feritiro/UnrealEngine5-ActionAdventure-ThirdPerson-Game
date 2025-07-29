// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    check(InASCToGive);

    GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
    GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

    if (!StartUpGameplayEffects.IsEmpty())
    {
        for (const TSubclassOf < UGameplayEffect >& EffectClass : StartUpGameplayEffects)
        {
            if (!EffectClass) continue;

            UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();

            InASCToGive->ApplyGameplayEffectToSelf(
                EffectCDO,
                ApplyLevel,
                InASCToGive->MakeEffectContext()
            );
        }
    }
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UBaseGameplayAbility>>& InAbilitiesToGive, UBaseAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    if (InAbilitiesToGive.IsEmpty())
    {
        return;
    }

    for (const TSubclassOf<UBaseGameplayAbility>& Ability : InAbilitiesToGive)
    {
        if (!Ability) continue;

        FGameplayAbilitySpec AbilitySpec(Ability);
        AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;

        InASCToGive->GiveAbility(AbilitySpec);
    }
}

