#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "KnowledgeNode.h"
// #include "PointData.h" // Make sure this include points to your actual PointData structure file location

struct PointData
{
	// FVector Position;
	// FVector Velocity;
	AKnowledgeNode* Node;

	PointData* Next = nullptr;

	PointData(
		// FVector position = FVector(),
		// FVector velocity = FVector()
		AKnowledgeNode* node = nullptr
		)
		:
	// Position(position),
	// Velocity(velocity)
	Node(node)
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
	void CalculateCenterOfMass();
	void AccumulateStrengthAndComputeCenterOfMass();
	void Cover(float X0, float Y0, float Z0);

	void AddAll1(TMap<int32, AKnowledgeNode*> Map1);
};

void AddDataPoint(OctreeNode* node, AKnowledgeNode* newNode);


// Define the Callback Type
using OctreeCallback = std::function<bool(OctreeNode*,AKnowledgeNode* kn, float alpha)>;

// Declare the BFS traversal function
// void TraverseBFS(OctreeNode* root, OctreeCallback callback);
void TraverseBFS(OctreeNode* root, OctreeCallback callback, float alpha, AKnowledgeNode* kn);

// bool SampleCallback(OctreeNode* node);
bool SampleCallback(OctreeNode* node, AKnowledgeNode* kn, float alpha);
