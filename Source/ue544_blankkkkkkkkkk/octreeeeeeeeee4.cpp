#include "octreeeeeeeeee3.h"

#include <queue>
#include <stack>

#include "utillllllssss.h"
#include "Chaos/AABB.h"

bool OctreeNode::IsLeaf() const
{
	return Children[0] == nullptr;
}

bool OctreeNode::ContainsPoint(const FVector point) const
{
	return (FMath::Abs(point.X - Center.X) <= Extent.X &&
		FMath::Abs(point.Y - Center.Y) <= Extent.Y &&
		FMath::Abs(point.Z - Center.Z) <= Extent.Z);
}

void OctreeNode::PrintData() const
{
	bool log = true;
	if (Data)
	{
		if (Data->Node)
		{
			ll("77777777777777Data: " + Data->Node->GetActorLocation().ToString(),log);
		}
	}

	for (auto child : Children)
	{
		if (child)
		{
			child->PrintData();
		}
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

bool OctreeNode::check_contain_data_or_not()
{

	if (Data||!IsLeaf())
	{
		
		return true;
		
	}

	return false;
}


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
