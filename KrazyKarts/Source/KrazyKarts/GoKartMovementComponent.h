// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoKartMovementComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class KRAZYKARTS_API UGoKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGoKartMovementComponent();

	FVector GetVelocity() const;
	void SetVelocity(const FVector& Velocity);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SimulateMove(const FGoKartMove&);
	FGoKartMove CreateMove(float DeltaTime) const;
	
	float GetThrottle() const;
	void SetThrottle(float Throttle);
	float GetSteeringThrow() const;
	void SetSteeringThrow(float SteeringThrow);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector GetAirResistance() const;
	FVector GetRollingResistance() const;
	
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteerThrow);

	float Throttle;
	float SteeringThrow;
	FVector Velocity;
	
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
};
