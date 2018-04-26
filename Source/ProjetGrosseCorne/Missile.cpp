// Fill out your copyright notice in the Description page of Project Settings.

#include "Missile.h"
#include "Components/StaticMeshComponent.h"
#include "ProjetGrosseCorneCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMissile::AMissile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");

	//RootComponent->ComponentVelocity = GetActorForwardVector();

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	//Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMissile::OnBeginOverlap);
	Mesh->SetSimulatePhysics(true);
	RootComponent = Mesh;

	DamageAmount = 1;

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = Mesh;
	ProjectileMovement->InitialSpeed = 1000.f;
	//ProjectileMovement->MaxSpeed = 0.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	/*ProjectileMovement->SetPlaneConstraintEnabled(true);
	ProjectileMovement->SetPlaneConstraintNormal(FVector(1.0f, 0.0f, 1.0f));*/

}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();
	//ProjectileMovement->InitialSpeed = -10000.f;
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetActorLocation(GetActorLocation() + GetActorForwardVector() * 10.f * DeltaTime);

}


void AMissile::OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (!OtherActor->GetClass()->IsChildOf(AMissile::StaticClass()))
	{
		if (OtherActor->GetClass()->IsChildOf(AProjetGrosseCorneCharacter::StaticClass()))
		{
			((AProjetGrosseCorneCharacter*)OtherActor)->GetHit(DamageAmount);
		}
		this->Explode(OtherActor->GetActorLocation());
	}
}

void AMissile::NotifyHit(class UPrimitiveComponent* MyComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->GetClass()->IsChildOf(AMissile::StaticClass()))
	{
		this->Explode(HitLocation);
	}
}

void AMissile:: Explode(FVector HitLocation) {
	
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, HitLocation, GetActorRotation());
	Destroy(this);	
}

void AMissile::InitVelocity(/*const FVector& ShootDirection*/ float speed)
{
	if (ProjectileMovement)
	{
		// set the projectile's velocity to the desired direction
		ProjectileMovement->InitialSpeed = speed;
		//ProjectileMovement->Velocity = ShootDirection;
	}
}
