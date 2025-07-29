// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/Hero/HeroLinkedAnimLayer.h"
#include "AnimInstances/Hero/HeroAnimInstance.h"
 
UHeroAnimInstance* UHeroLinkedAnimLayer::GetHeroAnimInstance() const
{
    return Cast<UHeroAnimInstance>(GetOwningComponent()->GetAnimInstance());
}

