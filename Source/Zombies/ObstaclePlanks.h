// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ObstaclePlanks.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIES_API AObstaclePlanks : public AItem
{
	GENERATED_BODY()
	
public :
	AObstaclePlanks();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	int HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* Plank1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* Plank2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* Plank3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* Plank4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		class UStaticMeshComponent* Plank5;
	
	//for collision purpose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		class UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		bool bCanBlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		bool bCanBeRepaired;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public :
	UFUNCTION(BlueprintImplementableEvent)
		void OnHealthPointChanging();

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

};
