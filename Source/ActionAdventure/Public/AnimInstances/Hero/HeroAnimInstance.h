// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/CharacterAnimInstance.h"
#include "HeroAnimInstance.generated.h"

class AHeroCharacter;
/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UHeroAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
	
public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|References")
    AHeroCharacter* OwningHeroCharacter;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    bool bShouldEnterRelaxState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    float EnterRelaxStateThreshold = 5.f;

    float IdleElapsedTime;
};
