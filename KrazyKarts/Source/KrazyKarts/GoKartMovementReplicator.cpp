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

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		const FGoKartMove Move = MovementComponent->CreateMove(DeltaTime);
		MovementComponent->SimulateMove(Move);
		AddUnacknowledgedMove(Move);
		Server_SendMove(Move);
	}

	if (GetOwnerRole() == ROLE_Authority && GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		const FGoKartMove Move = MovementComponent->CreateMove(DeltaTime);
		Server_SendMove(Move);
		UE_LOG(LogTemp, Warning, TEXT("Queue length: %d"), GetUnacknowledgedMoves().Num());
		
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(GetServerState().LastMove);
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
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

bool UGoKartMovementReplicator::Server_SendMove_Validate(FGoKartMove Move)
{
	return true; // TODO: make better validation
}


