// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestTaskPawn.generated.h"

UCLASS()
class TESTTASK_API ATestTaskPawn : public APawn
{
	GENERATED_BODY()

protected:
	float AdditionalVelocity;

	bool IsVelocityIncreasing;

public:
	UPROPERTY(EditAnywhere)
	float VelocityIncreasePerSecond{ 200 };

	UPROPERTY(EditAnywhere)
	float VelocityDecreasePerSecond{ 100 };

	UPROPERTY(EditAnywhere)
	float KillZ{ -30 };
public:
	// Sets default values for this pawn's properties
	ATestTaskPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ResetToStartState();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void IncreaseAdditionalVelocity();

	void AddVelocityImpulse();

};
