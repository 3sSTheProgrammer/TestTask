// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TestTaskPawn.generated.h"

class ATestTaskEnemyActor;

UCLASS()
class TESTTASK_API ATestTaskPawn : public APawn
{
	GENERATED_BODY()

protected:
	// Current velocity value
	float PawnVelocity;

	// A flag that shows if velocity is increasing by player
	bool IsVelocityIncreasing;

	// Player rotation input
	float RotationInput;

	// Starting actor transform for restart
	FTransform StartingTransform;
	
	bool IsUsingAbility;
	
	TArray<ATestTaskEnemyActor*> EnemiesImpactedByAbility;

public:

	// Pawn rotation speed
	UPROPERTY(EditAnywhere)
	float RotationSpeedPerSecond{ 45 };

	// Pawn velocity increase rate
	UPROPERTY(EditAnywhere)
	float VelocityIncreasePerSecond{ 200 };

	// Pawn velocity decrease rate
	UPROPERTY(EditAnywhere)
	float VelocityDecreasePerSecond{ 100 };

	// Height which indicates that pawn has fallen out of map
	UPROPERTY(EditAnywhere)
	float KillZ{ -30 };
	
	UPROPERTY(EditAnywhere)
	float AbilityRange{ 600 };

	// Delay between kills when using ability
	UPROPERTY(EditAnywhere)
	float EnemyKillInterval{ 0.1f };
	
public:
	// Sets default values for this pawn's properties
	ATestTaskPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Puts pawn in the starting state
	void ResetToStartState();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// Returns an array of pointer to enemies, that should be impacted by ability
	TArray<ATestTaskEnemyActor*> GetEnemiesInRange(float Range);

	// Finds and destroys enemy after using ability
	void DestroyNextEnemy();

	// Returns angle between direction to enemy and self forward vector in degrees
	float CalculateAngleToEnemy(FVector EnemyCoordinates);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Used to process increase velocity input
	void IncreaseVelocity();

	// Used to process add impulse input
	void AddVelocityImpulse();

	// Used to process change direction input
	void ChangeDirection(float Input);

	// Used to process use ability input
	void UseAbility();
};
