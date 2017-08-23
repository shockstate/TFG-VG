// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "SecondPlayerTest.h"


// Sets default values
ASecondPlayerTest::ASecondPlayerTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASecondPlayerTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASecondPlayerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASecondPlayerTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

