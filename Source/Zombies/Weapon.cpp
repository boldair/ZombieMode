// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "ZombieEnemy.h"
#include "ZombiesCharacter.h"
#include "DrawDebugHelpers.h"
#include "Chaos/ChaosEngineInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"



// Sets default values
AWeapon::AWeapon()
{

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());
	ZoomCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ZoomCamera"));
	ZoomCamera->SetupAttachment(GetRootComponent());

	Range = 15000.0f;
	Damage = 15.0f;
	HeadshotDamage = 30.0f;

	bCanShoot = true;
	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (WeaponState == EWeaponState::EWS_Pickup && OtherActor) {
		AZombiesCharacter* MainChar = Cast<AZombiesCharacter>(OtherActor);
		if (MainChar) {
			MainChar->SetOverlappingItem(this);
		}
	}
	
}



void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	AZombiesCharacter* MainChar = Cast<AZombiesCharacter>(OtherActor);
	if (MainChar) {
		MainChar->SetOverlappingItem(nullptr);
	}

}

void AWeapon::Fire(FRotator Direction, AZombiesCharacter* Initiator)
{

	if (NumberOfBullets - 1 >= 0 && !bCanShoot) {
		ShootBullet(Direction, *Initiator);
		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}
	}
	else {
		//play Empty clip sound
		bCanShoot = false;
	}
	

		
}

void AWeapon::Reload()
{
	//animation
	//after animation 
	NumberOfBullets = NumberOfBulletsMax;
	bCanShoot = true;

}


void AWeapon::ShootBullet(FRotator& Direction, AZombiesCharacter& Initiator)
{
	NumberOfBullets--;
	FVector Start;
	FVector End;

	Start = SkeletalMesh->GetSocketLocation("Muzzle");
	End = Start + (Direction.Vector() * Range);


	// additional trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = false;
	TraceParams.bReturnPhysicalMaterial = true;
	//Re-initialize hit info
	FHitResult HitDetails = FHitResult(ForceInit);
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(
		HitDetails,      // FHitResult object that will be populated with hit info
		Start,      // starting position
		End,        // end position
		ECC_Pawn,  
		TraceParams      // additional trace settings
	);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f, ECC_WorldStatic, 1.f);
	if (bIsHit)
	{
		DrawDebugBox(GetWorld(), HitDetails.ImpactPoint, FVector(1.0f, 1.0f, 1.0f), FColor::Green, false, 5.f, ECC_WorldStatic, 1.f);
		AZombieEnemy* Victim = Cast<AZombieEnemy>(HitDetails.GetActor());
		if (Victim) {
			
			UPhysicalMaterial* PhyMat = HitDetails.PhysMaterial.Get();
			if (PhyMat) {
				UGameplayStatics::ApplyDamage(Victim, Damage, WeaponInstigator, this, DamageTypeClass);
				Initiator.AddMoney(20);
				FString PMName = PhyMat->GetName();
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *PMName);

			}
			
		}
	}
	else
	{

	}
}

void AWeapon::Equip(AZombiesCharacter* Char)
{
	if (Char) {

		SetInstigator(Char->GetController());
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* WeaponSocket = Char->GetMesh1P()->GetSocketByName("WeaponSocket");
		if (WeaponSocket) {
			WeaponSocket->AttachActor(this, Char->GetMesh1P());
			


			Char->SetEquippedWeapon(this);
			WeaponState = EWeaponState::EWS_Equipped;
		}
		if (OnEquipSound) {
			UGameplayStatics::PlaySound2D(this, OnEquipSound);
		}
	}
}

