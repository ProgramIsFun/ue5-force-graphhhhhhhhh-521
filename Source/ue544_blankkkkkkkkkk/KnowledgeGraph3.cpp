#include "KnowledgeGraph.h"



void AKnowledgeGraph::InitBodies()
{
	check(InstancedStaticMeshComponent);
	check(SimulationConfig);


	SimParameters.Bodies.SetNumUninitialized(SimulationConfig->NumberOfBody + SimulationConfig->CustomBodies.Num());
	

	BodyTransforms.SetNumUninitialized(SimulationConfig->NumberOfBody + SimulationConfig->CustomBodies.Num());
	
	// Initialize the random Bodies with a default random position, velocity and mass depending on config.
	for (int32 Index = 0; Index < SimulationConfig->NumberOfBody; ++Index)
	{
		float RandomMass = FMath::FRandRange(SimulationConfig->InitialBodyMassRange.X,
		                                     SimulationConfig->InitialBodyMassRange.Y);

		FVector3f RandomPosition;
		if (!initialize_with_zero_position)
		{
			RandomPosition= FVector3f(RandPointInCircle(SimulationConfig->BodySpawnCircleRadius));
		}
		else
		{
			RandomPosition = FVector3f(0,0,0);
		}



		
		/**
		 *	For the velocity, we need to have a starting velocity for each body so that they are rotating into the circle clockwise.
		 *	This allow to have a nice starting movement.
		 *	With 0 starting velocity, we kind of have a Supernova.
		 *	RadialSpeedRate, once applied, allow to give bodies less velocity when spawning near center (0,0) and more and more near the edge of the spawning circle.
		 */
		float RadialSpeedRate = SimulationConfig->BodySpawnCircleRadius / RandomPosition.Size();
		FVector3f RandomVelocity
		{
			0, 0, 0
		};
		/** Trigonometry to rotate velocity in a clockwise movement in the circle. */
		// RandomVelocity = RandomVelocity.GetRotated(90.0f + FMath::RadiansToDegrees(FMath::Atan2(RandomPosition.Y, RandomPosition.X)));

		float MeshScale = FMath::Sqrt(RandomMass) * SimulationConfig->MeshScaling;

		
		
		FTransform MeshTransform(
			FRotator(),
			FVector(RandomPosition),
			FVector(MeshScale, MeshScale, MeshScale)
		);

		BodyTransforms[Index] = MeshTransform;
		SimParameters.Bodies[Index] = FBodyData(RandomMass, RandomPosition, RandomVelocity);
	}

	// Initialize the additional bodies set in the config file.
	// for (int32 Index = 0; Index < SimulationConfig->CustomBodies.Num(); ++Index)
	// {
	// 	FBodyConfigEntry CustomBodyEntry = SimulationConfig->CustomBodies[Index];
	// 	
	// 	float MeshScale = FMath::Sqrt(CustomBodyEntry.Mass) * SimulationConfig->MeshScaling;
	// 	
	// 	FTransform MeshTransform (
	// 	FRotator(),
	// 	FVector(FVector2D(CustomBodyEntry.SpawnPosition), 0.0f),
	// 	FVector(MeshScale, MeshScale, 1.0f)
	// 	);
	//
	// 	int32 BodyIndex = SimulationConfig->NumberOfBody + Index;
	// 	BodyTransforms[BodyIndex] = MeshTransform;
	// 	SimParameters.Bodies[BodyIndex] = FBodyData(CustomBodyEntry.Mass, CustomBodyEntry.SpawnPosition, CustomBodyEntry.SpawnVelocity);
	// }

	/** Finally add instances to component to spawn them. */

	if(use_instance_static_mesh)
	{
		InstancedStaticMeshComponent->AddInstances(BodyTransforms, false);
	}

	SimParameters.NumBodies = SimParameters.Bodies.Num();

	ll("SimParameters.NumBodies: " + FString::FromInt(SimParameters.NumBodies),true,2);
	SimParameters.GravityConstant = SimulationConfig->GravitationalConstant;
}


void AKnowledgeGraph::UpdateBodiesPosition(float DeltaTime)
{
	// Retrieve GPU computed bodies position.
	TArray<FVector3f> GPUOutputPositions = FNBodySimModule::Get().GetComputedPositions();

	if (GPUOutputPositions.Num() != SimParameters.Bodies.Num())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "Size differ for GPU Velocities Ouput buffer and current Bodies instanced mesh buffer. Bodies (%d) Output(%d)"
		       ), SimParameters.Bodies.Num(), GPUOutputPositions.Num());
		qq();
		return;
	}else
	{
		ll("Size is same for GPU Velocities Ouput buffer and current Bodies instanced mesh buffer. Bodies (" + FString::FromInt(SimParameters.Bodies.Num()) + ") Output(" + FString::FromInt(GPUOutputPositions.Num()) + ")",true,2);
		ll("First element position is: " + GPUOutputPositions[0].ToString(),true,2);
	}
	
	// QUICK_SCOPE_CYCLE_COUNTER(STAT_SimulationEngine_UpdateBodiesPosition);

	// Update bodies visual with new positions.
	for (int i = 0; i < SimParameters.Bodies.Num(); i++)
	{
		if (use_instance_static_mesh)
		{
			BodyTransforms[i].SetTranslation(FVector(GPUOutputPositions[i]));
		}


		if (use_text_render_component)
		{
			TextComponents11111111111111111111[i]->SetWorldLocation(FVector(GPUOutputPositions[i]));
		}
	}

	if (use_instance_static_mesh)
	{
		InstancedStaticMeshComponent->BatchUpdateInstancesTransforms(0, BodyTransforms, false, true);
	}
}
