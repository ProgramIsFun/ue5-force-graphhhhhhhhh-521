// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KnowledgeNode.h"
#include "KnowledgeEdge.h"
// #include "GenericOctree333.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "octreeeeeeeeee3.h"
#include "utillllllssss.h"



// Testing new shader. /////////////////////////////////////////////////////////////
#include "NBodySimModule.h"
#include "Config/SimulationConfig.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

///////////////////////////////////////////////////////////////////////////////////////


#include "KnowledgeGraph.generated.h"


/**
 *
 */
// class FORCEGRAPH_API AKnowledgeGraph : public AActor




UCLASS()
class UE544_BLANKKKKKKKKKK_API AKnowledgeGraph : public AActor
{
	GENERATED_BODY()

public:
	AKnowledgeGraph();
	~AKnowledgeGraph();
	void GenerateConnectedGraph(int32 NumClusters, int32 NodesPerCluster);


	void generateGraph();
	void defaultGenerateGraphMethod();


	void AddNode1(int32 id, AKnowledgeNode* kn);
	// void AddNode(int32 id, AKnowledgeNode* kn, FVector location);
	void AddEdge(int32 id, int32 source, int32 target);
	// void AddOctreeElement(const FOctreeElement& inNewOctreeElement);


	void initializeNodePosition();

	void initializeNodePosition_Individual(AKnowledgeNode* node, int index, int NumDimensions, float InitialRadius);

	void CalculateBiasstrengthOflinks();


	void ApplyForces();
	void calculate_link_force_and_update_velocity();
	void calculate_charge_force_and_update_velocity();
	void calculate_centre_force_and_update_position();


	void update_Node_world_position_according_to_position_array();


	void tttttttttttt();


	
	
	// Every node will be Initialize with position 0. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool initialize_with_zero_position = true;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool use_shaders = true;


	// Use instance static mesh or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool use_instance_static_mesh = true;


	// The size of Static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float static_mesh_size = 10;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	UStaticMesh* SelectedMesh1111111111111;

	
	// Use TextRenderComponent or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool use_text_render_components_directly_on_this_actor = true;

	// The size of TextRenderComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float text_size = 10;
	


	
	UPROPERTY(VisibleAnywhere)
	TArray<UTextRenderComponent*> TextComponents11111111111111111111;

	// If want to use constant delta time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float use_constant_delta_time = -1;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool use_parallel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float LOGGGGGGGGG = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int32 maxiterations = 1000000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	// Use extremely small tick interval
	bool use_tick_interval = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float tick_interval = 0.9f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int jnodes1 = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	// Calculate many body force or not. 
	bool manybody = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool many_body_use_brute_force = true;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	// bool many_body_octree_use_recursion = true;	

	// Use a lot of actor Or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool use_a_lot_of_actor = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool connect_to_previous = true;

	

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float alpha = 1;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float iterations = 0;


	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float alphaMin = 0.001;
	// float alphaMin = 0.09;


	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	// float alphaDecay = pow(alphaMin, 0.05);
	float alphaDecay = 1 - FMath::Pow(alphaMin, 1.0 / 300);


	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int32 wayofinitnodeslinks = 2;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float edgeDistance = 30;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float nodeStrength = -60;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float distancemin = 1;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float distancemax = 10000000;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float theta2 = 0.81;


	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int biasinitway = 0;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float initialRadius = 10;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void update_position_array_according_to_velocity_array();
	void update_link_position();


	TMap<int32, AKnowledgeNode*> all_nodes;

	TArray<FVector> nodePositions;
	TArray<FVector> nodeVelocities;


	TMap<int32, AKnowledgeEdge*> all_links;

	// TMap<FString, NodeStrength> octree_node_strengths;
	//    FVector GetWeightedDistance(FVector prev_loc);
	float alphaTarget = 0;
	float velocityDecay = 0.6;
	float initialAngle = PI * (3 - sqrt(5));


	

	// FSimpleOctree* OctreeData;
	OctreeNode* OctreeData2;


	TArray<double> ElapsedTimes;


	virtual void Tick(float DeltaTime) override;


	// Member function that times another member function
	template <typename Func, typename... Args>
	auto timeThisMemberFunction(const char* functionName, Func function, Args&&... args)
	{
		// auto start = std::chrono::high_resolution_clock::now();
		double StartTime = FPlatformTime::Seconds();


		// Invoke the member function
		(this->*function)(std::forward<Args>(args)...);


		// auto end = std::chrono::high_resolution_clock::now();
		// auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		// lll("Execution time: " + FString::FromInt(duration.count()) + " milliseconds");


		double EndTime = FPlatformTime::Seconds();
		double ElapsedTime = EndTime - StartTime;
		lll("Elapsed time For " + FString(functionName) + ": " + FString::SanitizeFloat(ElapsedTime) + " seconds");


		return ElapsedTime;
	}

	void qq()
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
	}

	//////////// Testing new shader. /////////////////////////////////////////////////////////////


	
	FNBodySimParameters SimParameters;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Attributes)
	TObjectPtr<USimulationConfig> SimulationConfig;

	/** Store the transform of all body of the simulation. */
	UPROPERTY()
	TArray<FTransform> BodyTransforms;

	UPROPERTY(VisibleAnywhere, Instanced)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;

	void InitBodies();

	void UpdateBodiesPosition(float DeltaTime);


	///////////////////////////////////////////////////////////////////////////////////////
};










