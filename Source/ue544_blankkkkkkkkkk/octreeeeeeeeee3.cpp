#include "octreeeeeeeeee3.h"

#include <queue>
#include <stack>

#include "utillllllssss.h"
#include "Chaos/AABB.h"

OctreeNode::OctreeNode(FVector center, FVector extent)
	: Center(center),
	  Extent(extent),
	  TotalDataPoints(0),
	  CenterOfMass(center),
	  Data(nullptr)
{
	Children.SetNum(8, false);
	isCenterSet = true;
}

OctreeNode::OctreeNode()
	:
	// Center(center),
	// Extent(extent),
	TotalDataPoints(0),
	// CenterOfMass(center),
	Data(nullptr)
{
	Children.SetNum(8, false);
}

OctreeNode::~OctreeNode()
{
	for (auto child : Children)
	{
		delete child;
	}
	Children.Empty(); // Clear the TArray, not strictly necessary since the node is being destroyed

	// Now clean up the linked list in `Data`
	PointData* current = Data;
	if (0)
	{
		while (current != nullptr)
		{
			delete current;
			PointData* next = current->Next;
			current = next;
		}
	}
	else
	{
		delete current;
	}
	Data = nullptr; // Reset pointer to nullptr after deletion
}

void OctreeNode::Subdivide()
{
	bool log = true;
	if (
		!IsLeaf() // 
		||
		Data == nullptr // 

			// These two conditions shall never matter, because dysfunctions before calling shall already have the checking. 
	)
		return;

	FVector NewExtent = Extent * 0.5;
	PointData* OldData = Data;


	Data = nullptr; // Clear current data as it's redistributed to children

	for (int i = 0; i < 8; ++i)
	{
		FVector NewCenter = Center + FVector(
			(i & 1) ? NewExtent.X : -NewExtent.X,
			(i & 2) ? NewExtent.Y : -NewExtent.Y,
			(i & 4) ? NewExtent.Z : -NewExtent.Z
		);

		// if (Center - Extent == FVector(-8, -1, 0) && Center + Extent == FVector(8, 15, 16))
		// {
		// 	ll("i: " + FString::FromInt(i) +
		// 	   "newLower bound" + (NewCenter - NewExtent).ToString() +
		// 	   " newUpper bound" + (NewCenter + NewExtent).ToString(), log);
		// }


		Children[i] = new OctreeNode(NewCenter, NewExtent);
	}

	AddDataPoint(this, OldData->Node); // Add the old data point to the children
	delete OldData; // Deleting the old data after redistributing
}

void AddDataPoint(OctreeNode* node, AKnowledgeNode* kn)
{
	FVector newPoint = kn->GetActorLocation();


	if (!node->IsLeaf())
	{
		// Recursively found the leaf node to add the data point


		if (0)
		{
			for (auto* child : node->Children)
			{
				if (1)
				{
					// This method is slow. 
					if (child->ContainsPoint(newPoint))
					{
						AddDataPoint(child, kn);
						return;
					}
				}
				else
				{
				}
			}
		}
		else
		{
			// FVector NewCenter = Center + FVector(
			// (i & 1) ? NewExtent.X : -NewExtent.X,
			// (i & 2) ? NewExtent.Y : -NewExtent.Y,
			// (i & 4) ? NewExtent.Z : -NewExtent.Z

			// We determine which region by just comparing the center of the node with the new point.
			// int i = (z >= zm ? 4 : 0) | (y >= ym ? 2 : 0) | (x >= xm ? 1 : 0);
			int32 i = (newPoint.Z >= node->Center.Z ? 4 : 0) | (newPoint.Y >= node->Center.Y ? 2 : 0) | (newPoint.X >= node->Center.X ? 1 : 0);

			AddDataPoint(node->Children[i], kn);
			
		}
	}
	else
	{
		if (
			node->Data != nullptr
			// If this leaf node already has data
		)
		{
			// If the node already has data, subdivide and add the new data point
			node->Subdivide();
			AddDataPoint(node, kn);
		}
		else
		{
			// No data is associated with the current node
			node->Data = new PointData(kn);
			node->TotalDataPoints = 1; // Now properly accounting for the node having new data

			// All the 8 pointers of children's property Should still be nullptr.
		}
	}
}

void OctreeNode::CalculateCenterOfMass()
{
	if (IsLeaf())
	{
		if (Data)
		{
			CenterOfMass = Data->Node->GetActorLocation();
			TotalDataPoints = 1;
		}
	}
	else
	{
		FVector aggregateMass = FVector(0);
		int count = 0;
		for (OctreeNode* child : Children)
		{
			if (child != nullptr)
			{
				child->CalculateCenterOfMass();
				aggregateMass += child->CenterOfMass * child->TotalDataPoints;
				count += child->TotalDataPoints;
			}
		}
		if (count > 0)
		{
			CenterOfMass = aggregateMass / count;
			TotalDataPoints = count;
		}
	}
}


