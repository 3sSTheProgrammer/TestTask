// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelExitActor.h"

#include "TestTaskEnemyActor.h"
#include "TestTaskPawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

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

	//Find Collision Box and set overlap dynamic for it
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
	// If overlapping with player
	if (ATestTaskPawn* PlayerActor = Cast<ATestTaskPawn>(OtherActor))
	{
		//Check if win condition is met
		TArray<AActor*> RemainedEnemies;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestTaskEnemyActor::StaticClass(), RemainedEnemies);
		if (RemainedEnemies.Num() > 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
				FString::Printf(TEXT("%d enemies remain to finish the level"), RemainedEnemies.Num()));
		}
		else
		{
			EndLevel();
		}
		
	}
}

void ALevelExitActor::EndLevel()
{
	//Depending on current level name
	FString CurrentLevelName = GetWorld()->GetMapName();
	if ( CurrentLevelName == "Level1" )
	{
		//Open level2
		UGameplayStatics::OpenLevel(GetWorld(), "Level2");
	}
	else if ( CurrentLevelName == "Level2" )
	{
		//Spawn Congratulations widget
		if (FinishGameWidgetClass)
		{
			UUserWidget* FinishGameWidget = CreateWidget<UUserWidget>(GetWorld(), FinishGameWidgetClass);
			if (FinishGameWidget)
			{
				FinishGameWidget->AddToViewport();
			}
		}
	}
}


