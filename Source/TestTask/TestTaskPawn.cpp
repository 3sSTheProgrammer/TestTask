// Fill out your copyright notice in the Description page of Project Settings.


#include "TestTaskPawn.h"

#include "DrawDebugHelpers.h"
#include "TestTaskEnemyActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATestTaskPawn::ATestTaskPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	IsVelocityIncreasing = false;
	PawnVelocity = 0.0f;
	IsUsingAbility = false;
	RotationInput = 0;
}

// Called when the game starts or when spawned
void ATestTaskPawn::BeginPlay()
{
	Super::BeginPlay();

	// Save pawn starting transform
	StartingTransform = GetActorTransform();
	
	// find static mesh component
	UStaticMeshComponent* StaticMeshComponent;
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents(StaticMeshComponents);

	if (StaticMeshComponents.Num() > 0)
	{
		StaticMeshComponent = StaticMeshComponents[0];

		// set up delegate for collisions with wall
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATestTaskPawn::OnHit);
	}
}

// Puts pawn in the starting state
void ATestTaskPawn::ResetToStartState()
{
	// Set actor to starting place and stop it 
    SetActorTransform(StartingTransform);
	PawnVelocity = 0.f;
	IsVelocityIncreasing = false;
}

void ATestTaskPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	// If hit the wall
	if (OtherActor->ActorHasTag("Wall"))
	{
		// Change current direction depending on self and wall rotations
		FRotator CurrentRotation = GetActorRotation();
		float YawDifference = CurrentRotation.Yaw - OtherActor->GetActorRotation().Yaw;
		CurrentRotation.Yaw += 180 - 2 * YawDifference;		
		SetActorRotation(CurrentRotation);
	}
}

// Returns an array of pointer to enemies, that should be impacted by ability
TArray<ATestTaskEnemyActor*> ATestTaskPawn::GetEnemiesInRange(float Range)
{
	// Find all enemies actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestTaskEnemyActor::StaticClass(), FoundActors);

	TArray<ATestTaskEnemyActor*> EnemiesInRange;

	// For every found actor 
	for (AActor* Actor : FoundActors)
	{
		// If actor is enemy
		if (ATestTaskEnemyActor* EnemyActor = Cast<ATestTaskEnemyActor>(Actor))
		{
			// Check if the enemy is in ability range
			float Distance = FVector::Dist(GetActorLocation(), EnemyActor->GetActorLocation());
			if (Distance < Range)
			{
				// Add enemy to EnemiesInRange
				EnemiesInRange.Add(EnemyActor);
			}
		}
	}

	return EnemiesInRange;
}

// Finds and destroys enemy after using ability
void ATestTaskPawn::DestroyNextEnemy()
{
	// If amount of impacted enemies is not zero
	if (EnemiesImpactedByAbility.Num() > 0)
	{
		//Choose enemy with lowest angle to current direction to destroy
		int32 MinAngleEnemyIndex = 0;
		if (EnemiesImpactedByAbility.Num() > 1)
		{
			float MinAngleToEnemy = CalculateAngleToEnemy(EnemiesImpactedByAbility[0]->GetActorLocation());
			
			for (int i = 1; i < EnemiesImpactedByAbility.Num(); ++i)
			{
				float AngleToEnemy = CalculateAngleToEnemy(EnemiesImpactedByAbility[i]->GetActorLocation());
				if (AngleToEnemy < MinAngleToEnemy)
				{
					MinAngleToEnemy = AngleToEnemy;
					MinAngleEnemyIndex = i;
				}
			}
		}

		// Draw a line to a targeted enemy
		DrawDebugLine(GetWorld(), GetActorLocation(), EnemiesImpactedByAbility[MinAngleEnemyIndex]->GetActorLocation(),
			FColor::Red, false, EnemyKillInterval, 0, 3);

		// Kill the enemy and remove him from impacted enemies array
		EnemiesImpactedByAbility[MinAngleEnemyIndex]->Die();
		EnemiesImpactedByAbility.RemoveAt(MinAngleEnemyIndex);
	}

	// If impacted enemies array is not yet empty
	if (EnemiesImpactedByAbility.Num() > 0)
	{
		// Set timer for killing next enemy
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATestTaskPawn::DestroyNextEnemy,
			EnemyKillInterval, false);
	}
	else
	{
		// Finish using the ability
		IsUsingAbility = false;
	}	
}

