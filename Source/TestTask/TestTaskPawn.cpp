// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskPawn.h"

// Sets default values
ATestTaskPawn::ATestTaskPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsVelocityIncreasing = false;
	AdditionalVelocity = 0.0f;
}

// Called when the game starts or when spawned
void ATestTaskPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATestTaskPawn::ResetToStartState()
{
	SetActorLocation(FVector::ZeroVector);
	SetActorRotation(FRotator::ZeroRotator);
	AdditionalVelocity = 0;
	IsVelocityIncreasing = false;
}

// Called every frame
void ATestTaskPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	if (CurrentLocation.Z < KillZ)
	{
		ResetToStartState();
	}
	//if player holds increase velocity button
	if (IsVelocityIncreasing)
	{
		//increase starting velocity
		AdditionalVelocity += VelocityIncreasePerSecond * DeltaTime;
	}
	else if (AdditionalVelocity > 0)
	{
		//Move
		
		//UE_LOG(LogTemp, Warning, TEXT("Current X coordinate: %f"), CurrentLocation.X);
		float CoordinateChange = AdditionalVelocity * DeltaTime;
		FRotator CurrentRotation = GetActorRotation();
		UE_LOG(LogTemp, Warning, TEXT("Current Yaw: %f"), CurrentRotation.Yaw);
		CurrentLocation.X += CoordinateChange * FMath::Cos(CurrentRotation.Yaw);
		CurrentLocation.Y += CoordinateChange * FMath::Sin(CurrentRotation.Yaw);
		SetActorLocation(CurrentLocation);

		//Decrease speed
		AdditionalVelocity -= VelocityDecreasePerSecond * DeltaTime;
		if (AdditionalVelocity < 0)
		{
			AdditionalVelocity = 0;
		}
	
	}
	//UE_LOG(LogTemp, Warning, TEXT("Additional velocity: %f"), AdditionalVelocity);
}

void ATestTaskPawn::IncreaseAdditionalVelocity()
{
	UE_LOG(LogTemp, Warning, TEXT("Increase additional velocity"));
	AdditionalVelocity = 0.0f;
	IsVelocityIncreasing = true;
}

void ATestTaskPawn::AddVelocityImpulse()
{
	IsVelocityIncreasing = false;
	UE_LOG(LogTemp, Warning, TEXT("Add velocity ipulse"));
}

