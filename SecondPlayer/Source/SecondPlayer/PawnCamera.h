// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DefaultPawn.h"
#include "Hability.h"
//#include "AbilitySystemInterface.h"
#include "PawnCamera.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class SECONDPLAYER_API APawnCamera : public ADefaultPawn
{
	GENERATED_UCLASS_BODY()
		
	class AHability *activeAbility;
	/*UAbilitySystemComponent* GetAbilitySystemComponent() const override //We add this function, overriding it from IAbilitySystemInterface.
	{
		return AbilitySystem;
	};*/

	/** spawn abilities, setup initial variables */
	virtual void PostInitializeComponents() override;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
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

/*UENUM(BlueprintType)
enum class AbilityInput : uint8
{
	UseAbility1 UMETA(DisplayName = "Use Spell 1"), //This maps the first ability(input ID should be 0 in int) to the action mapping(which you define in the project settings) by the name of "UseAbility1". "Use Spell 1" is the blueprint name of the element.
	UseAbility2 UMETA(DisplayName = "Use Spell 2"), //Maps ability 2(input ID 1) to action mapping UseAbility2. "Use Spell 2" is mostly used for when the enum is a blueprint variable.
	UseAbility3 UMETA(DisplayName = "Use Spell 3"),
	UseAbility4 UMETA(DisplayName = "Use Spell 4"),
	WeaponAbility UMETA(DisplayName = "Use Weapon"), //This finally maps the fifth ability(here designated to be your weaponability, or auto-attack, or whatever) to action mapping "WeaponAbility".

													 //You may also do something like define an enum element name that is not actually mapped to an input, for example if you have a passive ability that isn't supposed to have an input. This isn't usually necessary though as you usually grant abilities via input ID,
													 //which can be negative while enums cannot. In fact, a constant called "INDEX_NONE" exists for the exact purpose of rendering an input as unavailable, and it's simply defined as -1.
													 //Because abilities are granted by input ID, which is an int, you may use enum elements to describe the ID anyway however, because enums are fancily dressed up ints.
};
*/
