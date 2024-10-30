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


	if (0)
	{
		if (
			(use_actorfornode && use_instance_static_mesh)
			||
			(use_text_render_components_directly_on_this_actor && use_instance_static_mesh)
			||
			(use_actorfornode && use_text_render_components_directly_on_this_actor)

		)
		{
			ll("You can only choose one to be true in these three boolean values. ", 1, 2);
			prechecksucceeded = false;
			qq();
			return;
		}
	}


	if (
		!use_shaders && !use_actorfornode
	)
	{
		ll("If CPU we must use actor for node for right now. ", 1, 2);
		prechecksucceeded = false;
		qq();
		return;
	}
	if (use_instance_static_mesh)
	{
		check(InstancedStaticMeshComponent);
	}
	
	
	if (use_tick_interval)
	{
		UE_LOG(LogTemp, Warning, TEXT("Restricting tick interval"));

		PrimaryActorTick.TickInterval = tick_interval;
	}
	
	// generateGraph();
	timeThisMemberFunction(
		"AKnowledgeGraph::generateGraph",
		&AKnowledgeGraph::generateGraph
		);
	
	if (use_shaders)
	{
		SimParameters.Bodies.SetNumUninitialized(
			jnodes1
		);
		BodyTransforms.SetNumUninitialized(
			jnodes1);
	}
	
	timeThisMemberFunction(
		"AKnowledgeGraph::initializeNodePosition",
		&AKnowledgeGraph::initializeNodePosition);

	if(use_instance_static_mesh)
	{
		InstancedStaticMeshComponent->AddInstances(BodyTransforms, false);
	}
	
	if (use_shaders)
	{
		// In a new commits because we are no longer wrapping the simulation in Fix containing Cube container.
		// the following two lines is useless. 
		SimParameters.ViewportWidth = 8000.0;
		SimParameters.CameraAspectRatio =1.777778;
		SimParameters.GravityConstant = 1000.0;


		SimParameters.NumBodies = jnodes1;
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);

	}


	if (!use_shaders)
	{
		update_Node_world_position_according_to_position_array();
		
		timeThisMemberFunction(
			"AKnowledgeGraph::CalculateBiasstrengthOflinks",
			&AKnowledgeGraph::CalculateBiasstrengthOflinks);
	}
}


void AKnowledgeGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!prechecksucceeded)
	{
		ll("prechecksucceeded is false", true, 2);
		qq();
		return;
	}

	
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
