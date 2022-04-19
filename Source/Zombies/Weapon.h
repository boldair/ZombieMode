// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ZombiesCharacter.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
	EWS_Pickup		UMETA(DisplayName = "Pickup"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_Default_MAX UMETA(DisplayName = "DefaultMAX")
};


UCLASS()
class ZOMBIES_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ZoomCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
		class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkeletalMesh)
		class USoundCue* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Icon)
		class UTexture* Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		float Range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		int NumberOfBullets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		int NumberOfBulletsMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		float HeadshotDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		float Price;

	FORCEINLINE float GetPrice() { return Price; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		bool bIsAuto;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		bool bCanShoot; 

	FTimerHandle FireRateTimer;
	FTimerDelegate TimerDel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
		EWeaponState WeaponState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
		AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) { WeaponInstigator = Inst; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
		TSubclassOf<UDamageType> DamageTypeClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION(BlueprintCallable)
	void Fire(FRotator Direction, AZombiesCharacter* Initiator);
	void Reload();
	void ShootBullet(FRotator& Direction, AZombiesCharacter& Initiator);
	void Equip(AZombiesCharacter* Char);
};
