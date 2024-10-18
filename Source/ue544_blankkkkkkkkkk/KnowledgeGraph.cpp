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
		// SimParameters.ViewportWidth = SimulationConfig->CameraOrthoWidth;
		// SimParameters.CameraAspectRatio = SimulationConfig->CameraAspectRatio;
		//
		// InitBodies();
		//
		// FNBodySimModule::Get().BeginRendering();
		// FNBodySimModule::Get().InitWithParameters(SimParameters);


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





	
	if (1)
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
	else
	{
	}
}
