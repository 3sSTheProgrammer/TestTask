// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestTaskPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API ATestTaskPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	float AdditionalVelocity;

public:
	ATestTaskPlayerController();

protected:

	virtual void SetupInputComponent() override;

	void RotatePawn(float Input);

	void IncreaseAdditionalVelocity();

	void AddVelocityImpulse();
};
