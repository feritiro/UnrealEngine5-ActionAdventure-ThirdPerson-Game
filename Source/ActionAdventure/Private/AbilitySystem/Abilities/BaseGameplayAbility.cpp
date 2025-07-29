#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseFunctionLibrary.h"
#include "HeroGameplayTags.h"
#include "AbilitySystemGlobals.h"

void UBaseGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnGiveAbility(ActorInfo, Spec);

    if (AbilityActivationPolicy == EBaseAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo && !Spec.IsActive())
        {
            ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
        }
    }
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

    if (AbilityActivationPolicy == EBaseAbilityActivationPolicy::OnGiven)
    {
        if (ActorInfo)
        {
            ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
        }
    }
}

bool UBaseGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	// Define a common lambda to check for blocked tags
	bool bBlocked = false;
	auto CheckForBlocked = [&](const FGameplayTagContainer& ContainerA, const FGameplayTagContainer& ContainerB)
		{
			// Do we not have any tags in common?  Then we're not blocked
			if (ContainerA.IsEmpty() || ContainerB.IsEmpty() || !ContainerA.HasAny(ContainerB))
			{
				return;
			}

			if (OptionalRelevantTags)
			{
				// Ensure the global blocking tag is only added once
				if (!bBlocked)
				{
					UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
					const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
					OptionalRelevantTags->AddTag(BlockedTag);
				}

				// Now append all the blocking tags
				OptionalRelevantTags->AppendMatchingTags(ContainerA, ContainerB);
			}

			bBlocked = true;
		};

	// Define a common lambda to check for missing required tags
	bool bMissing = false;
	auto CheckForRequired = [&](const FGameplayTagContainer& TagsToCheck, const FGameplayTagContainer& RequiredTags)
		{
			// Do we have no requirements, or have met all requirements?  Then nothing's missing
			if (RequiredTags.IsEmpty() || TagsToCheck.HasAll(RequiredTags))
			{
				return;
			}

			if (OptionalRelevantTags)
			{
				// Ensure the global missing tag is only added once
				if (!bMissing)
				{
					UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
					const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;
					OptionalRelevantTags->AddTag(MissingTag);
				}

				FGameplayTagContainer MissingTags = RequiredTags;
				MissingTags.RemoveTags(TagsToCheck.GetGameplayTagParents());
				OptionalRelevantTags->AppendTags(MissingTags);
			}

			bMissing = true;
		};

	// Start by checking all of the blocked tags first (so OptionalRelevantTags will contain blocked tags first)
	CheckForBlocked(GetAssetTags(), AbilitySystemComponent.GetBlockedAbilityTags());
	CheckForBlocked(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationBlockedTags);
	if (SourceTags != nullptr)
	{
		CheckForBlocked(*SourceTags, SourceBlockedTags);
	}
	if (TargetTags != nullptr)
	{
		CheckForBlocked(*TargetTags, TargetBlockedTags);
	}

	// Now check all required tags
	CheckForRequired(AbilitySystemComponent.GetOwnedGameplayTags(), ActivationRequiredTags);
	if (SourceTags != nullptr)
	{
		CheckForRequired(*SourceTags, SourceRequiredTags);
	}
	if (TargetTags != nullptr)
	{
		CheckForRequired(*TargetTags, TargetRequiredTags);
	}

	// We succeeded if there were no blocked tags and no missing required tags	
	return !bBlocked && !bMissing;
}

UPawnCombatComponent* UBaseGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
    return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}
UBaseAbilitySystemComponent* UBaseGameplayAbility::GetBaseAbilitySystemComponentFromActorInfo() const
{
    return Cast<UBaseAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}


FActiveGameplayEffectHandle UBaseGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
    UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

    check(TargetASC && InSpecHandle.IsValid());

    return GetBaseAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
        *InSpecHandle.Data,
        TargetASC
    );
}

FActiveGameplayEffectHandle UBaseGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EBaseSuccessType& OutSuccessType)
{
    FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

    OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EBaseSuccessType::Successful : EBaseSuccessType::Failed;

    return ActiveGameplayEffectHandle;
}

void UBaseGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty())
	{
		return;
	}

	APawn* OwningPawn = CastChecked<APawn>(GetAvatarActorFromActorInfo());

	for (const FHitResult& Hit : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
		{
			if (UBaseFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn))
			{
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(HitPawn, InSpecHandle);

				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;

					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						HeroGameplayTags::Shared_Event_HitReact,
						Data
					);
				}
			}
		}
	}
}