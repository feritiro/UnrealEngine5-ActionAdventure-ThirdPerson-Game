// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Kismet/KismetMathLibrary.h"
#include "HeroGameplayTags.h"

#include "DebugHelper.h"

UBaseAbilitySystemComponent* UBaseFunctionLibrary::NativeGetBaseASCFromActor(AActor* InActor)
{
    check(InActor);

    return CastChecked<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UBaseFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    UBaseAbilitySystemComponent* ASC = NativeGetBaseASCFromActor(InActor);

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd);
    }
}

void UBaseFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
    UBaseAbilitySystemComponent* ASC = NativeGetBaseASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove);
    }
}

bool UBaseFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    UBaseAbilitySystemComponent* ASC = NativeGetBaseASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UBaseFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EBaseConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EBaseConfirmType::Yes : EBaseConfirmType::No;
}

UPawnCombatComponent* UBaseFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
    check(InActor);

    if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
    {
        return PawnCombatInterface->GetPawnCombatComponent();
    }

    return nullptr;
}

UPawnCombatComponent* UBaseFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EBaseValidType& OutValidType)
{
    UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);

    OutValidType = CombatComponent ? EBaseValidType::Valid : EBaseValidType::Invalid;

    return CombatComponent;
}

bool UBaseFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn && TargetPawn);

    IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
    {
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
    }

    return false;
}

FVector UBaseFunctionLibrary::BP_GetRollDirectionFromInput(const ACharacter* Character, const UInputAction* InputAction)
{
    if (!Character || !InputAction)
    {
        return FVector::ZeroVector;
    }

    // Get the Player Controller
    const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (!PlayerController)
    {
        return FVector::ZeroVector;
    }

    // Retrieve the Enhanced Player Input
    const UEnhancedPlayerInput* EnhancedPlayerInput = Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);
    if (!EnhancedPlayerInput)
    {
        return FVector::ZeroVector;
    }

    // Get the input value from the action
    const FInputActionValue InputValue = EnhancedPlayerInput->GetActionValue(InputAction);
    if (!InputValue.IsNonZero())
    {
        return FVector::ZeroVector;
    }

    // Retrieve the input vector
    const FVector2D InputVector = InputValue.Get<FVector2D>();
    if (InputVector.IsZero())
    {
        return FVector::ZeroVector;
    }

    // Retrieve the camera's forward and right vectors
    const FRotator CameraRotation = PlayerController->GetControlRotation();
    const FVector CameraForward = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
    const FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);

    // Combine the input vector with camera directions
    FVector RollDirection = (CameraForward * InputVector.Y) + (CameraRight * InputVector.X);

    // Normalize the result
    return RollDirection.GetSafeNormal2D();
}

float UBaseFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
    return InScalableFloat.GetValueAtLevel(InLevel);
}
FGameplayTag UBaseFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
    check(InAttacker && InVictim);

    const FVector VictimForward = InVictim->GetActorForwardVector();
    const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();

    const float DotResult = FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
    OutAngleDifference = UKismetMathLibrary::DegAcos(DotResult);

    const FVector CrossResult = FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);

    if (CrossResult.Z < 0.f)
    {
        OutAngleDifference *= -1.f;
    }

    if (OutAngleDifference >= -45.f && OutAngleDifference <= 45.f)
    {
        return HeroGameplayTags::Shared_Status_HitReact_Front; 
    }
    else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f)
    {
        return HeroGameplayTags::Shared_Status_HitReact_Left;
    }
    else if (OutAngleDifference < -135.f || OutAngleDifference>135.f)
    {
        return HeroGameplayTags::Shared_Status_HitReact_Back;
    }
    else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f)
    {
        return HeroGameplayTags::Shared_Status_HitReact_Right;
    }

    return HeroGameplayTags::Shared_Status_HitReact_Front;
}

bool UBaseFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
    check(InAttacker && InDefender);

    const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
    /*
    const FString DebugString = FString::Printf(TEXT("Dot Result: %f %s"), DotResult, DotResult < -0.1f ? TEXT("Valid Block") : TEXT("InvalidBlock"));

    Debug::Print(DebugString, DotResult < -0.1f ? FColor::Green : FColor::Red);
    */
    return DotResult < -0.1f ? true : false;
}

bool UBaseFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UBaseAbilitySystemComponent* SourceASC = NativeGetBaseASCFromActor(InInstigator);
    UBaseAbilitySystemComponent* TargetASC = NativeGetBaseASCFromActor(InTargetActor);

    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);

    return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}