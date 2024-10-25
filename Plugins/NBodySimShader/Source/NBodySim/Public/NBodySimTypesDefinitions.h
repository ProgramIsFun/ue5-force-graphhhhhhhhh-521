
#pragma once

// #if defined(__cplusplus)
// 	#define UINT_TYPE	unsigned int
// 	#define INT_TYPE	int
// 	#define FLOAT2_TYPE	FVector2f
// #else
// 	#define UINT_TYPE	uint
// 	#define INT_TYPE	int
// 	#define FLOAT2_TYPE	float2
// #endif



struct FBodyData
{
	float Mass;
	FVector3f Position;
	FVector3f Velocity;

	FBodyData()
		: Mass(0.0f)
		, Position(FVector3f::Zero())
		, Velocity(FVector3f::Zero())
	{
	}
	
	FBodyData(float InMass, FVector3f InPosition, FVector3f InVelocity)
		: Mass(InMass)
		, Position(InPosition)
		, Velocity(InVelocity)
	{
	}
};


// #undef UINT_TYPE
// #undef INT_TYPE