// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	float Throttle;
	
	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

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

UCLASS()
class KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGoKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void SimulateMove(const FGoKartMove&);
	
	FVector GetAirResistance() const;
	FVector GetRollingResistance() const;
	// Mass of the car in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;
	// force applied to the car when the throttle is fully done in N
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16; // kg/m
	UPROPERTY(EditAnywhere)
	float TurningRadius = 10; // m

	UPROPERTY(ReplicatedUsing=OnRep_ServerState)
	FGoKartState ServerState;
	
	float Throttle;
	float SteeringThrow;
	FVector Velocity;
	
	UFUNCTION()
	void OnRep_ServerState();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMove(FGoKartMove Value);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteerThrow);
	FGoKartMove CreateMove(float DeltaTime);
	void ClearAcknowledgedMoves(FGoKartMove LastMove);
	
	TArray<FGoKartMove> UnacknowledgedMoves;
};
