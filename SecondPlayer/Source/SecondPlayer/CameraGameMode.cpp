// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "CameraGameMode.h"


ACameraGameMode::ACameraGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("Blueprint'/Game/Blueprints/PawnCamera1_Blueprint.PawnCamera1_Blueprint'"));
	DefaultPawnClass = APawnCamera::StaticClass();

}
