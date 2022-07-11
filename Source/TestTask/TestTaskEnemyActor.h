// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTaskEnemyActor.generated.h"

UCLASS()
class TESTTASK_API ATestTaskEnemyActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Spawn Bridge")
	bool SpawnBridgeAfterDeath{ false };

	UPROPERTY(EditAnywhere, Category = "Spawn Bridge")
	FVector SpawnBridgeLocation;

	UPROPERTY(EditAnywhere, Category = "Spawn Bridge")
	FRotator SpawnBridgeRotation;
	
	UPROPERTY(EditAnywhere, Category = "Spawn Bridge")
	TSubclassOf<AActor> BridgeActor;

public:	
	// Sets default values for this actor's properties
	ATestTaskEnemyActor();

	void Die();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
