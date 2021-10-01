// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

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
	FVector GetResistance();
	// Mass of the car in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// force applied to the car when the throttle is fully done in N
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;
	UPROPERTY(EditAnywhere)
	float MaxRotDegPerSec = 90;
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16; // kg/m
	float Throttle;
	float SteeringThrow;
	
	FVector Velocity;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime);
};
