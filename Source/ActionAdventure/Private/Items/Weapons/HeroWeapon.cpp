// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/HeroWeapon.h"


void AHeroWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
    GrantedAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> AHeroWeapon::GetGrantedAbilitySpecHandles() const
{
    return GrantedAbilitySpecHandles;
}