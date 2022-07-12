// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskPlayerController.h"

#include "TestTaskPawn.h"
#include "TestTaskUserInterface.h"
#include "Blueprint/UserWidget.h"

ATestTaskPlayerController::ATestTaskPlayerController()
{

}

void ATestTaskPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ATestTaskPawn* TaskPawn = Cast<ATestTaskPawn>(GetPawn()))
	{
		TestTaskPawn = TaskPawn;
	}

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
		this, &ATestTaskPlayerController::IncreaseAdditionalVelocity);
	InputComponent->BindAction("ChangeVelocity", EInputEvent::IE_Released,
		this, &ATestTaskPlayerController::AddVelocityImpulse);

	InputComponent->BindAction("UseAbility", EInputEvent::IE_Pressed,
		this, &ATestTaskPlayerController::UseAbility);

	InputComponent->BindAxis("ChangeDirection", this, &ATestTaskPlayerController::RotatePawn);
}

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

void ATestTaskPlayerController::IncreaseAdditionalVelocity()
{
	if (TestTaskPawn)
	{
		TestTaskPawn->IncreaseAdditionalVelocity();
	}
}

void ATestTaskPlayerController::AddVelocityImpulse()
{
	if (TestTaskPawn)
	{
		TestTaskPawn->AddVelocityImpulse();
	}
}

void ATestTaskPlayerController::UseAbility()
{
	if (TestTaskPawn)
	{
		TestTaskPawn->UseAbility();
	}
}
