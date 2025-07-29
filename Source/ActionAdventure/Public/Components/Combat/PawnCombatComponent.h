// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtensionComponentBase.h"
#include "GameplayTagContainer.h"
#include "PawnCombatComponent.generated.h"

class ABaseWeapon;


UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
    CurrentEquippedWeapon,
    LeftHand,
    RightHand
};
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Base|Combat")
    void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ABaseWeapon* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

    UFUNCTION(BlueprintCallable, Category = "Base|Combat")
    ABaseWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

    UFUNCTION(BlueprintCallable, Category = "Base|Combat")
    void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

    UPROPERTY(BlueprintReadWrite, Category = "Base|Combat")
    FGameplayTag CurrentEquippedWeaponTag;

    UFUNCTION(BlueprintCallable, Category = "Base|Combat")
    ABaseWeapon* GetCharacterCurrentEquippedWeapon() const;

private:
    TMap<FGameplayTag, ABaseWeapon*> CharacterCarriedWeaponMap;
    virtual void OnHitTargetActor(AActor* HitActor);
    virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor);

protected:
    virtual void ToggleCurrentEquippedWeaponCollision(bool bShouldEnable);
    virtual void ToggleBodyCollsionBoxCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType);

    TArray<AActor*> OverlappedActors;
};
