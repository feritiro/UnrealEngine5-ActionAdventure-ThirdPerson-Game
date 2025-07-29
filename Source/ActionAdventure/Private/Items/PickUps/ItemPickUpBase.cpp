// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickUps/ItemPickUpBase.h"
#include "Characters/HeroCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "HeroGameplayTags.h"


void AItemPickUpBase::Consume(UBaseAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel)
{
	check(ItemGameplayEffectClass);

	UGameplayEffect* EffectCDO = ItemGameplayEffectClass->GetDefaultObject<UGameplayEffect>();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		EffectCDO,
		ApplyLevel,
		AbilitySystemComponent->MakeEffectContext()
	);

	BP_OnItemConsumed();
}


void AItemPickUpBase::OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHeroCharacter* OverlappedHeroCharacter = Cast<AHeroCharacter>(OtherActor))
	{
		OverlappedHeroCharacter->GetBaseAbilitySystemComponent()->TryActivateAbilityByTag(HeroGameplayTags::Player_Ability_PickUp_Items);
	}
}
