// Fill out your copyright notice in the Description page of Project Settings.
#include "ZombieAnimInstance.h"
#include "ZombieEnemy.h"


void UZombieAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Zombie = Cast<AZombieEnemy>(Pawn);
		}
	}
}


void UZombieAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();

	}
	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = LateralSpeed.Size();

		Zombie = Cast<AZombieEnemy>(Pawn);

	}
}
