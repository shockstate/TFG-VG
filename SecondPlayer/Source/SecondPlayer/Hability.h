// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mine.h"
#include "GameFramework/Actor.h"
#include "Hability.generated.h"


UCLASS()
class SECONDPLAYER_API AHability : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	UFUNCTION(Reliable, Server, WithValidation)
	void DeployRPCServer();

	virtual int Deploy();

	virtual void ToggleVisibility(bool state);

protected:
	FVector deployLoc= FVector();
	FRotator deployRot= FRotator();
	
	bool canDeploy = false;
	bool isOnCooldown = false;
public:	

	UPROPERTY(EditDefaultsOnly, Category = "Info")
	int goldCost;

	UPROPERTY(EditDefaultsOnly, Category = "Info")
	int32 CountdownTime;

	void InitTheRay(const FVector &direction, const FVector &startPosition);

	FHitResult TraceTheRay(const FVector & TraceFrom, const FVector & TraceTo) const;

	void HitTheRay(const FHitResult & Impact, const FVector & startPosition, const FVector & direction);
	
	void AdvanceTimer();

	void CountdownHasFinished();

	FTimerHandle CountdownTimerHandle;
	
	
};
