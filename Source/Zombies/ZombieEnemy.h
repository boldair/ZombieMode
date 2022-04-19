// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombiesCharacter.h"
#include "GameFramework/Character.h"
#include "ZombieEnemy.generated.h"

UCLASS()
class ZOMBIES_API AZombieEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombieEnemy();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WaveSystem)
		class AZombiesGameMode* GameMode;
	
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	//	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		class UBoxComponent* LegsCollision; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		class UBoxComponent* TorsoCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		class UBoxComponent* FistCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		class USphereComponent* HeadCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		class USphereComponent* CombatSphere;
	
	bool bHasValidTarget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	bool bOverlappingCombatSphere;
	bool bAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	class AZombiesCharacter* TargetChar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
		class AAIController* AIController;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		class UAnimMontage* CombatMontage;
	//Timer for delaying attacks 
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float AttackMaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TSubclassOf<UDamageType> DamageTypeClass;

	//Timer for destoring corpse
	FTimerHandle DeathTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		float DeathDelay;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* DeathAnimation;

	bool bIsDead;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Amount, struct FDamageEvent const& DamageEvent, class AController* EventInvestigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(AZombiesCharacter* Target);

	/**Called when enemy lose all its Health*/
	void Die();

	FORCEINLINE class UCapsuleComponent* GetCapsuleCollision() { return GetCapsuleComponent(); }
	//Handle the fists collision
	UFUNCTION()
		void FistOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void FistOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Target in radius of attack
	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
		void Attack();
	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	UFUNCTION(BlueprintCallable)
		void DisableFistCollision();
	UFUNCTION(BlueprintCallable)
		void EnableFistCollision();
};
