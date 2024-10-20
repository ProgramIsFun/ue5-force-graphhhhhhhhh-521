// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeGraph.h"

#include <random>

// #define ENABLE_LOGGING 1
#include "utillllllssss.h"
#include "nameOfTheShader9/nameOfTheShader9.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)


AKnowledgeGraph::AKnowledgeGraph()
{
	PrimaryActorTick.bCanEverTick = true;


	if (1)
	{
		PrimaryActorTick.bStartWithTickEnabled = true;
		PrimaryActorTick.TickGroup = TG_DuringPhysics;

		InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
			TEXT("InstancedStaticMeshComponent"));
	}

}

AKnowledgeGraph::~AKnowledgeGraph()
{
	UE_LOG(LogTemp, Warning, TEXT("DESTRUCTOR CALLED!@!!!!!!!!!!!!!!!!!!!!!!!!!!"));
}




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

		FVector2f RandomPosition(FMath::RandPointInCircle(SimulationConfig->BodySpawnCircleRadius));

		/**
		 *	For the velocity, we need to have a starting velocity for each body so that they are rotating into the circle clockwise.
		 *	This allow to have a nice starting movement.
		 *	With 0 starting velocity, we kind of have a Supernova.
		 *	RadialSpeedRate, once applied, allow to give bodies less velocity when spawning near center (0,0) and more and more near the edge of the spawning circle.
		 */
		float RadialSpeedRate = SimulationConfig->BodySpawnCircleRadius / RandomPosition.Size();
		FVector2f RandomVelocity
		{
			FMath::FRandRange(SimulationConfig->BodySpawnVelocityRange.X, SimulationConfig->BodySpawnVelocityRange.Y) /
			RadialSpeedRate,
			0
		};
		/** Trigonometry to rotate velocity in a clockwise movement in the circle. */
		RandomVelocity = RandomVelocity.GetRotated(
			90.0f + FMath::RadiansToDegrees(FMath::Atan2(RandomPosition.Y, RandomPosition.X)));

		float MeshScale = FMath::Sqrt(RandomMass) * SimulationConfig->MeshScaling;

		FTransform MeshTransform(
			FRotator(),
			FVector(FVector2D(RandomPosition), 0.0f),
			FVector(MeshScale, MeshScale, 1.0f)
		);

		BodyTransforms[Index] = MeshTransform;
		SimParameters.Bodies[Index] = FBodyData(RandomMass, RandomPosition, RandomVelocity);
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

		int32 BodyIndex = SimulationConfig->NumberOfBody + Index;
		BodyTransforms[BodyIndex] = MeshTransform;
		SimParameters.Bodies[BodyIndex] = FBodyData(CustomBodyEntry.Mass, CustomBodyEntry.SpawnPosition,
		                                            CustomBodyEntry.SpawnVelocity);
	}

	/** Finally add instances to component to spawn them. */
	InstancedStaticMeshComponent->AddInstances(BodyTransforms, false);


	SimParameters.NumBodies = SimParameters.Bodies.Num();
	SimParameters.GravityConstant = SimulationConfig->GravitationalConstant;

}





void AKnowledgeGraph::BeginDestroy()
{
	FNBodySimModule::Get().EndRendering();
	Super::BeginDestroy();
}

void AKnowledgeGraph::UpdateBodiesPosition(float DeltaTime)
{
	bool log = true;
	// Retrieve GPU computed bodies position.
	TArray<FVector2f> GPUOutputPositions = FNBodySimModule::Get().GetComputedPositions();
	ll("GPUOutputPositions.Num(): " + FString::FromInt(GPUOutputPositions.Num()),log);
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

void AKnowledgeGraph::BeginPlay()
{
	Super::BeginPlay();


	bool testing_shaders_from_other_repo = true;
	if (testing_shaders_from_other_repo)
	{
		if (!SimulationConfig)
		{
			// UE_LOG(LogNBodySimulation, Error,
			//        TEXT(
			// 	       "Failed to start simulation : SimulationConfig data asset has not been assigned in simulation engine."
			//        ));

			// UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit,
			// 							   false);

			return;
		}

		// Compute static variables.
		SimParameters.ViewportWidth = SimulationConfig->CameraOrthoWidth;
		SimParameters.CameraAspectRatio = SimulationConfig->CameraAspectRatio;
		//
		InitBodies();
		//
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);


		if (0)
		{
			// Params struct used to pass args to our compute shader
			FnameOfTheShader9DispatchParams Params(1, 1, 1);

			// Fill in your input parameters here
			Params.X = 123;

			// These are defined/used in:
			// 1. Private/nameOfTheShader9/nameOfTheShader9.cpp under BEGIN_SHADER_PARAMETER_STRUCT
			// 2. Public/nameOfTheShader9/nameOfTheShader9.h under FnameOfTheShader9DispatchParams
			// 3. Private/nameOfTheShader9/nameOfTheShader9.cpp under FnameOfTheShader9Interface::DispatchRenderThread

			// Executes the compute shader and blocks until complete. You can place outputs in the params struct
			// FnameOfTheShader9Interface::Dispatch(Params);
		}
	}





	
	if (0)
	{
		ClearLogFile();


		if (use_tick_interval)
		{
			UE_LOG(LogTemp, Warning, TEXT("Restricting tick interval"));

			PrimaryActorTick.TickInterval = tick_interval;
		}
	
		// generateGraph();
		timeThisMemberFunction(
			"AKnowledgeGraph::generateGraph",
			&AKnowledgeGraph::generateGraph);


		if (!init)
		{
		
			timeThisMemberFunction(
				"AKnowledgeGraph::initializeNodePosition",
				&AKnowledgeGraph::initializeNodePosition);


			update_Node_world_position_according_to_position_array();


		
			timeThisMemberFunction(
				"AKnowledgeGraph::CalculateBiasstrengthOflinks",
				&AKnowledgeGraph::CalculateBiasstrengthOflinks);
		}

		
	}

}



void AKnowledgeGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool log = true;

	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");
	if (1)
	{
		SimParameters.DeltaTime = DeltaTime;
		FNBodySimModule::Get().UpdateDeltaTime(DeltaTime);

		UpdateBodiesPosition(DeltaTime);
		
	}

	if (0)
	{

		iterations += 1;


		if (iterations > maxiterations)
		{
			return;
		}


		double StartTime = FPlatformTime::Seconds();

		
		
		
		ll("TICK----------------------------------------------------------------------------"
			"----------------------------------------------------------------------------",log);
		ll("alpha: " + FString::SanitizeFloat(alpha),log);
		ll("iterations: " + FString::FromInt(iterations),log);

		if (alpha < alphaMin)
		{
			ll("alpha is less than alphaMin",log);
			// UE_LOG(LogTemp, Warning, TEXT("alpha is less than alphaMin"));
			return;
		}

		alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
		ll("alpha: " + FString::SanitizeFloat(alpha),log);


		ll("apply forces",log);
		ApplyForces();

		ll("update actor location based on velocity",log);
		update_position_array_according_to_velocity_array();


		update_Node_world_position_according_to_position_array();

		
		ll("update link position",log);
		update_link_position();
		
		
		// Optionally, log the average time every N ticks
		if (0)
		{
			double EndTime = FPlatformTime::Seconds();
			double ElapsedTime = EndTime - StartTime;
			lll(FString::SanitizeFloat(ElapsedTime));

			ElapsedTimes.Add(ElapsedTime);

		}
	}
	
}
