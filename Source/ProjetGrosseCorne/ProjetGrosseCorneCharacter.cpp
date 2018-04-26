// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ProjetGrosseCorneCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MissileSpawner.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"

AProjetGrosseCorneCharacter::AProjetGrosseCorneCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->TargetArmLength = 2000.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->RelativeRotation = FRotator(0.f,180.f,0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 750.f;
	GetCharacterMovement()->MaxFlySpeed = 10000.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	HitPoints = 3;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjetGrosseCorneCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjetGrosseCorneCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjetGrosseCorneCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjetGrosseCorneCharacter::TouchStopped);


}

void AProjetGrosseCorneCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void AProjetGrosseCorneCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AProjetGrosseCorneCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void AProjetGrosseCorneCharacter::GetHit(int damage)
{
	HitPoints -= damage;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("You got hit ! HP left : %i"), HitPoints));

	if (HitPoints <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("YOU DIED"));
		for (TActorIterator<AMissileSpawner> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			AMissileSpawner* mSpawner = Cast<AMissileSpawner>(*ActorItr);
			mSpawner->APlayerKilled(); 
		}
		Destroy();

	}
}