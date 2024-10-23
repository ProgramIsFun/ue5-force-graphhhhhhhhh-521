#include "KnowledgeGraph.h"

FVector RandPointInCircle3ddd(float CircleRadius)
{
	FVector Point;
	FVector::FReal L;

	do
	{
		// Check random vectors in the unit circle so result is statistically uniform.
		Point.X = FMath::FRand() * 2.f - 1.f;
		Point.Y = FMath::FRand() * 2.f - 1.f;
		Point.Z = FMath::FRand() * 2.f - 1.f;
		L = Point.SizeSquared();
	}
	while (L > 1.0f);

	return Point * CircleRadius;
}

void AKnowledgeGraph::InitBodies()
{
	check(InstancedStaticMeshComponent);
	check(SimulationConfig);


	SimParameters.Bodies.SetNumUninitialized(
		SimulationConfig->NumberOfBody +
		SimulationConfig->CustomBodies.Num()
	);
	BodyTransforms.SetNumUninitialized(
		SimulationConfig->NumberOfBody +
		SimulationConfig->CustomBodies.Num()
	);

	// Initialize the random Bodies with a default random position, velocity and mass depending on config.
	for (int32 Index = 0;
	     Index < SimulationConfig->NumberOfBody;
	     ++Index
	)
	{
		float RandomMass = FMath::FRandRange(
			SimulationConfig->InitialBodyMassRange.X,
			SimulationConfig->InitialBodyMassRange.Y
		);

		FVector2f RandomPosition(
			// RandPointInCircle3ddd
			FMath::RandPointInCircle(
				SimulationConfig->BodySpawnCircleRadius
			)
		);



		
		/**
		 *	For the velocity, we need to have a starting velocity for each body so that they are rotating into the circle clockwise.
		 *	This allow to have a nice starting movement.
		 *	With 0 starting velocity, we kind of have a Supernova.
		 *	RadialSpeedRate, once applied, allow to give bodies less velocity when spawning near center (0,0) and more and more near the edge of the spawning circle.
		 */
		float RadialSpeedRate =
			SimulationConfig->BodySpawnCircleRadius /
			RandomPosition.Size();
		FVector2f RandomVelocity
		{
			FMath::FRandRange(
				SimulationConfig->BodySpawnVelocityRange.X,
				SimulationConfig->BodySpawnVelocityRange.Y
			) /
			RadialSpeedRate,
			0
		};
		/** Trigonometry to rotate velocity in a clockwise movement in the circle. */
		RandomVelocity = RandomVelocity.GetRotated(
			90.0f +
			FMath::RadiansToDegrees(
				FMath::Atan2(
					RandomPosition.Y,
					RandomPosition.X
				)
			)
		);

		float MeshScale = FMath::Sqrt(RandomMass) *
			SimulationConfig->MeshScaling;

		FTransform MeshTransform(
			FRotator(),
			FVector(
				FVector2D(RandomPosition),
				0.0f
				),
			FVector(
				MeshScale,
				MeshScale,
				1.0f
				)
		);

		BodyTransforms[Index] = MeshTransform;
		SimParameters.Bodies[Index] =
			FBodyData(
				RandomMass,
				RandomPosition,
				RandomVelocity
			);
	}

	// Initialize the additional bodies set in the config file.
	for (int32 Index = 0; Index < SimulationConfig->CustomBodies.Num(); ++Index)
	{
		FBodyConfigEntry CustomBodyEntry = SimulationConfig->CustomBodies[Index];

		float MeshScale = FMath::Sqrt(CustomBodyEntry.Mass) * SimulationConfig->MeshScaling;

		FTransform MeshTransform(
			FRotator(),
			FVector(FVector2D(CustomBodyEntry.SpawnPosition), 0.0f),
			FVector(MeshScale, MeshScale, 1.0f)
		);

		int32 BodyIndex =
			SimulationConfig->NumberOfBody +
			Index;
		BodyTransforms[BodyIndex] = MeshTransform;
		SimParameters.Bodies[BodyIndex] = FBodyData(
			CustomBodyEntry.Mass,
			CustomBodyEntry.SpawnPosition,
			CustomBodyEntry.SpawnVelocity
		);
	}

	/** Finally add instances to component to spawn them. */
	InstancedStaticMeshComponent->AddInstances(
		BodyTransforms,
		false
	);


	SimParameters.NumBodies = SimParameters.Bodies.Num();
	SimParameters.GravityConstant = SimulationConfig->GravitationalConstant;
}


void AKnowledgeGraph::UpdateBodiesPosition(float DeltaTime)
{
	bool log = true;
	// Retrieve GPU computed bodies position.
	TArray<FVector2f> GPUOutputPositions = FNBodySimModule::Get().GetComputedPositions();
	ll("GPUOutputPositions.Num(): " + FString::FromInt(GPUOutputPositions.Num()), log);
	if (GPUOutputPositions.Num() != SimParameters.Bodies.Num())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "Size differ for GPU Velocities Ouput buffer and current Bodies instanced mesh buffer. Bodies (%d) Output(%d)"
		       ), SimParameters.Bodies.Num(), GPUOutputPositions.Num());
		return;
	}

	QUICK_SCOPE_CYCLE_COUNTER(STAT_SimulationEngine_UpdateBodiesPosition);

	// Update bodies visual with new positions.
	for (int i = 0; i < SimParameters.Bodies.Num(); i++)
	{
		BodyTransforms[i].SetTranslation(FVector(FVector2D(GPUOutputPositions[i]), 0.0f));
	}
	InstancedStaticMeshComponent->BatchUpdateInstancesTransforms(0, BodyTransforms, false, true);
}
