// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "CameraGameMode.h"


ACameraGameMode::ACameraGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	
	static ConstructorHelpers::FClassFinder<APawn> SecondPlayer(TEXT("/Game/Blueprints/BP_PawnCamera"));
	if (SecondPlayer.Succeeded()) 
		DefaultPawnClass = SecondPlayer.Class;

}

