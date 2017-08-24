// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "Mine_Ability.h"

AMine_Ability::AMine_Ability(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(10.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	SphereVisual->BodyInstance.SetCollisionProfileName("Mine");
	SphereVisual->OnComponentHit.AddDynamic(this, &AMine_Ability::OnHit);

	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
}


void AMine_Ability::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("VEC"));

	//UMaterialInstanceDynamic* DynamicMaterial = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	/* Change color to yellow */
	//DynamicMaterial->SetVectorParameterValue("BodyColor", FLinearColor::Yellow);
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMine_Ability::DestroyObject, 3.0f, false);

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("VEC"));
	}
}

void AMine_Ability::DestroyObject() {
	Destroy();
}

int AMine_Ability::Deploy() {
	//Sino soy el server llamo a la funcion del server
	if (Role < ROLE_Authority) {
		DeployRPCServer();
	}
	//Este codigo lo ejecuta el server
	else {
		UWorld* const World = GetWorld();
		if (World != NULL && canDeploy && !isOnCooldown)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			AMine_Ability* mine = World->SpawnActor<AMine_Ability>(deployLoc, deployRot, SpawnParams);
			GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &AHability::AdvanceTimer, 1.0f, true);
			isOnCooldown = true;
			return goldCost;
		}
	}
	return 0;
}

void AMine_Ability::ToggleVisibility(bool state) {
	//TArray<UStaticMeshComponent*> Components;
	//GetComponents<UStaticMeshComponent>(Components);
	//for (int32 i = 0; i<Components.Num(); i++)
	//{
	//	UStaticMeshComponent* StaticMeshComponent = Components[i];
	//	//UStaticMesh* StaticMesh = StaticMeshComponent->StaticMesh;
	//	StaticMeshComponent->SetVisibility(state);
	//	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
}

