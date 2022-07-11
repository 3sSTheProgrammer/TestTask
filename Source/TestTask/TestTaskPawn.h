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
	
	float AdditionalVelocity;

	bool IsVelocityIncreasing;

	FRotator CurrentRotationVelocity;

	FTransform StartingTransform;
	FVector StartingLocation;

	bool IsUsingAbility;

	TArray<ATestTaskEnemyActor*> EnemiesImpactedByAbility;
public:
	UPROPERTY(EditAnywhere)
	float RotationSpeedPerSecond { 45 };
	
	UPROPERTY(EditAnywhere)
	float VelocityIncreasePerSecond{ 200 };

	UPROPERTY(EditAnywhere)
	float VelocityDecreasePerSecond{ 100 };

	UPROPERTY(EditAnywhere)
	float KillZ{ -30 };

	UPROPERTY(EditAnywhere)
	float AbilityRange{ 600 };

	UPROPERTY(EditAnywhere)
	float EnemyKillInterval{ 0.1f };
	
public:
	// Sets default values for this pawn's properties
	ATestTaskPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ResetToStartState();

	// Function that describes behaviour when overlap starts
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
			class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	TArray<ATestTaskEnemyActor*> GetEnemiesInRange(float Range);

	void DestroyNextEnemy();

	float CalculateAngleToEnemy(FVector EnemyCoordinates);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void IncreaseAdditionalVelocity();

	void AddVelocityImpulse();

	void ChangeDirection(float Input);

	void UseAbility();
};
