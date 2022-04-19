// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieEnemy.h"
#include "ZombiesGameMode.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "ZombiesCharacter.h"
#include "ObstaclePlanks.h"
#include "Components/CapsuleComponent.h"
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "Runtime/Engine/Public/EngineGlobals.h"

// Sets default values
AZombieEnemy::AZombieEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 100.0f;
	Health = MaxHealth; 


	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	FistCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FistCollision"));
	FistCollision->SetupAttachment(GetMesh(), FName("EnemyAttackSocket"));

	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Head"));
	HeadCollision->SetupAttachment(GetRootComponent());
	HeadCollision->InitSphereRadius(15.f);
	bIsDead = false;
	bAttacking = false;

	

}

// Called when the game starts or when spawned
void AZombieEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	GameMode = Cast<AZombiesGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	AZombiesCharacter* Prey= Cast<AZombiesCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AZombieEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AZombieEnemy::CombatSphereOnOverlapEnd);

	DisableFistCollision();

	if (Prey) {
		TargetChar = Prey;
	}
}

// Called every frame
void AZombieEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsDead && !bAttacking) {
		MoveToTarget(TargetChar);
	}
}

// Called to bind functionality to input
void AZombieEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AZombieEnemy::TakeDamage(float Amount, FDamageEvent const& DamageEvent, AController* EventInvestigator, AActor* DamageCauser)
{
	if (Health - Amount <= 0.f) {
		Health -= Amount;

		Die();
	}
	else {
		Health -= Amount;
	}
	return Amount;
}

void AZombieEnemy::MoveToTarget(AZombiesCharacter* Goal)
{
	if (AIController) {

		//Initiliaze a AIMoveRequest to use AIMoveTo function
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Goal);
		MoveRequest.SetAcceptanceRadius(75.0f);

		//Needed too
		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);


	}
}

void AZombieEnemy::Die()
{
	if (!bIsDead) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr) {
			AnimInstance->Montage_Play(DeathAnimation, 1.f);
		}
		AIController->StopMovement();
		CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FistCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		bAttacking = false;
		if (GameMode) {
			GameMode->DecrementZombie();
		}
	}
	
	bIsDead = true;

}

void AZombieEnemy::FistOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{


}

void AZombieEnemy::FistOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AZombieEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && !bIsDead) {

		AZombiesCharacter* Main = Cast<AZombiesCharacter>(OtherActor);
		if (Main) {

			bHasValidTarget = true; 
			bOverlappingCombatSphere = true;

			/*float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AZombie::Attack, AttackTime);*/
			Attack();
		}
		AObstaclePlanks* Planks = Cast<AObstaclePlanks>(OtherActor);

		if (Planks) {
			bHasValidTarget = true;
			bOverlappingCombatSphere = true;

			/*float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AZombie::Attack, AttackTime);*/
			Attack();
		}
		
	}
}

void AZombieEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlappingCombatSphere = false;
}

void AZombieEnemy::Attack()
{

	if (!bIsDead) {
		if (AIController) {
			AIController->StopMovement();
		}
		if (!bAttacking) {
			bAttacking = true;
			UAnimInstance* Anim = GetMesh()->GetAnimInstance();
			if (Anim) {
				Anim->Montage_Play(CombatMontage, 1.0f);
			}
		}
	}
}

void AZombieEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere) {
		float AttackTime = FMath::RandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AZombieEnemy::Attack, AttackTime);

	}
}

void AZombieEnemy::DisableFistCollision()
{
	FistCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZombieEnemy::EnableFistCollision()
{
	FistCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
