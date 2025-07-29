// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BaseAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class ACTIONADVENTURE_API UCharacterAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY()
    ABaseCharacter* OwningCharacter;

    UPROPERTY()
    UCharacterMovementComponent* OwningMovementComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    float GroundSpeed;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    bool bHasAcceleration;  

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
    float LocomotionDirection;
};
