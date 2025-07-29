// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/EnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UBaseAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
    Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);

    if (!EnemyCombatAbilities.IsEmpty())
    {
        for (const TSubclassOf < UEnemyGameplayAbility >& AbilityClass : EnemyCombatAbilities)
        {
            if (!AbilityClass) continue;

            FGameplayAbilitySpec AbilitySpec(AbilityClass);
            AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
            AbilitySpec.Level = ApplyLevel;

            InASCToGive->GiveAbility(AbilitySpec);
        }
    }
}
