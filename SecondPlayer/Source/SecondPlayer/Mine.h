// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Mine.generated.h"

UCLASS()
class SECONDPLAYER_API AMine : public AActor
{
	GENERATED_BODY()
	class USphereComponent* CollisionComp;
	
public:	
	// Sets default values for this actor's properties
	AMine();
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	USphereComponent* SphereComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyObject();
	
};
