// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Engine/AssetManager.h"
#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components/WidgetComponent.h"
#include "Widgets/WidgetBase.h"
#include "Components/BoxComponent.h"
#include "BaseFunctionLibrary.h"

#include "DebugHelper.h"

AEnemyCharacter::AEnemyCharacter()
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

    EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");

    EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");

    EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
    EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

    LeftHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("LeftHandCollisionBox"); 
    LeftHandCollisionBox->SetupAttachment(GetMesh());
    LeftHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LeftHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);

    RightHandCollisionBox = CreateDefaultSubobject<UBoxComponent>("RightHandCollisionBox");
    RightHandCollisionBox->SetupAttachment(GetMesh());
    RightHandCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightHandCollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnBodyCollisionBoxBeginOverlap);
}

UPawnCombatComponent* AEnemyCharacter::GetPawnCombatComponent() const
{
    return EnemyCombatComponent;
}

void AEnemyCharacter::OnBodyCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APawn* HitPawn = Cast<APawn>(OtherActor))
    {
        if (UBaseFunctionLibrary::IsTargetPawnHostile(this, HitPawn))
        {
            EnemyCombatComponent->OnHitTargetActor(HitPawn);
        }
    }
}

UPawnUIComponent* AEnemyCharacter::GetPawnUIComponent() const
{
    return EnemyUIComponent;
}

UEnemyUIComponent* AEnemyCharacter::GetEnemyUIComponent() const
{
    return EnemyUIComponent;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (UWidgetBase* HealthWidget = Cast<UWidgetBase>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
    {
        HealthWidget->InitEnemyCreatedWidget(this);
    }
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    InitEnemyStartUpData();
}

void AEnemyCharacter::InitEnemyStartUpData()
{
    if (CharacterStartUpData.IsNull())
    {
        return;
    }

    UAssetManager::GetStreamableManager().RequestAsyncLoad(
        CharacterStartUpData.ToSoftObjectPath(),
        FStreamableDelegate::CreateLambda(
            [this]()
            {
                if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.Get())
                {
                    LoadedData->GiveToAbilitySystemComponent(BaseAbilitySystemComponent);

                }
            }
        )
    );
}

#if WITH_EDITOR
void AEnemyCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, LeftHandCollisionBoxAttachBoneName))
    {
        LeftHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandCollisionBoxAttachBoneName);
    }

    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(ThisClass, RightHandCollisionBoxAttachBoneName))
    {
        RightHandCollisionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandCollisionBoxAttachBoneName);
    }

}
#endif


