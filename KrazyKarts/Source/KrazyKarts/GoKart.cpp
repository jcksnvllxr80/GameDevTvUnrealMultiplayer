// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

void AGoKart::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( AGoKart, ServerState );
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

void AGoKart::ApplyRotation(const float DeltaTime, float SteerThrow)
{ // s = r * theta
	const float DeltaArcLength = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;
	const float RotationAngle = (DeltaArcLength * SteerThrow) / TurningRadius;
	const FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	AddActorWorldRotation(RotationDelta);
}

FGoKartMove AGoKart::CreateMove(float DeltaTime) 
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;

	return Move;
}

void AGoKart::ClearAcknowledgedMoves( FGoKartMove LastMove)
{
	TArray<FGoKartMove> NewMoves;
	for (const FGoKartMove& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}
	UnacknowledgedMoves = NewMoves;
}

// Called every frame
void AGoKart::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		const FGoKartMove Move = CreateMove(DeltaTime);
		if (!HasAuthority())
		{
			UnacknowledgedMoves.Add(Move);
			UE_LOG(LogTemp, Warning, TEXT("Queue length: %d"), UnacknowledgedMoves.Num());
		}
		
		Server_SendMove(Move);
		SimulateMove(Move);
	}
	
	DrawDebugString(GetWorld(),
		FVector(0,0,100),
		UEnum::GetValueAsString(GetLocalRole()),
		this, FColor::White, DeltaTime);
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
	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);

}

void AGoKart::SimulateMove(FGoKartMove Move)
{
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += (GetAirResistance() + GetRollingResistance());
	const FVector Acceleration = Force / Mass;
	const FVector DeltaVelocity = Acceleration * Move.DeltaTime;  // because dv/dt = acceleration
	Velocity = Velocity + DeltaVelocity;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);
	UpdateLocationFromVelocity(Move.DeltaTime);
}

void AGoKart::OnRep_ServerState()
{
	UE_LOG(LogTemp, Warning, TEXT("Replicated Actor Transform"))
	SetActorTransform(ServerState.Transform);
	Velocity = ServerState.Velocity;

	ClearAcknowledgedMoves(ServerState.LastMove);
}

void AGoKart::MoveForward(float Value)
{
	Throttle = Value;
}

void AGoKart::MoveRight(float Value)
{
	SteeringThrow = Value;
}

void AGoKart::Server_SendMove_Implementation(FGoKartMove Move)
{
	SimulateMove(Move);
	ServerState.LastMove = Move;
	ServerState.Transform = GetActorTransform();
	ServerState.Velocity = Velocity;
}

bool AGoKart::Server_SendMove_Validate(FGoKartMove Move)
{
	return true; // TODO: make better validation
}