void OctreeNode::AccumulateStrengthAndComputeCenterOfMass()
{
	if (1)
	{
		FVector aggregatePosition = FVector(0);
		float aggregateStrength = 0.0;
		float totalWeight = 0;

		if (IsLeaf())
		{
			if (Data)
			{
				FVector position = Data->Node->GetActorLocation();
				double strength = Data->Node->strength;

				ll("strength555555555: " + FString::SanitizeFloat(strength));
				Strength = strength;
				StrengthSet = true;

				// TotalWeight = FMath::Abs(strength);


				// In javascript implementations,
				// we extract the value of that node and assign directly xyz property  
				CenterOfMass = position; // Assign directly for leaf nodes
			}
			else
			{
				// If no data is associated with this Leaf node, we will never record the total weight and strength.
				// StrengthSet=false;
			}
		}
		else
		{
			// Recursive accumulation from children nodes
			for (OctreeNode* child : Children)
			{
				if (
					child != nullptr
				)
				{
					child->AccumulateStrengthAndComputeCenterOfMass();


					if (
						child->StrengthSet || !child->IsLeaf()
					)
					{
						float c = FMath::Abs(child->Strength);

						aggregateStrength += child->Strength;


						totalWeight += c;
						ll("c: " + FString::SanitizeFloat(c));
						ll("child->CenterOfMass: " + child->CenterOfMass.ToString());
						aggregatePosition += c * child->CenterOfMass;
					}
				}
				else
				{
					// Should never happens here,
					// because if this is not a leaf note, all the child should not be an empty pointer. 
				}
			}

			// Calculate the center of mass based on total weight
			if (totalWeight > 0)
			{
				ll("aggregateStrength2222: " + FString::SanitizeFloat(aggregateStrength));
				aggregateStrength *= sqrt(4.0 / 8);
				ll("aggregateStrength: " + FString::SanitizeFloat(aggregateStrength));
				ll("aggregatePosition: " + aggregatePosition.ToString());
				ll("totalWeight: " + FString::SanitizeFloat(totalWeight));
				ll("aggregatePosition / totalWeight: " + (aggregatePosition / totalWeight).ToString());
				CenterOfMass = aggregatePosition / totalWeight;
				ll("CenterOfMass: " + CenterOfMass.ToString());
				Strength = aggregateStrength; // Optionally, adjust strength scaling here
				// TotalWeight = totalWeight;
			}
		}
	}
	else
	{
		// Instead of using recursion, we will traverse the tree using bfs and record the path, And then we will calculate center of mass based on the Reverse order.
	}
}

void OctreeNode::Cover(float x, float y, float z)
{
	// intended to call on the lower bound and the highest bound of all the data combined. 


	FVector point(x, y, z);
	if (!isCenterSet)
	{
		// Set the initial bounds as the data point acting as the lowest bound.
		FVector minBound = FVector(FMath::FloorToFloat(x), FMath::FloorToFloat(y), FMath::FloorToFloat(z));

		// Assuming the initial extent should be (1,1,1)
		FVector initialExtent = FVector(1, 1, 1);

		// The center will be the minBound plus half of the initial extent
		Center = minBound + initialExtent * 0.5f;
		Extent = initialExtent * 0.5f;

		isCenterSet = true;
		return;
	}

	while (!ContainsPoint(point))
	{
		FVector temp = Center - Extent;


		Extent *= 2; // Double the size of the octree
		// Determine the direction to adjust the center based on where the point lies

		Center = temp + Extent;

		// FVector direction = point - Center;
		// Center += FVector(
		//     (direction.X < 0) ? -Extent.X / 2 : Extent.X / 2,
		//     (direction.Y < 0) ? -Extent.Y / 2 : Extent.Y / 2,
		//     (direction.Z < 0) ? -Extent.Z / 2 : Extent.Z / 2
		// );
	}
}

