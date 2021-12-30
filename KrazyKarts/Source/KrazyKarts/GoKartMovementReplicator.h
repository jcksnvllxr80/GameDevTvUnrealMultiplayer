// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.h"
#include "GoKartMovementReplicator.generated.h"

USTRUCT()
struct FGoKartState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FTransform Transform;
	
	UPROPERTY()
	FGoKartMove LastMove;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartMovementReplicator();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Value);

	FGoKartState GetServerState() const;
	void SetServerState(const FGoKartState& ServerState);
	TArray<FGoKartMove> GetUnacknowledgedMoves() const;
	void AddUnacknowledgedMove(const FGoKartMove& Move);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_ServerState)
	FGoKartState ServerState;
	TArray<FGoKartMove> UnacknowledgedMoves;
	UPROPERTY()
	UGoKartMovementComponent* MovementComponent;

	UFUNCTION()
	void OnRep_ServerState();
	void ClearAcknowledgedMoves(FGoKartMove LastMove);
};
