// Fill out your copyright notice in the Description page of Project Settings.


#include "KnowledgeGraph.h"

#include <random>

// #define ENABLE_LOGGING 1
#include "utillllllssss.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)


AKnowledgeGraph::AKnowledgeGraph()
{
	PrimaryActorTick.bCanEverTick = true;


	if (use_shaders)
	{
		PrimaryActorTick.bStartWithTickEnabled = true;
		PrimaryActorTick.TickGroup = TG_DuringPhysics;


		if(use_instance_static_mesh)
		{
			InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(
				TEXT("InstancedStaticMeshComponent"));
		}

	}

}

AKnowledgeGraph::~AKnowledgeGraph()
{
	UE_LOG(LogTemp, Warning, TEXT("DESTRUCTOR CALLED!@!!!!!!!!!!!!!!!!!!!!!!!!!!"));
}







void AKnowledgeGraph::BeginDestroy()
{
	if (use_shaders)
	{
		FNBodySimModule::Get().EndRendering();
		
	}

	Super::BeginDestroy();
}

void AKnowledgeGraph::BeginPlay()
{
	Super::BeginPlay();

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
				UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this, FName("TextComponent" + FString::FromInt(i)));
				if (TextComponent)
				{
					TextComponent->SetText(FText::FromString("Sample Text : " + FString::FromInt(i)));
					TextComponent->SetupAttachment(RootComponent);
					TextComponent->RegisterComponent();  // This is important to initialize the component
					TextComponents11111111111111111111.Add(TextComponent);  // Assuming TextComponents is a valid TArray<UTextRenderComponent*>
				}
			}
		}

		
		if (!SimulationConfig)
		{
			ll("Failed to start simulation : SimulationConfig data asset has not been assigned in simulation engine.",true,2);

			qq();
			return;
		}

		
		// In a new commits because we are no longer wrapping the simulation in Fix containing Cube container.
		// the following two lines is useless. 
		SimParameters.ViewportWidth = SimulationConfig->CameraOrthoWidth;
		SimParameters.CameraAspectRatio = SimulationConfig->CameraAspectRatio;
		//
		InitBodies();   // In this function, we also set a lot of things in SimParameters
		//
		FNBodySimModule::Get().BeginRendering();
		FNBodySimModule::Get().InitWithParameters(SimParameters);

		if (0)
		{
			// Params struct used to pass args to our compute shader
			// FnameOfTheShader9DispatchParams Params(1, 1, 1);

			// Fill in your input parameters here
			// Params.X = 123;

			// These are defined/used in:
			// 1. Private/nameOfTheShader9/nameOfTheShader9.cpp under BEGIN_SHADER_PARAMETER_STRUCT
			// 2. Public/nameOfTheShader9/nameOfTheShader9.h under FnameOfTheShader9DispatchParams
			// 3. Private/nameOfTheShader9/nameOfTheShader9.cpp under FnameOfTheShader9Interface::DispatchRenderThread

			// Executes the compute shader and blocks until complete. You can place outputs in the params struct
			// FnameOfTheShader9Interface::Dispatch(Params);
		}
	}





	
	if (!use_shaders)
	{
		ClearLogFile();


		
	
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


	iterations += 1;


	if (iterations > maxiterations)
	{
		return;
	}
	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White, "TICK");
	if (use_shaders)
	{
		if (use_constant_delta_time<0)
		{
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime,1);
			UpdateBodiesPosition(DeltaTime);
		}
		else
		{
			float DeltaTime = use_constant_delta_time;
			SimParameters.DeltaTime = DeltaTime;
			FNBodySimModule::Get().UpdateDeltaTime(DeltaTime,1);
			UpdateBodiesPosition(DeltaTime);
		}
	}

	if (!use_shaders)
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
	
}
