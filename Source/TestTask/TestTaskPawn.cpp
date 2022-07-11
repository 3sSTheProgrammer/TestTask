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
	AdditionalVelocity = 0.0f;
	IsUsingAbility = false;
		
	CurrentRotationVelocity = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ATestTaskPawn::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetActorLocation();

	// find static mesh component
	UStaticMeshComponent* StaticMeshComponent;
	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents(StaticMeshComponents);

	if (StaticMeshComponents.Num() > 0)
	{
		StaticMeshComponent = StaticMeshComponents[0];

		// set up delegate for collisions with something else
		StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ATestTaskPawn::OnOverlapBegin);
		StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATestTaskPawn::OnHit);
	}
}

void ATestTaskPawn::ResetToStartState()
{
    
	SetActorLocation(StartingLocation);
	SetActorRotation(FRotator::ZeroRotator);
	AdditionalVelocity = 0;
	IsVelocityIncreasing = false;
}

void ATestTaskPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Began overlap"));
	if (OtherActor->ActorHasTag("Wall"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit the wall"));
	}
}

void ATestTaskPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->ActorHasTag("Wall"))
	{
		FRotator CurrentRotation = GetActorRotation();
		float YawDifference = CurrentRotation.Yaw - OtherActor->GetActorRotation().Yaw;
		CurrentRotation.Yaw += 180 - 2 * YawDifference;		
		SetActorRotation(CurrentRotation);
	}
}

TArray<ATestTaskEnemyActor*> ATestTaskPawn::GetEnemiesInRange(float Range)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestTaskEnemyActor::StaticClass(), FoundActors);

	TArray<ATestTaskEnemyActor*> EnemiesInRange;
	for (AActor* Actor : FoundActors)
	{
		if (ATestTaskEnemyActor* EnemyActor = Cast<ATestTaskEnemyActor>(Actor))
		{
			float Distance = FVector::Dist(GetActorLocation(), EnemyActor->GetActorLocation());
			if (Distance < Range)
			{
				EnemiesInRange.Add(EnemyActor);
			}
		}
	}

	return EnemiesInRange;
}

void ATestTaskPawn::DestroyNextEnemy()
{
	
	if (EnemiesImpactedByAbility.Num() > 0)
	{
		//Choose enemy to destroy
		int32 MinAngleEnemyIndex = 0;
		if (EnemiesImpactedByAbility.Num() > 1)
		{
			float MinAngleToEnemy = CalculateAngleToEnemy(EnemiesImpactedByAbility[0]->GetActorLocation());
			
			//UE_LOG(LogTemp, Warning, TEXT("found enemy at %f degrees"), AngleToEnemy);
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
		
		DrawDebugLine(GetWorld(), GetActorLocation(), EnemiesImpactedByAbility[MinAngleEnemyIndex]->GetActorLocation(),
			FColor::Red, false, EnemyKillInterval, 0, 3);
		
		EnemiesImpactedByAbility[MinAngleEnemyIndex]->Destroy();
		EnemiesImpactedByAbility.RemoveAt(MinAngleEnemyIndex);
		UE_LOG(LogTemp, Warning, TEXT("%d enemies remain"), EnemiesImpactedByAbility.Num());
	}
	
	if (EnemiesImpactedByAbility.Num() > 0)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATestTaskPawn::DestroyNextEnemy,
			EnemyKillInterval, false);
	}
	else
	{
		IsUsingAbility = false;
	}	
}

float ATestTaskPawn::CalculateAngleToEnemy(FVector EnemyCoordinates)
{
	FVector ForwardVector = GetActorForwardVector();
	FVector VectorToEnemy = EnemyCoordinates - GetActorLocation();
	VectorToEnemy.Normalize();
	float AngleToEnemy = 180 / 3.1415 * FMath::Acos( FVector::DotProduct(VectorToEnemy, ForwardVector));

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
	if (CurrentLocation.Z < KillZ)
	{
		ResetToStartState();
	}

	if (!IsUsingAbility)
	{
		// If ChangeDirection input is not zero
		if (CurrentRotationVelocity.Yaw != 0)
		{
			//Rotate
			FRotator CurrentRotation = GetActorRotation();
			CurrentRotation.Yaw += CurrentRotationVelocity.Yaw * RotationSpeedPerSecond * DeltaTime;
			//UE_LOG(LogTemp, Warning, TEXT("%f %f"), CurrentRotation.Roll, CurrentRotation.Pitch);
			SetActorRotation(CurrentRotation);
			CurrentRotationVelocity = FRotator::ZeroRotator;
		
		}
	
		//if player holds increase velocity button
		if (IsVelocityIncreasing)
		{
			//increase starting velocity
			AdditionalVelocity += VelocityIncreasePerSecond * DeltaTime;
		}
		else if (AdditionalVelocity > 0)
		{
			//Move
			float CoordinateChange = AdditionalVelocity * DeltaTime;
			FRotator CurrentRotation = GetActorRotation();
			CurrentLocation.X += CoordinateChange * FMath::Cos(CurrentRotation.Yaw * 3.1415 / 180);
			CurrentLocation.Y += CoordinateChange * FMath::Sin(CurrentRotation.Yaw * 3.1415 / 180);
			SetActorLocation(CurrentLocation);

			//Decrease speed
			AdditionalVelocity -= VelocityDecreasePerSecond * DeltaTime;
			if (AdditionalVelocity < 0)
			{
				AdditionalVelocity = 0;
			}
	
		}
	}
	
}

void ATestTaskPawn::IncreaseAdditionalVelocity()
{
	//UE_LOG(LogTemp, Warning, TEXT("Increase additional velocity"));
	AdditionalVelocity = 0.0f;
	IsVelocityIncreasing = true;
}

void ATestTaskPawn::AddVelocityImpulse()
{
	IsVelocityIncreasing = false;
	//UE_LOG(LogTemp, Warning, TEXT("Add velocity impulse"));
}

void ATestTaskPawn::ChangeDirection(float Input)
{
	if (!IsVelocityIncreasing)
	{
		IsVelocityIncreasing = false;
	}
	AdditionalVelocity = 0.0f;
	CurrentRotationVelocity.Yaw = Input;
}

void ATestTaskPawn::UseAbility()
{
	if (IsUsingAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("Already using ability"));
		return;
	}
	
	IsUsingAbility = true;
	AdditionalVelocity = 0;
	
	UE_LOG(LogTemp, Warning, TEXT("Using ability"));
	EnemiesImpactedByAbility = GetEnemiesInRange(AbilityRange);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATestTaskPawn::DestroyNextEnemy, EnemyKillInterval, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Found %d enemies"), EnemiesImpactedByAbility.Num());
	DrawDebugCircle(GetWorld(), GetActorLocation(), AbilityRange, 100,
		FColor::Red, false, EnemiesImpactedByAbility.Num() * EnemyKillInterval, 0, 3,FVector(1, 0, 0),
		FVector(0, 1, 0), true);

	
	
}

