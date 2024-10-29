// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeGraph.h"

#include <random>

// #define ENABLE_LOGGING 1
#include "utillllllssss.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)

AKnowledgeGraph::~AKnowledgeGraph()
{
	ll("AKnowledgeGraph::~AKnowledgeGraph", true, 2);
}


AKnowledgeGraph::AKnowledgeGraph()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;


	if (use_shaders)
	{
		PrimaryActorTick.bStartWithTickEnabled = true;
		PrimaryActorTick.TickGroup = TG_DuringPhysics;


		if (1)
		{
			InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
				TEXT("InstancedStaticMeshComponent"));
		}
	}
}

void AKnowledgeGraph::BeginDestroy()
{
	ll("AKnowledgeGraph::BeginDestroy", true, 2);
	if (use_shaders)
	{
		FNBodySimModule::Get().EndRendering();
	}

	Super::BeginDestroy();
}

void AKnowledgeGraph::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ll("AKnowledgeGraph::EndPlay", true, 2);
	ll("EndPlayReason: " + FString::FromInt((int)EndPlayReason), true, 2);
	// if (use_shaders)
	// {
	// 	FNBodySimModule::Get().EndRendering();
	// }
	Super::EndPlay(EndPlayReason);
}

void AKnowledgeGraph::BeginPlay()
{
	Super::BeginPlay();

	ClearLogFile();


	if (use_tick_interval)
	{
		UE_LOG(LogTemp, Warning, TEXT("Restricting tick interval"));

		PrimaryActorTick.TickInterval = tick_interval;
	}

	check(SimulationConfig);

	if (use_instance_static_mesh)
	{
		check(InstancedStaticMeshComponent);
	}




	if (use_shaders)
	{
		if (use_text_render_component)
		{
			for (int32 i = 0; i < SimulationConfig->NumberOfBody; i++)
			{
				UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(
					this, FName("TextComponent" + FString::FromInt(i))
					);
				if (TextComponent)
				{
					TextComponent->SetText(FText::FromString("Sample Text : " + FString::FromInt(i)));
					TextComponent->SetupAttachment(RootComponent);
					TextComponent->RegisterComponent(); // This is important to initialize the component



					TextComponents11111111111111111111.Add(TextComponent);
					// Assuming TextComponents is a valid TArray<UTextRenderComponent*>
				}
			}
		}


		if (!SimulationConfig)
		{
			ll("Failed to start simulation : SimulationConfig data asset has not been assigned in simulation engine.",
			   true, 2);

			qq();
			return;
		}


		// In a new commits because we are no longer wrapping the simulation in Fix containing Cube container.
		// the following two lines is useless. 
		SimParameters.ViewportWidth = SimulationConfig->CameraOrthoWidth;
		SimParameters.CameraAspectRatio = SimulationConfig->CameraAspectRatio;
		//
		InitBodies(); // In this function, we also set a lot of things in SimParameters
		//
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);

	}


	if (!use_shaders)
	{
		// generateGraph();
		timeThisMemberFunction(
			"AKnowledgeGraph::generateGraph",
			&AKnowledgeGraph::generateGraph);



		timeThisMemberFunction(
			"AKnowledgeGraph::initializeNodePosition",
			&AKnowledgeGraph::initializeNodePosition);


		update_Node_world_position_according_to_position_array();


		timeThisMemberFunction(
			"AKnowledgeGraph::CalculateBiasstrengthOflinks",
			&AKnowledgeGraph::CalculateBiasstrengthOflinks);
	}
}


void AKnowledgeGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool log = true;


	iterations += 1;

	ll("TICK----------------------------------------------------------------------------"
	   "----------------------------------------------------------------------------", log);

	ll("iterations: " + FString::FromInt(iterations), log);


	if (iterations > maxiterations)
	{
		ll("iterations is greater than maxiterations", log);
		// qq();
		return;
	}


	ll("alpha: " + FString::SanitizeFloat(alpha), log);

	if (alpha < alphaMin)
	{
		ll("alpha is less than alphaMin", log);
		// UE_LOG(LogTemp, Warning, TEXT("alpha is less than alphaMin"));
		// qq();
		return;

	}

	alpha += (alphaTarget - alpha) * alphaDecay; //need to restart this if want to keep moving
	ll("alpha: " + FString::SanitizeFloat(alpha), log);




	
	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");
	if (use_shaders)
	{
		if (use_constant_delta_time < 0)
		{
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, 1);
			UpdateBodiesPosition(DeltaTime);
		}
		else
		{
			float DeltaTime = use_constant_delta_time;
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime, 1);
			UpdateBodiesPosition(DeltaTime);
		}
	}

	if (!use_shaders)
	{
		double StartTime = FPlatformTime::Seconds();



		ll("apply forces", log);
		ApplyForces();

		ll("update actor location based on velocity", log);
		update_position_array_according_to_velocity_array();


		ll("Logging out The position of the first node: " + nodePositions[0].ToString(), log);


		update_Node_world_position_according_to_position_array();


		ll("update link position", log);
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
