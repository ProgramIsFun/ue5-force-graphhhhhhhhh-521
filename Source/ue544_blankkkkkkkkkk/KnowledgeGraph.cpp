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


	UE_LOG(LogTemp, Warning, TEXT("Begin play called, Restricting tick interval"));

	if (use_tick_interval)
	{
		PrimaryActorTick.TickInterval = tick_interval;
	}


	if (0)
	{
		// InitOctree(FBox(
		// 		FVector(-200, -200, -200),
		// 		FVector(200, 200, 200)
		// 	)
		// );
	}
	else
	{
	}


	generateGraph();


	if (!init)
	{
		initializeNodePosition();


		CalculateBiasstrengthOflinks();
	}

	ClearLogFile();
}



void AKnowledgeGraph::Tick(float DeltaTime)
{
	bool log = true;
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");
	}
	else
	{
		ll("Engine is not available for some reasonssssssssssssssssssssssss");
		return;
	}


	iterations += 1;


	if (iterations > maxiterations)
	{
		return;
	}


	if (1)
	{
		
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
	}
	else
	{
	}
}
