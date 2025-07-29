// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HeroGameplayAbility.h"
#include "HeroGameplayTags.h"

void UBaseAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
    if (!InInputTag.IsValid())
    {
        return;
    }

    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag)) continue;

        if (InInputTag.MatchesTag(HeroGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive())
        {
            CancelAbilityHandle(AbilitySpec.Handle);
        }
        else
        {
            TryActivateAbility(AbilitySpec.Handle);
        }
    }
}

void UBaseAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
    if (!InInputTag.IsValid() || !InInputTag.MatchesTag(HeroGameplayTags::InputTag_MustBeHeld))
    {
        return;
    }

        for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
        {
            if (AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
            {
                CancelAbilityHandle(AbilitySpec.Handle);
            }
        }
}

void UBaseAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FHeroAbilitySet>& InDefaultWeaponAbilities, const TArray<FHeroSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
    if (InDefaultWeaponAbilities.IsEmpty())
    {
        return;
    }

    for (const FHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
    {
        if (!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
    }

    for (const FHeroSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities)
    {
        if (!AbilitySet.IsValid()) continue;

        FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
        AbilitySpec.SourceObject = GetAvatarActor();
        AbilitySpec.Level = ApplyLevel;
        AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

        OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
    }
}
void UBaseAbilitySystemComponent::RemovedGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
    if (InSpecHandlesToRemove.IsEmpty())
    {
        return;
    }

    for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
    {
        if (SpecHandle.IsValid())
        {
            ClearAbility(SpecHandle);
        }
    }

    InSpecHandlesToRemove.Empty();
}

bool UBaseAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
    check(AbilityTagToActivate.IsValid());

    TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
    GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

    if (!FoundAbilitySpecs.IsEmpty())
    {
        const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
        FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

        check(SpecToActivate);

        if (!SpecToActivate->IsActive())
        {
            return TryActivateAbility(SpecToActivate->Handle);
        }
    }

    return false; 
}