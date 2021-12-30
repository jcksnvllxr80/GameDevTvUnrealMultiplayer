// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKartMovementReplicator.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UGoKartMovementReplicator::UGoKartMovementReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);

	// ...
}

// Called when the game starts
void UGoKartMovementReplicator::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<UGoKartMovementComponent>();
	
}

// Called every frame
void UGoKartMovementReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;
	FGoKartMove LastMove = MovementComponent->GetLastMove();
	
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		AddUnacknowledgedMove(LastMove);
		Server_SendMove(LastMove);
	}

	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(LastMove);
		// UE_LOG(LogTemp, Warning, TEXT("Queue length: %d"), GetUnacknowledgedMoves().Num());
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
		// MovementComponent->SimulateMove(GetServerState().LastMove);
	}
}

FGoKartState UGoKartMovementReplicator::GetServerState() const
{
	return ServerState;
}

void UGoKartMovementReplicator::SetServerState(const FGoKartState& Value)
{
	this->ServerState = Value;
}

void UGoKartMovementReplicator::UpdateServerState(const FGoKartMove& Move)
{
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UGoKartMovementReplicator::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBtwnLastUpdates < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;
	const float LerpRatio = ClientTimeSinceUpdate / ClientTimeBtwnLastUpdates;
	const FVector TargetLocation = GetServerState().Transform.GetLocation();
	const FQuat TargetRotation = GetServerState().Transform.GetRotation();
	const FVector StartLocation = ClientStartTransform.GetLocation();
	const FQuat StartRotation = ClientStartTransform.GetRotation();
	const float VelocityToDerivative = ClientTimeBtwnLastUpdates * 100;  // convert m to cm
	const FVector StartDerivative = ClientStartVelocity;
	const FVector TargetDerivative = ServerState.Velocity * VelocityToDerivative;
	GetOwner()->SetActorLocation(FMath::CubicInterp(
		StartLocation,StartDerivative, TargetLocation, TargetDerivative, LerpRatio));
	const FVector NewDerivative = FMath::CubicInterpDerivative(
		StartLocation,StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	FVector NewVelocity = NewDerivative / VelocityToDerivative;
	
	GetOwner()->SetActorRotation(FQuat::Slerp(StartRotation, TargetRotation, LerpRatio));
}

TArray<FGoKartMove> UGoKartMovementReplicator::GetUnacknowledgedMoves() const
{
	return UnacknowledgedMoves;
}

void UGoKartMovementReplicator::AddUnacknowledgedMove(const FGoKartMove& Move)
{
	this->UnacknowledgedMoves.Add(Move);
}

void UGoKartMovementReplicator::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME( UGoKartMovementReplicator, ServerState );
}

void UGoKartMovementReplicator::ClearAcknowledgedMoves( FGoKartMove LastMove)
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

void UGoKartMovementReplicator::OnRep_ServerState()
{
	switch (GetOwnerRole())
	{
		case ROLE_AutonomousProxy:
			AutonomousProxy_OnRep_ServerState();
			break;
		case ROLE_SimulatedProxy:
			SimulatedProxy_OnRep_ServerState();
			break;
		default:
			break;
	}
}

void UGoKartMovementReplicator::SimulatedProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;
	ClientTimeBtwnLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;

	ClientStartTransform = GetOwner()->GetActorTransform();
	ClientStartVelocity = MovementComponent->GetVelocity();

}

void UGoKartMovementReplicator::AutonomousProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;
	
	UE_LOG(LogTemp, Warning, TEXT("Replicated Actor Transform"))
	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgedMoves(ServerState.LastMove);

	for (const FGoKartMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UGoKartMovementReplicator::Server_SendMove_Implementation(FGoKartMove Move)
{
	if (MovementComponent == nullptr) return;
	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}

bool UGoKartMovementReplicator::Server_SendMove_Validate(FGoKartMove Move)
{
	return true; // TODO: make better validation
}
