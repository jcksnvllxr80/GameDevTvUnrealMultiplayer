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

struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(const float LerpRatio) const
	{
		return FMath::CubicInterp(StartLocation,StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
	FVector InterpolateDerivative(const float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation,StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementReplicator final : public UActorComponent
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
	TArray<FGoKartMove> GetUnacknowledgedMoves() const;
	void AddUnacknowledgedMove(const FGoKartMove& Move);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(ReplicatedUsing=OnRep_ServerState)
	FGoKartState ServerState;
	TArray<FGoKartMove> UnacknowledgedMoves;
	float ClientTimeSinceUpdate;
	float ClientTimeBtwnLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;
	
	UPROPERTY()
	UGoKartMovementComponent* MovementComponent;

	UFUNCTION()
	void OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	void ClearAcknowledgedMoves(FGoKartMove LastMove);
	void SetServerState(const FGoKartState& ServerState);
	void UpdateServerState(const FGoKartMove& Move);
	FHermiteCubicSpline CreateSpline(float& LerpRatio, float& VelocityToDerivative) const;
	void InterpolateLocation(float LerpRatio, FHermiteCubicSpline Spline) const;
	void InterpolateVelocity(float LerpRatio, FHermiteCubicSpline Spline, float VelocityToDerivative) const;
	void InterpolateRotation(float LerpRatio) const;
	void ClientTick(float DeltaTime);
	
	UPROPERTY()
    USceneComponent* MeshOffsetRoot;
	UFUNCTION(BlueprintCallable)
	void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }
};
