// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawn.h"
#include "ZombieEnemy.h"
#include "Components/BoxComponent.h"
#include "Kismet\KismetMathLibrary.h"
#include "Engine/World.h"
#include "AIController.h"

// Sets default values
AZombieSpawn::AZombieSpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));


	ZombiesToSpawn = 1;
}

// Called when the game starts or when spawned
void AZombieSpawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AZombieSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AZombieSpawn::GetSpawningPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();

	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return RandomPoint;
}

void AZombieSpawn::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn) {

		FActorSpawnParameters SpawnParams;

		if (GetWorld()) {
			AActor* Actor = GetWorld()->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.0f), SpawnParams);
			AZombieEnemy* Enemy = Cast<AZombieEnemy>(Actor);
			if (Enemy) {
				Enemy->SpawnDefaultController();
				AAIController* AICont = Cast<AAIController>(Enemy->GetController());
				if (AICont) {
					Enemy->AIController = AICont;
				}
			}
		}
	}
}

