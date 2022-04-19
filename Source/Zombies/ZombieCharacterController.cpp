// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieCharacterController.h"
#include "Blueprint/UserWidget.h"

void AZombieCharacterController::BeginPlay() {
	Super::BeginPlay();
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

}