// Returns angle between direction to enemy and self forward vector in degrees
float ATestTaskPawn::CalculateAngleToEnemy(FVector EnemyCoordinates)
{
	// Get and normalize forward vector and vector to enemy
	FVector ForwardVector = GetActorForwardVector();
	FVector VectorToEnemy = EnemyCoordinates - GetActorLocation();
	VectorToEnemy.Normalize();

	// Calculate angle between vectors using dot product
	float AngleToEnemy = 180 / 3.1415 * FMath::Acos( FVector::DotProduct(VectorToEnemy, ForwardVector));

	// Use cross product to adjust angles so they are calculated only clockwise
	FVector CrossProduct = FVector::CrossProduct(VectorToEnemy, ForwardVector);
	if (CrossProduct.Z > 0)
	{
		AngleToEnemy = 360 - AngleToEnemy;			
	}
	
	return  AngleToEnemy;
}

// Called every frame
void ATestTaskPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	FVector CurrentLocation = GetActorLocation();

	// If fallen out of map -> move to start
	if (CurrentLocation.Z < KillZ)
	{
		ResetToStartState();
	}

	// If player isn't using ability
	if (!IsUsingAbility)
	{
		// If ChangeDirection input is not zero
		if (RotationInput != 0)
		{
			// Rotate pawn
			FRotator CurrentRotation = GetActorRotation();
			CurrentRotation.Yaw += RotationInput * RotationSpeedPerSecond * DeltaTime;
			SetActorRotation(CurrentRotation);
			RotationInput = 0;
		}
	
		// If player holds increase velocity button
		if (IsVelocityIncreasing)
		{
			//increase starting velocity
			PawnVelocity += VelocityIncreasePerSecond * DeltaTime;
		}
		// otherwise
		else if (PawnVelocity > 0)
		{
			//Move
			float CoordinateChange = PawnVelocity * DeltaTime;
			FRotator CurrentRotation = GetActorRotation();
			CurrentLocation.X += CoordinateChange * FMath::Cos(CurrentRotation.Yaw * 3.1415 / 180);
			CurrentLocation.Y += CoordinateChange * FMath::Sin(CurrentRotation.Yaw * 3.1415 / 180);
			SetActorLocation(CurrentLocation);

			//Decrease speed
			PawnVelocity -= VelocityDecreasePerSecond * DeltaTime;
			if (PawnVelocity < 0)
			{
				PawnVelocity = 0;
			}
	
		}
	}
	
}

// Used to process increase velocity input
void ATestTaskPawn::IncreaseVelocity()
{
	// Stop pawn if moving
	PawnVelocity = 0.0f;

	// Set flag to increase velocity
	IsVelocityIncreasing = true;
}

// Used to process add impulse input
void ATestTaskPawn::AddVelocityImpulse()
{
	// Stop increasing velocity
	IsVelocityIncreasing = false;
}

// Used to process change direction input
void ATestTaskPawn::ChangeDirection(float Input)
{
	// Stop pawn if moving
	PawnVelocity = 0.0f;

	// Set rotation input
	RotationInput = Input;
}

// Used to process use ability input
void ATestTaskPawn::UseAbility()
{
	// Ability cant be used while it still works
	if (IsUsingAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already using ability"));
		return;
	}

	// Setting the flag and stopping the pawn
	IsUsingAbility = true;
	PawnVelocity = 0.f;

	// Get enemies that should be impacted by ability
	EnemiesImpactedByAbility = GetEnemiesInRange(AbilityRange);

	// Set timer to destroy next enemy after given interval
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATestTaskPawn::DestroyNextEnemy, EnemyKillInterval, false);
	
	// Show ability range for the duration of its work
	DrawDebugCircle(GetWorld(), GetActorLocation(), AbilityRange, 100,
		FColor::Red, false, EnemiesImpactedByAbility.Num() * EnemyKillInterval, 0, 3,FVector(1, 0, 0),
		FVector(0, 1, 0), true);
	
}

