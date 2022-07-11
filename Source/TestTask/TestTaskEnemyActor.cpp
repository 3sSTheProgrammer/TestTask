// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskEnemyActor.h"


// Sets default values
ATestTaskEnemyActor::ATestTaskEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATestTaskEnemyActor::Die()
{
	if (SpawnBridgeAfterDeath)
	{
		if (BridgeActor)
		{
			GetWorld()->SpawnActor<AActor>(BridgeActor, SpawnBridgeLocation, SpawnBridgeRotation);
		}
	}
	Destroy();
}

// Called when the game starts or when spawned
void ATestTaskEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