void TraverseBFS(OctreeNode* root, OctreeCallback callback, float alpha, AKnowledgeNode* kn)
{
	bool log = true;


	// return;
	if (!root) return; // If the root is null, return immediately

	// std::queue<OctreeNode*> nodeQueue;
	std::stack<OctreeNode*> Stack1;
	Stack1.push(root);
	while (!Stack1.empty())
	{
		OctreeNode* currentNode = Stack1.top();
		Stack1.pop();
		ll("--------------------Right now, dealing with:  Lower bound" +
		   (currentNode->Center - currentNode->Extent).ToString() +
		   " Upper bound" + " " + (currentNode->Center + currentNode->Extent).ToString(), log);
		ll("Prepare to call the call back functions with this node. ", log);
		// Execute the callback on the current node
		bool skipChildren = callback(currentNode, kn, alpha);

		// If callback returns true, do not enqueue children
		if (skipChildren)
		{
			continue;
		}

		if (0)
		{
			// Otherwise, enqueue all non-null children
			for (OctreeNode* child : currentNode->Children)
			{
				if (child)
				{
					Stack1.push(child);
				}
			}
		}
		else
		{
			// Reverse enqueue all non-null children using TArray
			for (int i = currentNode->Children.Num() - 1; i >= 0; --i)
			{
				OctreeNode* child = currentNode->Children[i];
				if (child)
				{
					if (child->Data || !child->IsLeaf())
					{
						ll("i" + FString::FromInt(i), log);
						ll("Lower bound" + (child->Center - child->Extent).ToString() +
						   " Upper bound" + (child->Center + child->Extent).ToString(), log);
						if (0)
						{
							ll("printing the data of the child", log);
							child->PrintData();
							ll("finished printing the data of the child", log);
							
						}
						Stack1.push(child);
					}
				}
			}
		}
	}
}

bool SampleCallback(OctreeNode* node, AKnowledgeNode* kn, float alpha)
{
	bool log = true;
	bool log2 = false;
	ll("-----------------", log);
	// ll("SampleCallback", log);

	if (0)
	{
	}
	else
	{
		// FVector center = CurrentBounds.Center;
		FVector width = node->Extent * 2;


		FVector dir = node->CenterOfMass - kn->GetActorLocation();


		// Remember that direction is the sum of all the Actor locations of the elements in that note. 
		float l = dir.Size() * dir.Size();

		float theta2 = 0.81;
		float distancemax = 1000000000;
		float distancemin = 1;
		// ll("bounds: " + node->Center.ToString() + " " + node->Extent.ToString());

		ll(FString::SanitizeFloat(node->Center.X - node->Extent.X) + " " +
		   FString::SanitizeFloat(node->Center.Y - node->Extent.Y) + " " +
		   FString::SanitizeFloat(node->Center.Z - node->Extent.Z) + " " +
		   FString::SanitizeFloat(node->Center.X + node->Extent.X) + " " +
		   FString::SanitizeFloat(node->Center.Y + node->Extent.Y) + " " +
		   FString::SanitizeFloat(node->Center.Z + node->Extent.Z), log);


		// ll("lower: " + (node->Center - node->Extent).ToString() +
		// 	" upper: " + (node->Center + node->Extent).ToString(), log);

		// ll("width: " + width.ToString(), log);
		ll("dir: " + dir.ToString(), log2);
		ll("l: " + FString::SanitizeFloat(l), log2);
		ll("width.X * width.X / theta2: " + FString::SanitizeFloat(width.X * width.X / theta2), log2);


		// if size of current box is less than distance between nodes
		// This is used to stop recurring down the tree.
		if (width.X * width.X / theta2 < l)
		{
			//        print("GOING IN HERE");
			if (l < distancemax)
			{
				if (0)
				{
					if (dir.X == 0)
					{
						// Assign a random value   // return (random() - 0.5) * 1e-6;
						dir.X = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
						// l += x * x;
						l += dir.X * dir.X;
					}
					if (dir.Y == 0)
					{
						// Assign a random value   // return (random() - 0.5) * 1e-6;
						dir.Y = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
						// l += x * x;
						l += dir.Y * dir.Y;
					}
					if (dir.Z == 0)
					{
						// Assign a random value   // return (random() - 0.5) * 1e-6;
						dir.Z = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
						// l += x * x;
						l += dir.Z * dir.Z;
					}
				}
				else
				{
				}


				if (l < distancemin)
					l = sqrt(distancemin * l);


				//print(FString::SanitizeFloat(ns.strength));


				FVector Vector = dir
					*
					node->Strength
					*
					alpha;

				if (1)
				{
					ll("l: " + FString::SanitizeFloat(l), log2);
					ll("dir: " + dir.ToString(), log2);
					ll("node->Strength: " + FString::SanitizeFloat(node->Strength), log2);
					ll("alpha: " + FString::SanitizeFloat(alpha), log2);

					ll("vector: " + Vector.ToString() + " velocity: " + kn->
					                                                    velocity.ToString(), log2);
				}

				// float mult = pow(ns.strength / nodeStrength, 1.0);
				kn->velocity += Vector / l;


				ll("velocity Updated: " + kn->velocity.ToString(), log);

				if (1)
				{
					if (kn->velocity.Size() > 100000000000000)
					{
						ll("velocity is too large. eeeeeeeeeeeee ");
						eeeee();
					}
				}
			}
			ll("11111111111111 Early termination. ", log);
			return true;
		}

		// if not leaf, get all children

		if (!node->IsLeaf()
			||
			l >= distancemax)
		{
			ll("22222222222222222 You need to return false here. ", log);
			// ll("l: " + FString::SanitizeFloat(l), log);
			return false;
		}


		// For the function to reach here, it has to be a leaf node

		if (node->Data == nullptr)

		{
			ll("Data is null", log, 2);
			return true;
		}
		else
		{
			if (node->Data->Node == nullptr)
			{
				ll("Pointer is null!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!.", log, 2);
			}
			else
			{
			}
		}


		bool bCond = node->Data->Node != kn;


		if (
			// The data is not same as the current node. 
			bCond

			// ||
			// node->Data->Next != nullptr
			//
		)
		{
			// ll("Need to randomize something here.", log);

			//print("IM LEAF");
			if (0)
			{
				if (dir.X)
				{
					// (random() - 0.5) * 1e-6;
					dir.X = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
					l += dir.X * dir.X;
				}
				if (dir.Y)
				{
					// (random() - 0.5) * 1e-6;
					dir.Y = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
					l += dir.Y * dir.Y;
				}
				if (dir.Z)
				{
					// (random() - 0.5) * 1e-6;
					dir.Z = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
					l += dir.Z * dir.Z;
				}
			}
			else
			{
			}
			// if (l < distanceMin2) l = Math.sqrt(distanceMin2 * l);
			if (l < distancemin)
				l = sqrt(distancemin * l);
		}


		// do if (
		// 			treeNode.data !== node
		// 		) {
		// 	w = strengths[treeNode.data.index] * alpha / l;
		// 	node.vx += x * w;
		// 	if (nDim > 1) {
		// 		node.vy += y * w;
		// 	}
		// 	if (nDim > 2) {
		// 		node.vz += z * w;
		// 	}
		// 		} while (treeNode = treeNode.next);
		//

		if (0)
		{
			PointData* currentNode = node->Data;
			while (1)
			{
				if (
					currentNode->Node != kn
				)
				{
					float w = currentNode->Node->strength * alpha / l;


					kn->velocity += dir * w;
				}
				else
				{
				}
				if (currentNode->Next == nullptr)
				{
					break;
				}
				currentNode = currentNode->Next;
			}
		}
		else
		{
			PointData* currentNode = node->Data;

			if (
				currentNode->Node != kn
			)
			{
				float w = currentNode->Node->strength * alpha / l;
				kn->velocity += dir * w;
				ll("velocity Updated: " + kn->velocity.ToString(), log);
			}
		}
		ll("3333333333333333 Returning false at the very end. ", log);
		return false;
	}
}

