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
#include "Components/InstancedStaticMeshComponent.h"

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool use_parallel = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float LOGGGGGGGGG = false;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float alpha = 1;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float iterations = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int32 maxiterations = 1000000;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	// Use extremely small tick interval
	bool use_tick_interval = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float tick_interval = 0.9f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int jnodes1 = 50;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	int biasinitway = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	bool connect_to_previous = true;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Attributes)
	float initialRadius = 10;

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


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
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


	bool init = false;

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



	// Testing new shader. /////////////////////////////////////////////////////////////
	FNBodySimParameters SimParameters;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Simulation")
	TObjectPtr<USimulationConfig> SimulationConfig;

	/** Store the transform of all body of the simulation. */
	UPROPERTY()
	TArray<FTransform> BodyTransforms;

	UPROPERTY(VisibleAnywhere, Instanced)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedStaticMeshComponent;

	void InitBodies();

	///////////////////////////////////////////////////////////////////////////////////////
};











// Testing new shader. /////////////////////////////////////////////////////////////

USTRUCT(BlueprintType)
struct FBodyConfigEntry
{
	GENERATED_BODY()

	/** The mass of this body. */
	UPROPERTY(EditAnywhere)
	float Mass;

	/** The spawn 2D position of this body in the simulation's screen. */
	UPROPERTY(EditAnywhere)
	FVector2f SpawnPosition;

	/** The spawn 2D velocity of this body in the simulation's screen. */
	UPROPERTY(EditAnywhere)
	FVector2f SpawnVelocity;
};





/**
 *	Asset file containing the configuration data for the simulation.
 */
UCLASS(BlueprintType)
// UCLASS()

class UE544_BLANKKKKKKKKKK_API USimulationConfig : public UObject
{
	GENERATED_BODY()

public:
	/** The number of bodies to handle in the simulation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Body")
	int32 NumberOfBody = 100.0f;

	/** Bodies will be spawned with a random mass included within this range. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Body")
	FVector2f InitialBodyMassRange = FVector2f(20.0f, 50.0f);
	
	/** Radius of the circle in which bodies will be spawn randomly. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Body")
	float BodySpawnCircleRadius = 1000.0f;

	/** Bodies will be spawned with a random velocity included within this range. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Body")
	FVector2f BodySpawnVelocityRange = FVector2f(400.0f, 600.0f);

	/** You can add body manually with custom setup. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Body")
	TArray<FBodyConfigEntry> CustomBodies;


	
	/** The gravitational constant value. Cannot be less than 1.0 to avoid diving by zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WorldSettings", meta = (ClampMin = 1.0f))
	float GravitationalConstant = 1000.0f;

	
	
	/** Scale rate of the rendered bodies' static mesh knowing that mesh already scale depending on the body's mass. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rendering")
	float MeshScaling = 0.2f;

	/** The orthogonal camera's width configuration to wrap bodies when going out of screen. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rendering")
	float CameraOrthoWidth = 8000.0f;

	/** The main camera's aspect ratio. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Rendering")
	float CameraAspectRatio = 1.777778f;
};

///////////////////////////////////////////////////////////////////////////////////////