#pragma once

#include "NativeGameplayTags.h"

namespace HeroGameplayTags
{
    /** Input Tags **/
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttackAxe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttackAxe);

    /** Player tags **/
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unequip_Axe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_HitPause);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_HitPause);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_Death);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);

    /** Enemy tags **/
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Melee);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Ability_Ranged);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);        
    
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Status_Strafing);

    /** Shared tags **/
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Ability_HitReact);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);
    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_HitReact);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);

    ACTIONADVENTURE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Status_Dead);
}