// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HeroController.h"

AHeroController::AHeroController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeroTeamID = FGenericTeamId(0);
}


FGenericTeamId AHeroController::GetGenericTeamId() const
{
	return HeroTeamID;
}

