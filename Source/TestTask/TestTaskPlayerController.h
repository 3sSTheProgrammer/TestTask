// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestTaskPlayerController.generated.h"

class ATestTaskPawn;
/**
 * 
 */
UCLASS()
class TESTTASK_API ATestTaskPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	ATestTaskPawn* TestTaskPawn;
public:
	ATestTaskPlayerController();

protected:

	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	void RotatePawn(float Input);

	void IncreaseAdditionalVelocity();

	void AddVelocityImpulse();

	void UseAbility();
};
