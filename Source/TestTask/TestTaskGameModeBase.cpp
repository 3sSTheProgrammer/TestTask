// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskGameModeBase.h"

ATestTaskGameModeBase::ATestTaskGameModeBase()
{
	// set pawn and player controller
	ConstructorHelpers::FObjectFinder<UClass>
		PawnClass(TEXT("Class'/Game/Blueprints/BP_TestTaskPawn.BP_TestTaskPawn_C'"));
	if (PawnClass.Object != nullptr)
	{
		DefaultPawnClass = PawnClass.Object;
	}

	ConstructorHelpers::FObjectFinder<UClass>
		ControllerClass(TEXT("Class'/Game/Blueprints/BP_TestTaskPlayerController.BP_TestTaskPlayerController_C'"));
	if (ControllerClass.Object != nullptr)
	{
		PlayerControllerClass = ControllerClass.Object;
	}
}
