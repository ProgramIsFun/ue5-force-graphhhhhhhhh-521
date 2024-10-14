// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeGraph.h"

#include <random>

// #define ENABLE_LOGGING 1
#include "utillllllssss.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)


AKnowledgeGraph::AKnowledgeGraph()
{
	PrimaryActorTick.bCanEverTick = true;
	if (0)
	{
		alphaMin = 0.6f;
	}
	else
	{
	}
}

AKnowledgeGraph::~AKnowledgeGraph()
{
	UE_LOG(LogTemp, Warning, TEXT("DESTRUCTOR CALLED!@!!!!!!!!!!!!!!!!!!!!!!!!!!"));
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

	// generateGraph();
	timeThisMemberFunction(
"AKnowledgeGraph::generateGraph",
	&AKnowledgeGraph::generateGraph);


	if (!init)
	{
		
		timeThisMemberFunction(
"AKnowledgeGraph::initializeNodePosition",
		&AKnowledgeGraph::initializeNodePosition);


		update_node_position_according_to_array();


		
		timeThisMemberFunction(
"AKnowledgeGraph::CalculateBiasstrengthOflinks",
		&AKnowledgeGraph::CalculateBiasstrengthOflinks);
	}

}



void AKnowledgeGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bool log = true;

	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");


	iterations += 1;


	if (iterations > maxiterations)
	{
		return;
	}


	if (1)
	{

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
		update_actor_location_based_on_velocity();
		
		ll("update link position",log);
		update_link_position();


		
		

		
		// Optionally, log the average time every N ticks
		if (1)
		{
			double EndTime = FPlatformTime::Seconds();
			double ElapsedTime = EndTime - StartTime;
			lll(FString::SanitizeFloat(ElapsedTime));

			ElapsedTimes.Add(ElapsedTime);

		}

	}
	else
	{
	}
}
