// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "PawnCamera.h"
//#include "AbilitySystemComponent.h"

APawnCamera::APawnCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	//bReplicateMovement = true;

	AbilitySpawn = NULL;
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBlueprint(TEXT("Blueprint'/Game/Blueprints/Ability.Ability'"));

	if (WeaponBlueprint.Succeeded()) {
		AbilitySpawn = (UClass*)WeaponBlueprint.Object->GeneratedClass;

	}*/

	static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("Blueprint'/Game/Blueprints/Ability.Ability_C'"));
	if (bpClassFinder.Object) {
		AbilitySpawn = (UClass*)bpClassFinder.Object;

	}


	timerGoldPerSecond = 0.0f;
	totalGold = 200;
	

	//GetMeshComponent()->ToggleVisibility();
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Our ability system component.
	//AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));



}

void APawnCamera::BeginPlay() {
	Super::BeginPlay();

	/*if (AbilitySystem)
	{
		if (HasAuthority() && Ability)
		{
			AbilitySystem->GiveAbility(FGameplayAbilitySpec(Ability.GetDefaultObject(), 1, 0));
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
	*/
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AHability *spawner = GetWorld()->SpawnActor<AHability>(AbilitySpawn, SpawnParams);

	if (spawner) {
		activeAbility = spawner;
	}
}

void APawnCamera::Tick(float DeltaTime)
{	
	timerGoldPerSecond+= DeltaTime;
	if (timerGoldPerSecond > 5.0f) {
		totalGold += 10;
		timerGoldPerSecond = 0.0f;
	}
	Super::Tick(DeltaTime);
	const FRotator rotation = GetViewRotation();
	direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::X);
	activeAbility->InitTheRay(direction, GetMeshComponent()->GetComponentLocation());
	
}

void APawnCamera::SetupPlayerInputComponent(class UInputComponent* InInputComponent) {
	Super::SetupPlayerInputComponent(InInputComponent);

	check(InInputComponent);

	InInputComponent->BindAction("Spell", IE_Pressed, this, &APawnCamera::Fire);

	//AbilitySystem->BindAbilityActivationToInputComponent(InInputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));

}

void APawnCamera::Fire() {
	if (totalGold > activeAbility->goldCost) {
		int goldSpent = activeAbility->Deploy();
		totalGold = totalGold - goldSpent;
	}
}

int APawnCamera::getTotalGold() {
	return totalGold;
}

void APawnCamera::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(APawnCamera, totalGold);
}


