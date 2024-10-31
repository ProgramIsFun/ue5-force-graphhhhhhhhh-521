// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActortest77.h"

// Sets default values
AMyActortest77::AMyActortest77()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActortest77::BeginPlay()
{
	Super::BeginPlay();
	
	Location1 = GetActorLocation() + FVector(-100.f, 0.f, 1120.f);
	Location2 = GetActorLocation() + FVector(100.f, 200.f, 0.f);



	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
        
	
		
	
	// Spawning components dynamically every tick
	TempComponent1 = NewObject<UStaticMeshComponent>(this);
	if (TempComponent1)
	{
		TempComponent1->RegisterComponent();
		TempComponent1->SetWorldLocation(Location1);
		TempComponent1->SetStaticMesh(CubeMesh);
	}

	TempComponent2 = NewObject<UStaticMeshComponent>(this);
	if (TempComponent2)
	{
		TempComponent2->RegisterComponent();
		TempComponent2->SetWorldLocation(Location2);
		TempComponent2->SetStaticMesh(CubeMesh);
	}

}

// Called every frame
void AMyActortest77::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();
	if (!World) return;

	

	// Move the component by a little bit every frame
	Location1 += FVector(0.f, 0.f, 1.f);
	Location2 += FVector(0.f, 6.f, 1.f);

	TempComponent1->SetWorldLocation(Location1);
	TempComponent2->SetWorldLocation(Location2);

	
	// Dynamically draw a line between components
	DrawDebugLine(
		World,
		Location1,
		Location2,
		FColor::Red,
		false,
		-1,
		0,
		10.0f
	);

// 	DrawDebugLine(
// 	GetWorld(),
// 	StartLocation,
// 	EndLocation,
// 	FColor::Red,
// 	false,  // Persistent lines set to false
// 	0,      // Duration for zero seconds, meaning one frame
// 	0,      // Depth priority
// 	2.0f    // Thickness
// );





}

