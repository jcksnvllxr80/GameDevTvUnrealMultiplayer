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

void AGoKart::UpdateLocationFromVelocity(const float DeltaTime)
{
	const FVector Translation = Velocity * DeltaTime * 100; // convert from meters to centimeters (* 100)
	FHitResult VehicleCollision;
	AddActorWorldOffset(Translation, true, &VehicleCollision);
	if (VehicleCollision.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;  // set velocity to 0
	}
}

void AGoKart::ApplyRotation(const float DeltaTime)
{ // s = r * theta
	const float DeltaArcLength = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	const float RotationAngle = (DeltaArcLength * SteeringThrow) / TurningRadius;
	const FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}

// Called every frame
void AGoKart::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += (GetAirResistance() + GetRollingResistance());
	const FVector Acceleration = Force / Mass;
	const FVector DeltaVelocity = Acceleration * DeltaTime;
	Velocity = Velocity + DeltaVelocity;

	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTime);
	
}

FVector AGoKart::GetAirResistance() const
{
	return -(Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient);
}

FVector AGoKart::GetRollingResistance() const
{
	const float NormalForce = -(GetWorld()->GetGravityZ() / 100) * Mass;
	return -(Velocity.GetSafeNormal() * NormalForce * RollingResistanceCoefficient);
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::Server_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::Server_MoveRight);

}

void AGoKart::Server_MoveForward_Implementation(float Value)
{
	Throttle = Value;
}

bool AGoKart::Server_MoveForward_Validate(float Value)
{
	return (FMath::Abs(Value) > 1) ? false : true;
}

void AGoKart::Server_MoveRight_Implementation(float Value)
{
	SteeringThrow = Value;
}

bool AGoKart::Server_MoveRight_Validate(float Value)
{
	return (FMath::Abs(Value) > 1) ? false : true;
}


