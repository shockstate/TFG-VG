// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Hability.h"
#include "Mine_Ability.generated.h"

/**
 * 
 */
UCLASS()
class AMine_Ability : public AHability
{
	GENERATED_UCLASS_BODY()
	
	class USphereComponent* CollisionComp;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void DestroyObject();

	int Deploy() override;

	void ToggleVisibility(bool state) override;

protected:
	USphereComponent* SphereComponent;

public:
	// Sets default values for this actor's properties
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
};
