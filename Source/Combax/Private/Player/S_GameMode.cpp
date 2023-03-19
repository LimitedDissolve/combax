// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/S_GameMode.h"
#include "Player/S_Character.h"
#include "UObject/ConstructorHelpers.h"

AS_GameMode::AS_GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Mine/Blueprints/Character/S_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}
