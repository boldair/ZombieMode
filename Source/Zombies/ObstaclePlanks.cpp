// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstaclePlanks.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "ZombieEnemy.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"

AObstaclePlanks::AObstaclePlanks() {

	

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetupAttachment(GetRootComponent());
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	Plank1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank1"));
	Plank1->SetupAttachment(GetRootComponent());
	
	Plank2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank2"));
	Plank2->SetupAttachment(GetRootComponent());

	Plank3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank3"));
	Plank3->SetupAttachment(GetRootComponent());

	Plank4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank4"));
	Plank4->SetupAttachment(GetRootComponent());

	Plank5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plank5"));
	Plank5->SetupAttachment(GetRootComponent());

	HealthPoints = 5;
	
	bCanBeRepaired = false; 
	bCanBlock = true;
}
void AObstaclePlanks::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AObstaclePlanks::OnOverlapBegin);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AObstaclePlanks::OnOverlapEnd);
}

void AObstaclePlanks::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


}

void AObstaclePlanks::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}
