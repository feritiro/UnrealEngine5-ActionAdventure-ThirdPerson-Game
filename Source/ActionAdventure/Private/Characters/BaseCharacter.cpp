#include "Characters/BaseCharacter.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "MotionWarpingComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false; 
    
    BaseAbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("BaseAbilitySystemComponent"));

    BaseAttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("BaseAttributeSet"));

    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* ABaseCharacter::GetAbilitySystemComponent() const
{
    return GetBaseAbilitySystemComponent();
}

UPawnCombatComponent* ABaseCharacter::GetPawnCombatComponent() const
{
    return nullptr;
}

UPawnUIComponent* ABaseCharacter::GetPawnUIComponent() const
{
    return nullptr;
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (BaseAbilitySystemComponent)
    {
        BaseAbilitySystemComponent->InitAbilityActorInfo(this, this);

        ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
    }

}

