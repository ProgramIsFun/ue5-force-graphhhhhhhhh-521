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
