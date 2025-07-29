// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickUps/PickUpBase.h"
#include "ItemPickUpBase.generated.h"

class UBaseAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API AItemPickUpBase : public APickUpBase
{
	GENERATED_BODY()


public:
	void Consume(UBaseAbilitySystemComponent* AbilitySystemComponent, int32 ApplyLevel);

protected:
	virtual void OnPickUpCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Item Consumed"))
	void BP_OnItemConsumed();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> ItemGameplayEffectClass;
};
