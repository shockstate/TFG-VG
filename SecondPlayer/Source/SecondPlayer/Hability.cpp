// Fill out your copyright notice in the Description page of Project Settings.

#include "SecondPlayer.h"
#include "Hability.h"
#define COLLISION_PROJECTILE ECC_GameTraceChannel1


// Sets default values
AHability::AHability(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) 
{
 	
	//bReplicates = true;
	//bAlwaysRelevant = true;
	//bNetLoadOnClient = true;


	//deployLoc = FVector();
	//deployRot = FRotator();
	CountdownTime = 3;
	//goldCost = 30;
}
	

int AHability::Deploy()
{
	return 0;
}

void AHability::InitTheRay(const FVector &direction, const FVector &startPosition) {

	const FVector EndTrace = startPosition + direction * 3000.0f;
	const FHitResult Impact = TraceTheRay(startPosition, EndTrace);

	HitTheRay(Impact, startPosition, direction);
}


FHitResult AHability::TraceTheRay(const FVector &TraceFrom, const FVector &TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("Vision"));
	FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);
	FHitResult Hit(ForceInit);
		
		 GetWorld()->LineTraceSingleByChannel(Hit, TraceFrom, TraceTo, ECC_GameTraceChannel1, TraceParams);
		
		/*if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("aha").Append());
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Blocking Hit =: %s"), hiteado));
			*/
	return Hit;
}

void AHability::HitTheRay(const FHitResult & Impact, const FVector & startPosition, const FVector &direction)
{
	const FVector EndTrace = startPosition + direction * 3000.0f;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;


	//DrawDebugLine(this->GetWorld(), startPosition, EndPoint, FColor::Black, true, 10000, 10.f);
	deployLoc = EndPoint;
	if (Impact.GetActor()) {
		float thickness = 5.0f;
		
		///FVector yAxis = FRotationMatrix(Impact.GetActor()->GetActorRotation()).GetScaledAxis(EAxis::Y);
		FVector yAxis = Impact.ImpactNormal;
		//FVector zAxis = FRotationMatrix(Impact.GetActor()->GetActorRotation()).GetScaledAxis(EAxis::Z);
		FVector prueba = FVector(1.0f, 0.0f, 0.0f);
		FVector yAxis3 = FVector(yAxis.Z, yAxis.X, yAxis.Y);
		FVector yAxis2 = FVector(yAxis.Y, yAxis.Z, yAxis.X);
		//UE_LOG(LogTemp, Warning, TEXT("vectores %s, \n %s"), *yAxis.ToString(), *prueba.ToString());
		canDeploy = true;
		DrawDebugCircle(this->GetWorld(), EndPoint, 20.0f, 200, FColor::Green, false, -1.0f, 0, thickness, 
			yAxis3,yAxis2, false);

	}
	else canDeploy = false;
}
void AHability::AdvanceTimer()
{
	--CountdownTime;
	if (CountdownTime < 1)
	{
		// We're done counting down, so stop running the timer.
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		//Perform any special actions we want to do when the timer ends.
		CountdownHasFinished();
	}
}

void AHability::CountdownHasFinished()
{
	//Change to a special readout
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("VEC"));
	isOnCooldown = false;
	CountdownTime = 3;

}

bool AHability::DeployRPCServer_Validate() {
	//Habria que validar algo del plan tengo oro para invocarla 
	return true;
}

void AHability::DeployRPCServer_Implementation() {
	 Deploy();
}

void AHability::ToggleVisibility(bool state) {
	
}

//void AHability::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	// Replicate to everyone
//	DOREPLIFETIME(AHability, MineClass);
//}