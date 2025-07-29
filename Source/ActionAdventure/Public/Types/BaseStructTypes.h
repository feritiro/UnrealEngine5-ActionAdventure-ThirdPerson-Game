#pragma once

#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "BaseStructTypes.generated.h"

class UHeroLinkedAnimLayer;
class UHeroGameplayAbility;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FHeroAbilitySet
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
    FGameplayTag InputTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UHeroGameplayAbility> AbilityToGrant;

    bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FHeroSpecialAbilitySet : public FHeroAbilitySet
{
    GENERATED_BODY()
/*
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UMaterialInterface> AbilityIconMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Player.Cooldown"))
    FGameplayTag AbilityCooldownTag;
*/
};


USTRUCT(BlueprintType)
struct FHeroWeaponData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UHeroLinkedAnimLayer> WeaponAnimLayerToLink;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UInputMappingContext* WeaponInputMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
    TArray<FHeroAbilitySet> DefaultWeaponAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
    TArray<FHeroSpecialAbilitySet> SpecialWeaponAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FScalableFloat WeaponBaseDamage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};