// Assuming `root` is the root of your Octree and it's properly initialized
// TraverseBFS(root, SampleCallback);


void OctreeNode::AddAll1(TMap<int32, AKnowledgeNode*> Map)
{
	int32 N = Map.Num();


	TArray<float> Xz, Yz, Zz;
	Xz.Reserve(N);
	Yz.Reserve(N);
	Zz.Reserve(N);

	float X0 = std::numeric_limits<float>::infinity();
	float Y0 = std::numeric_limits<float>::infinity();
	float Z0 = std::numeric_limits<float>::infinity();
	float X1 = -std::numeric_limits<float>::infinity();
	float Y1 = -std::numeric_limits<float>::infinity();
	float Z1 = -std::numeric_limits<float>::infinity();

	// Compute the points and their extent
	for (int i = 0; i < N; ++i)
	{
		FVector D = Map[i]->GetActorLocation();
		float X = D.X;
		float Y = D.Y;
		float Z = D.Z;
		Xz.Add(X);
		Yz.Add(Y);
		Zz.Add(Z);

		X0 = FMath::Min(X0, X);
		Y0 = FMath::Min(Y0, Y);
		Z0 = FMath::Min(Z0, Z);
		X1 = FMath::Max(X1, X);
		Y1 = FMath::Max(Y1, Y);
		Z1 = FMath::Max(Z1, Z);
	}

	// Cover the extent


	Cover(X0, Y0, Z0);
	Cover(X1, Y1, Z1);

	ll("!!!!New center and you extend will be set to: " + Center.ToString() + " " + Extent.ToString());
	// Add the new points
	for (int i = 0; i < N; ++i)
	{
		AddDataPoint(this, Map[i]);
	}
}
