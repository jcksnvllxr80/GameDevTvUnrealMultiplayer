// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementComponent.h"


// Sets default values for this component's properties
UGoKartMovementComponent::UGoKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGoKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

FVector UGoKartMovementComponent::GetVelocity() const
{
	return Velocity;
}

void UGoKartMovementComponent::SetVelocity(const FVector& Vlcty)
{
	this->Velocity = Vlcty;
}

float UGoKartMovementComponent::GetThrottle() const
{
	return Throttle;
}

void UGoKartMovementComponent::SetThrottle(const float Thrttl)
{
	this->Throttle = Thrttl;
}

float UGoKartMovementComponent::GetSteeringThrow() const
{
	return SteeringThrow;
}

void UGoKartMovementComponent::SetSteeringThrow(float StThrw)
{
	this->SteeringThrow = StThrw;
}

// Called every frame
void UGoKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UGoKartMovementComponent::GetAirResistance() const
{
	return -(Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient);
}

FVector UGoKartMovementComponent::GetRollingResistance() const
{
	const float NormalForce = -(GetWorld()->GetGravityZ() / 100) * Mass;
	return -(Velocity.GetSafeNormal() * NormalForce * RollingResistanceCoefficient);
}

void UGoKartMovementComponent::SimulateMove(const FGoKartMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += (GetAirResistance() + GetRollingResistance());
	const FVector Acceleration = Force / Mass;
	const FVector DeltaVelocity = Acceleration * Move.DeltaTime;  // because dv/dt = acceleration
	Velocity = Velocity + DeltaVelocity;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(Move.DeltaTime);
}

void UGoKartMovementComponent::UpdateLocationFromVelocity(const float DeltaTime)
{
	const FVector Translation = Velocity * DeltaTime * 100; // convert from meters to centimeters (* 100)
	FHitResult VehicleCollision;
	GetOwner()->AddActorWorldOffset(Translation, true, &VehicleCollision);
	if (VehicleCollision.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;  // set velocity to 0
	}
}

void UGoKartMovementComponent::ApplyRotation(const float DeltaTime, float SteerThrow)
{ // s = r * theta
	const float DeltaArcLength = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	const float RotationAngle = (DeltaArcLength * SteerThrow) / TurningRadius;
	const FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

FGoKartMove UGoKartMovementComponent::CreateMove(float DeltaTime) const
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;

	return Move;
}
