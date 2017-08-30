// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SecondPlayer.h"
#include "GodCharacter.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AGodCharacter

AGodCharacter::AGodCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	//MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(ADefaultPawn::MovementComponentName);
	//MovementComponent->UpdatedComponent = CollisionComponent;


	timerGoldPerSecond = 0.0f;
	totalGold = 200;
}



/*UPawnMovementComponent* AGodCharacter::GetMovementComponent() const
{
	return CharacterMovement;
}*/

/*
void AGodCharacter::SetReplicateMovement(bool bInReplicateMovement)
{
	Super::SetReplicateMovement(bInReplicateMovement);

	if (MovementComponent != nullptr && Role == ROLE_Authority)
	{
		// Set prediction data time stamp to current time to stop extrapolating
		// from time bReplicateMovement was turned off to when it was turned on again
		FNetworkPredictionData_Server* NetworkPrediction = MovementComponent->HasPredictionData_Server() ? MovementComponent->GetPredictionData_Server() : nullptr;

		if (NetworkPrediction != nullptr)
		{
			NetworkPrediction->ServerTimeStamp = GetWorld()->GetTimeSeconds();
		}
	}
}
*/
void AGodCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	//Mesh1P->SetHiddenInGame(false, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGodCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUP", this, &AGodCharacter::MoveUp);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &AGodCharacter::StopJumping);

	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AGodCharacter::OnFire);
	PlayerInputComponent->BindAction("Spell", IE_Pressed, this, &AGodCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGodCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGodCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGodCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGodCharacter::LookUpAtRate);
}

void AGodCharacter::OnFire()
{
	// try and fire a projectile
	//if (ProjectileClass != NULL)
	//{
	//	UWorld* const World = GetWorld();
	//	if (World != NULL)
	//	{
	//		if (bUsingMotionControllers)
	//		{
	//			const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
	//			const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
	//			World->SpawnActor<AcharacterTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	//		}
	//		else
	//		{
	//			const FRotator SpawnRotation = GetControlRotation();
	//			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	//			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

	//			//Set Spawn Collision Handling Override
	//			FActorSpawnParameters ActorSpawnParams;
	//			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	//			// spawn the projectile at the muzzle
	//			World->SpawnActor<AcharacterTestProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
	//		}
	//	}
	//}

	//// try and play the sound if specified
	//if (FireSound != NULL)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	//}

	//// try and play a firing animation if specified
	//if (FireAnimation != NULL)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if (AnimInstance != NULL)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}
}


void AGodCharacter::MoveUp(float Value) {


	if (Value != 0.f)
	{
		AddMovementInput(FVector::UpVector, Value);
	}

}

void AGodCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		//AddMovementInput(GetActorForwardVector(), Value);

		FRotator const ControlSpaceRot = Controller->GetControlRotation();

		// transform to world space and add it
		AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Value);


	}
}

void AGodCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AGodCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGodCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGodCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		SpawnDefaultAbilities();
		activeAbility = Abilities[0];
	}
}

void AGodCharacter::SpawnDefaultAbilities() {
	if (Role < ROLE_Authority)
	{
		return;
	}

	int32 NumAbilitiesClasses = DefaultAbilities.Num();
	for (int32 i = 0; i < NumAbilitiesClasses; i++)
	{
		if (DefaultAbilities[i])
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			SpawnInfo.Instigator = Instigator;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AHability* NewHability = GetWorld()->SpawnActor<AHability>(DefaultAbilities[i], SpawnInfo);
			NewHability->ToggleVisibility(false);
			AddAbility(NewHability);
		}
	}

	// equip first ability in inventory
	//if (Abilities.Num() > 0)
	//{
	//	EquipWeapon(Inventory[0]);
	//}
}

void AGodCharacter::AddAbility(AHability* Ability) {
	if (Ability && Role == ROLE_Authority)
	{
		Abilities.AddUnique(Ability);
	}
}

void AGodCharacter::Tick(float DeltaTime)
{
	timerGoldPerSecond += DeltaTime;
	if (timerGoldPerSecond > 5.0f) {
		totalGold += 10;
		timerGoldPerSecond = 0.0f;
	}
	Super::Tick(DeltaTime);
	SendRayTrace();


}

void AGodCharacter::SendRayTrace() {
	if (Role < ROLE_Authority) {
		ServerSendRayTrace();
	}
	else {
		const FRotator rotation = GetViewRotation();
		direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::X);
		if (activeAbility)
			activeAbility->InitTheRay(direction, GetActorLocation(), this);
	}
}

bool AGodCharacter::ServerSendRayTrace_Validate() {
	return true;
}

void AGodCharacter::ServerSendRayTrace_Implementation() {
	SendRayTrace();
}

void AGodCharacter::Fire() {
	if (Role < ROLE_Authority) {
		ServerFire();
	}
	else {
		if (activeAbility) {
			if (totalGold > activeAbility->goldCost) {
				int goldSpent = activeAbility->Deploy();
				totalGold = totalGold - goldSpent;
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Something went wrong trying to fire!"));
		}

	}

}

bool AGodCharacter::ServerFire_Validate() {
	return true;
}

void AGodCharacter::ServerFire_Implementation() {
	Fire();
}

int AGodCharacter::getTotalGold() {
	return totalGold;
}

void AGodCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Only to local owner
	DOREPLIFETIME_CONDITION(AGodCharacter, Abilities, COND_OwnerOnly);

	// Replicate to everyone
	DOREPLIFETIME(AGodCharacter, totalGold);
	DOREPLIFETIME(AGodCharacter, direction);
	//DOREPLIFETIME(APawnCamera, AbilitySpawn);
}
