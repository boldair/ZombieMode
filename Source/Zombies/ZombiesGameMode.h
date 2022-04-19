// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZombiesGameMode.generated.h"

UCLASS(minimalapi)
class AZombiesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZombiesGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveSystem)
		int CurrentWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WaveSystem)
		int CurrentZombies;

	UFUNCTION(BlueprintCallable)
	void NextWave();

	UFUNCTION()
	void DecrementZombie();

};



