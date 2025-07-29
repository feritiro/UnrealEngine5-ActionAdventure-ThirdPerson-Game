// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/BaseAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "BaseFunctionLibrary.h"
#include "HeroGameplayTags.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PawnUIComponent.h"
#include "Components/UI/HeroUIComponent.h"

#include "DebugHelper.h"
 
UBaseAttributeSet::UBaseAttributeSet()
{
    InitCurrentHealth(1.f);
    InitMaxHealth(1.f);
    InitCurrentStamina(1.f);
    InitMaxStamina(1.f);
    InitCurrentRage(1.f);
    InitMaxRage(1.f);
    InitAttackPower(1.f);
    InitDefensePower(1.f);
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (!CachedPawnUIInterface.IsValid())
    {
        CachedPawnUIInterface = TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
    }

    checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

    UPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();

    checkf(PawnUIComponent, TEXT("Couldn't extract a PawnUIComponent from %s"), *Data.Target.GetAvatarActor()->GetActorNameOrLabel());

    if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
    {
        const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);
        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
    }

    if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
    {
        const float NewCurrentStamina = FMath::Clamp(GetCurrentStamina(), 0.f, GetMaxStamina());

        SetCurrentStamina(NewCurrentStamina);

        if (GetCurrentStamina() == GetMaxStamina())
        {
            UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), HeroGameplayTags::Player_Status_Stamina_Regen);
            UBaseFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), HeroGameplayTags::Player_Status_Stamina_Full);
        }
       /* else if (GetCurrentStamina() == 0.f)
        {
            UBaseFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), HeroGameplayTags::Player_Status_Stamina_Empty);
        } */
        else
        {
            UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), HeroGameplayTags::Player_Status_Stamina_Full);
            //UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(), HeroGameplayTags::Player_Status_Stamina_Empty);

            UBaseFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), HeroGameplayTags::Player_Status_Stamina_Regen);
        }


        if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
        {
            HeroUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
        }
    }

    if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
    {
        const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());

        SetCurrentRage(NewCurrentRage);

        if (UHeroUIComponent* HeroUIComponent = CachedPawnUIInterface->GetHeroUIComponent())
        {
            HeroUIComponent->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
        }
    }

    if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
    {
        const float OldHealth = GetCurrentHealth();
        const float DamageDone = GetDamageTaken();

        const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

        SetCurrentHealth(NewCurrentHealth);
        /*
        const FString DebugString = FString::Printf(
            TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
            OldHealth,
            DamageDone,
            NewCurrentHealth
        );

        Debug::Print(DebugString, FColor::Green);
        */
        PawnUIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

        //TODO::Handle character death
        if (GetCurrentHealth() == 0.f)
        {
            UBaseFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), HeroGameplayTags::Shared_Status_Dead);

        }
    }

}
