// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombiesCharacter.h"
#include "ZombiesProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "Chaos/ChaosEngineInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Weapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AZombiesCharacter

AZombiesCharacter::AZombiesCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.
	Money = 0;

	WalkingSpeed = 600.0f;
	SprintingSpeed = 1000.0f;
	bIsSprinting = false;
	bcanShoot = true;

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AZombiesCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////
// Input

void AZombiesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AZombiesCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AZombiesCharacter::StopSprint);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &AZombiesCharacter::Reload);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZombiesCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZombiesCharacter::StopFire);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AZombiesCharacter::PrimaryAction);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AZombiesCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AZombiesCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZombiesCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AZombiesCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AZombiesCharacter::LookUpAtRate);
}

void AZombiesCharacter::OnFire()
{
	FRotator Direction = FirstPersonCameraComponent->GetComponentRotation();

	// try and fire a projectile
	if (WeaponEquipped!=nullptr && !bIsSprinting) {
		WeaponEquipped->Fire(Direction, this);

		
			if (WeaponEquipped->bIsAuto) {

				GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AZombiesCharacter::OnFire, WeaponEquipped->FireRate, true);

			}
			
				if (FireAnimation != nullptr)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
					if (AnimInstance != nullptr) {
						if (WeaponEquipped->bCanShoot) {
							AnimInstance->Montage_Play(FireAnimation, 1.f);
						}
					}
				}
			
			

		
		
	}
	
}

void AZombiesCharacter::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
}

void AZombiesCharacter::Reload()
{
	if (WeaponEquipped != nullptr) {
		WeaponEquipped->Reload();
	}
}

void AZombiesCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AZombiesCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AZombiesCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AZombiesCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AZombiesCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AZombiesCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AZombiesCharacter::Sprint()
{
	bIsSprinting = true;

	GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
}

void AZombiesCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void AZombiesCharacter::PrimaryAction()
{
	if (ActiveOverlappingItem) {
		AWeapon* WeaponToPickup = Cast<AWeapon>(ActiveOverlappingItem);
		if (WeaponToPickup) {
			float PriceW = WeaponToPickup->GetPrice();
			if(PriceW){
				if (Money - PriceW >= 0) {
					WeaponToPickup->Equip(this);
					Money -= PriceW;
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("MESCOUILLES"));
				}
			
			}
			
		}
	}
}

void AZombiesCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZombiesCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AZombiesCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AZombiesCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AZombiesCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AZombiesCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void AZombiesCharacter::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	if (WeaponEquipped) {
		WeaponEquipped->Destroy();
	}

	WeaponEquipped = WeaponToSet;
}

void AZombiesCharacter::AddMoney(int Amount)
{
	Money += Amount;
}

bool AZombiesCharacter::SpendMoney(int Amount)
{
	if (Money - Amount < 0) {
		return false;
	}
	Money -= Amount; 
	return true;
}
