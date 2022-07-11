// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExitActor.h"

#include "TestTaskPawn.h"
#include "Components/BoxComponent.h"

// Sets default values
ALevelExitActor::ALevelExitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ALevelExitActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxComponents;
	GetComponents(BoxComponents);

	if (BoxComponents.Num() > 0)
	{
		BoxComponents[0]->OnComponentBeginOverlap.AddDynamic(this, &ALevelExitActor::OnOverlapBegin);
	}
}

void ALevelExitActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATestTaskPawn* PlayerActor = Cast<ATestTaskPawn>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player entered level end zone"));
	}
}


