// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HeroGameplayAbility.h"
#include "Characters/HeroCharacter.h"
#include "Controllers/HeroController.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "HeroGameplayTags.h"

AHeroCharacter* UHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
    if (!CachedHeroCharacter.IsValid())
    {
        CachedHeroCharacter = Cast<AHeroCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AHeroController* UHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
    if (!CachedHeroController.IsValid())
    {
        CachedHeroController = Cast<AHeroController>(CurrentActorInfo->PlayerController);
    }

    return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UHeroCombatComponent* UHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

UHeroUIComponent* UHeroGameplayAbility::GetHeroUIComponentFromActorInfo()
{
    return GetHeroCharacterFromActorInfo()->GetHeroUIComponent();
}

FGameplayEffectSpecHandle UHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponBaseDamage, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
    check(EffectClass);

    FGameplayEffectContextHandle ContextHandle = GetBaseAbilitySystemComponentFromActorInfo()->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
    ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

    FGameplayEffectSpecHandle EffectSpecHandle = GetBaseAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
        EffectClass,
        GetAbilityLevel(),
        ContextHandle
    );

    EffectSpecHandle.Data->SetSetByCallerMagnitude(
        HeroGameplayTags::Shared_SetByCaller_BaseDamage,
        InWeaponBaseDamage
    );

    if (InCurrentAttackTypeTag.IsValid())
    {
        EffectSpecHandle.Data->SetSetByCallerMagnitude(InCurrentAttackTypeTag, InUsedComboCount);
    }

    return EffectSpecHandle;
}