// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "PawnCamera.h"
//#include "AbilitySystemComponent.h"

APawnCamera::APawnCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	//bReplicates = true;
	//bAlwaysRelevant = true;
	//bNetLoadOnClient = true;
	//bReplicateMovement = true;

	//DefaultAbilities = NULL;
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBlueprint(TEXT("Blueprint'/Game/Blueprints/Ability.Ability'"));

	if (WeaponBlueprint.Succeeded()) {
		AbilitySpawn = (UClass*)WeaponBlueprint.Object->GeneratedClass;

	}*/

	//static ConstructorHelpers::FObjectFinder<UClass> bpClassFinder(TEXT("Blueprint'/Game/Blueprints/Ability.Ability_C'"));
	//if (bpClassFinder.Object) {
	//	AbilitySpawn = (UClass*)bpClassFinder.Object;

	//}

	timerGoldPerSecond = 0.0f;
	totalGold = 200;
	

	//GetMeshComponent()->ToggleVisibility();
	//AutoPossessPlayer = EAutoReceiveInput::Player0;
	// Our ability system component.
	//AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));



}
void APawnCamera::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Role == ROLE_Authority)
	{
		SpawnDefaultAbilities();
		activeAbility = Abilities[0];
	}
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
	/*FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AHability *spawner = GetWorld()->SpawnActor<AHability>(DefaultAbilities[0], SpawnParams);

	if (spawner) {
		activeAbility = spawner;
	}*/
}

void APawnCamera::SpawnDefaultAbilities() {
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

void APawnCamera::AddAbility(AHability* Ability) {
	if (Ability && Role == ROLE_Authority)
	{
		Abilities.AddUnique(Ability);
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
	if (activeAbility)
		activeAbility->InitTheRay(direction, GetMeshComponent()->GetComponentLocation());
	
}

void APawnCamera::SetupPlayerInputComponent(class UInputComponent* InInputComponent) {
	Super::SetupPlayerInputComponent(InInputComponent);

	check(InInputComponent);

	InInputComponent->BindAction("Spell", IE_Pressed, this, &APawnCamera::Fire);

	//AbilitySystem->BindAbilityActivationToInputComponent(InInputComponent, FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "AbilityInput"));

}

void APawnCamera::Fire() {
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

bool APawnCamera::ServerFire_Validate() {
	return true;
}

void APawnCamera::ServerFire_Implementation() {
	Fire();
}

int APawnCamera::getTotalGold() {
	return totalGold;
}

void APawnCamera::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Only to local owner
	DOREPLIFETIME_CONDITION(APawnCamera, Abilities, COND_OwnerOnly);

	// Replicate to everyone
	DOREPLIFETIME(APawnCamera, totalGold);
	//DOREPLIFETIME(APawnCamera, AbilitySpawn);
}


