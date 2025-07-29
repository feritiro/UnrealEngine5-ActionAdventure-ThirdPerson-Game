#include "Types/BaseStructTypes.h"
#include "AbilitySystem/Abilities/HeroGameplayAbility.h"

bool FHeroAbilitySet::IsValid() const
{
    return InputTag.IsValid() && AbilityToGrant;
}
