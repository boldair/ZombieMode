// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawn.generated.h"

UCLASS()
class ZOMBIES_API AZombieSpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AZombieSpawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Spawner)
		class UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawner)
		TSubclassOf<AActor> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawner)
		int ZombiesToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Spawner)
		FVector GetSpawningPoint();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Spawner)
		void SpawnOurActor(UClass* ToSpawn, const FVector& Location);

};
