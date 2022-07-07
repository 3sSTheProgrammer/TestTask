// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskPlayerController.h"

#include "TestTaskPawn.h"

ATestTaskPlayerController::ATestTaskPlayerController()
{
	AdditionalVelocity = 0.0f;
}

void ATestTaskPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent != nullptr);

	InputComponent->BindAction("ChangeVelocity", EInputEvent::IE_Pressed, 
		this, &ATestTaskPlayerController::IncreaseAdditionalVelocity);
	InputComponent->BindAction("ChangeVelocity", EInputEvent::IE_Released,
		this, &ATestTaskPlayerController::AddVelocityImpulse);
}

void ATestTaskPlayerController::RotatePawn(float Input)
{
}

void ATestTaskPlayerController::IncreaseAdditionalVelocity()
{
	if (ATestTaskPawn* TestTaskPawn = Cast<ATestTaskPawn>(GetPawn()))
	{
		TestTaskPawn->IncreaseAdditionalVelocity();
	}
}

void ATestTaskPlayerController::AddVelocityImpulse()
{
	if (ATestTaskPawn* TestTaskPawn = Cast<ATestTaskPawn>(GetPawn()))
	{
		TestTaskPawn->AddVelocityImpulse();
	}
}
