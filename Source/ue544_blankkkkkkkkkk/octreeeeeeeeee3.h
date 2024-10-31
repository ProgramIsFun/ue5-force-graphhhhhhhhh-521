#pragma once

#include <functional>

#include "CoreMinimal.h"
// #include "KnowledgeGraph.h"
#include "KnowledgeNode.h"
// #include "PointData.h" // Make sure this include points to your actual PointData structure file location

class Node;

struct PointData
{
	// FVector Position;
	// FVector Velocity;

	// AKnowledgeNode* Node;
	int32 nodeid;
	FVector Position;
	PointData* Next = nullptr;

	PointData(
		// FVector position = FVector(),
		// FVector velocity = FVector()
		// AKnowledgeNode* node = nullptr
	int32 nodeid,
	FVector position
		)
		:
	// Position(position),
	// Velocity(velocity)
	nodeid(nodeid),
	Position(position)
	{
	}

	// Optionally add methods that manipulate the point data if needed
};

struct OctreeNode
{
	bool isCenterSet = false;
	FVector Center;
	FVector Extent;
	TArray<OctreeNode*> Children;
	PointData* Data = nullptr;

	FVector CenterOfMass;
	int32 TotalDataPoints = 0;
	float Strength;
	bool StrengthSet = false;
	// double TotalWeight;


	// default to be zero vector.  
	OctreeNode(FVector center
	           // =FVector(0,0,0)
	           ,
	           FVector extent
	           // =FVector(0,0,0)
	);

	OctreeNode();

	~OctreeNode();

	bool IsLeaf() const;
	bool ContainsPoint(const FVector point) const;

	// Print data that is stored in the octree node
	void PrintData() const;
	
	void Subdivide();
	void accumulate_with_recursion();
	void accumulate_without_recursion();
	void AccumulateStrengthAndComputeCenterOfMass();
	void Cover(float X0, float Y0, float Z0);
	bool check_contain_data_or_not();
	// void AddAll1(TMap<int32, AKnowledgeNode*> Map1, TArray<FVector> nodePositions);
	void AddAll1(
		TArray<Node> all_nodes,
		TArray<FVector> nodePositions
	);

};

// void AddDataPoint(OctreeNode* node, AKnowledgeNode* newNode, FVector position);

void AddDataPoint(OctreeNode* node, FVector Location,int32 id);

// Define the Callback Type
using OctreeCallback = std::function<bool(OctreeNode*,float alpha, int32 id, TArray<FVector>&  nodePositions, TArray<FVector>&  nodeVelocities)>;

// Declare the BFS traversal function
// void TraverseBFS(OctreeNode* root, OctreeCallback callback);
void TraverseBFS(OctreeNode* root, OctreeCallback callback, float alpha, int32 id, TArray<FVector>&  nodePositions, TArray<FVector>&  nodeVelocities);

// bool SampleCallback(OctreeNode* node);
bool SampleCallback(OctreeNode* node, float alpha, int32 id, TArray<FVector>&  nodePositions, TArray<FVector>&  nodeVelocities);
