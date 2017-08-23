// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mine.h"
#include "GameFramework/Actor.h"
#include "Hability.generated.h"


UCLASS()
class SECONDPLAYER_API AHability : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHability();
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AMine> MineClass;

	UFUNCTION(Reliable, Server, WithValidation)
	void DeployRPCServer();

	virtual int Deploy();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

private:
	FVector deployLoc= FVector();
	FRotator deployRot= FRotator();
	
	bool canDeploy = false;
	bool isOnCooldown = false;
public:	
	// Called every frame
	int goldCost;
	virtual void Tick(float DeltaTime) override;
	void InitTheRay(const FVector &direction, const FVector &startPosition);
	FHitResult TraceTheRay(const FVector & TraceFrom, const FVector & TraceTo) const;
	void HitTheRay(const FHitResult & Impact, const FVector & startPosition, const FVector & direction);
	
	int32 CountdownTime;

	void AdvanceTimer();

	void CountdownHasFinished();

	FTimerHandle CountdownTimerHandle;
	
	
};
