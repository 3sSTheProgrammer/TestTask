// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTaskEnemyActor.generated.h"

class UTestTaskUserInterface;

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

protected:
	// Reference to player interface, saved for efficiency
	UPROPERTY()
	UTestTaskUserInterface* PlayerInterface;
	
public:	
	// Sets default values for this actor's properties
	ATestTaskEnemyActor();

	// Called to remove self from level
	void Die();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Used to get player interface reference
	void InitPlayerInterface();

};
