// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Hability.h"
#include "GodCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AGodCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class UStaticMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;


public:
	AGodCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	//virtual UPawnMovementComponent* GetMovementComponent() const override;

public:
	/** Name of the MovementComponent.  Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	//static FName MovementComponentName;

private:
	/** DefaultPawn movement component */
	//UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		//UPawnMovementComponent* MovementComponent;

public:
	/** Name of the CollisionComponent. */
	static FName CollisionComponentName;

private_subobject:
	/** DefaultPawn collision component */
	DEPRECATED_FORGAME(4.6, "CollisionComponent should not be accessed directly, please use GetCollisionComponent() function instead. CollisionComponent will soon be private and your code will not compile.")
		UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* CollisionComponent;
protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void MoveUp(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	
protected:
	// APawn interface
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class UStaticMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	class AHability *activeAbility;
	/*UAbilitySystemComponent* GetAbilitySystemComponent() const override //We add this function, overriding it from IAbilitySystemInterface.
	{
	return AbilitySystem;
	};*/

	/** spawn abilities, setup initial variables */
	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayAbility> Ability;*/

	void Fire();
	void SendRayTrace();

	/**
	* [server] add ability to inventory
	*
	* @param Weapon	Weapon to add.
	*/
	void AddAbility(class AHability* Ability);
protected:

	/** [server] spawns default abilities */
	void SpawnDefaultAbilities();

	UFUNCTION(reliable, server, WithValidation)
		void ServerFire();

	UFUNCTION(reliable, server, WithValidation)
		void ServerSendRayTrace();

private:
	UPROPERTY(Transient, Replicated)
		FVector direction;

	float timerGoldPerSecond;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		int32 totalGold;

	/** Our ability system */
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;
	*/
	UFUNCTION(BlueprintCallable, Category = Setup)
		int getTotalGold();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
		TArray<TSubclassOf<class AHability> > DefaultAbilities;

	UPROPERTY(Transient, Replicated)
		TArray <class AHability*> Abilities;

};


