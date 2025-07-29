// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/HeroInputComponent.h"
#include "HeroGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "DebugHelper.h"

AHeroCharacter::AHeroCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.f;
    CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));

    HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AHeroCharacter::GetPawnCombatComponent() const
{
    return HeroCombatComponent;
}

UPawnUIComponent* AHeroCharacter::GetPawnUIComponent() const
{
    return HeroUIComponent;
}
UHeroUIComponent* AHeroCharacter::GetHeroUIComponent() const
{
    return HeroUIComponent;
}


void AHeroCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (!CharacterStartUpData.IsNull())
    {
        if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
        {
            LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent);
        }
    }
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

    ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

    check(Subsystem);

    Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

    UHeroInputComponent* HeroInputComponent = CastChecked<UHeroInputComponent>(PlayerInputComponent);

    HeroInputComponent->BindNativeInputAction(InputConfigDataAsset, HeroGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
    HeroInputComponent->BindNativeInputAction(InputConfigDataAsset, HeroGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

    HeroInputComponent->BindNativeInputAction(InputConfigDataAsset, HeroGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
    HeroInputComponent->BindNativeInputAction(InputConfigDataAsset, HeroGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

    HeroInputComponent->BindNativeInputAction(InputConfigDataAsset, HeroGameplayTags::InputTag_PickUp_Items, ETriggerEvent::Started, this, &ThisClass::Input_PickUpItemsStarted);

    HeroInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);

}

void AHeroCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
    const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

    const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

    if (MovementVector.Y != 0.f)
    {
        const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

        AddMovementInput(ForwardDirection, MovementVector.Y);
    }

    if (MovementVector.X != 0.f)
    {
        const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
    const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

    if (LookAxisVector.X != 0.f)
    {
        AddControllerYawInput(LookAxisVector.X);
    }

    if (LookAxisVector.Y != 0.f)
    {
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
    SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
    FGameplayEventData Data; 

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        this,
        SwitchDirection.X > 0.f ? HeroGameplayTags::Player_Event_SwitchTarget_Right : HeroGameplayTags::Player_Event_SwitchTarget_Left,
        Data
    );
}

void AHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
    BaseAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
    BaseAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void AHeroCharacter::Input_PickUpItemsStarted(const FInputActionValue& InputActionValue)
{
    FGameplayEventData Data;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
        this,
        HeroGameplayTags::Player_Event_ConsumeItems,
        Data
    );
}
