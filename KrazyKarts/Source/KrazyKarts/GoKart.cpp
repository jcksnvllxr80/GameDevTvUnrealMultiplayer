// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "Components/InputComponent.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGoKart::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * DeltaTime * 100; // convert from meters to centimeters (* 100)
	FHitResult VehicleCollision;
	AddActorWorldOffset(Translation, true, &VehicleCollision);
	if (VehicleCollision.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;  // set velocity to 0
	}
}

// Called every frame
void AGoKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	FVector Acceleration = Force / Mass;
	FVector DeltaVelocity = Acceleration * DeltaTime;
	Velocity = Velocity + DeltaVelocity;
	
	UpdateLocationFromVelocity(DeltaTime);
	
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);

}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}

