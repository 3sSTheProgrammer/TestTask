// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskPlayerController.h"

#include "TestTaskPawn.h"
#include "TestTaskUserInterface.h"
#include "Blueprint/UserWidget.h"

void ATestTaskPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Find and save player pawn
	if (ATestTaskPawn* TaskPawn = Cast<ATestTaskPawn>(GetPawn()))
	{
		TestTaskPawn = TaskPawn;
	}

	// Create player interface
	UTestTaskUserInterface* PlayerInterface = CreateWidget<UTestTaskUserInterface>(this, PlayerInterfaceClass);
	if (PlayerInterface)
	{
		PlayerInterface->AddToViewport();
	}
}

void ATestTaskPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent != nullptr);

	InputComponent->BindAction("ChangeVelocity", EInputEvent::IE_Pressed, 
		this, &ATestTaskPlayerController::IncreaseVelocity);
	InputComponent->BindAction("ChangeVelocity", EInputEvent::IE_Released,
		this, &ATestTaskPlayerController::AddVelocityImpulse);

	InputComponent->BindAction("UseAbility", EInputEvent::IE_Pressed,
		this, &ATestTaskPlayerController::UseAbility);

	InputComponent->BindAxis("ChangeDirection", this, &ATestTaskPlayerController::RotatePawn);
}

// Used to process rotation input
void ATestTaskPlayerController::RotatePawn(float Input)
{
	if (Input != 0)
	{
		if (TestTaskPawn)
		{
			TestTaskPawn->ChangeDirection(Input);
		}
	}
}

// Used to process increasing velocity input
void ATestTaskPlayerController::IncreaseVelocity()
{
	if (TestTaskPawn)
	{
		TestTaskPawn->IncreaseVelocity();
	}
}

// Used to process add impulse input
void ATestTaskPlayerController::AddVelocityImpulse()
{
	if (TestTaskPawn)
	{
		TestTaskPawn->AddVelocityImpulse();
	}
}

// Used to process use ability input
void ATestTaskPlayerController::UseAbility()
{
	if (TestTaskPawn)
	{
		TestTaskPawn->UseAbility();
	}
}
