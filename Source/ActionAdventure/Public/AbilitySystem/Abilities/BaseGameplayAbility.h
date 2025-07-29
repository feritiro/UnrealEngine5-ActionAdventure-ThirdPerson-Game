// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Types/BaseEnumTypes.h"
#include "BaseGameplayAbility.generated.h"


class UPawnCombatComponent;
class UBaseAbilitySystemComponent;

UENUM(BlueprintType)
enum class EBaseAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
    //~ Begin UGameplayAbility Interface.
    virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
    //~ End UGameplayAbility Interface
 
    //~ Begin UGameplayAbility Interface.
    virtual bool DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
    //~ End UGameplayAbility Interface

    UPROPERTY(EditDefaultsOnly, Category = "BaseAbility")
    EBaseAbilityActivationPolicy AbilityActivationPolicy = EBaseAbilityActivationPolicy::OnTriggered;

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    UPawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

    UFUNCTION(BlueprintPure, Category = "Base|Ability")
    UBaseAbilitySystemComponent* GetBaseAbilitySystemComponentFromActorInfo() const;

    FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

    UFUNCTION(BlueprintCallable, Category = "Base|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
    FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EBaseSuccessType& OutSuccessType);

    UFUNCTION(BlueprintCallable, Category = "Base|Ability")
    void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults);

};