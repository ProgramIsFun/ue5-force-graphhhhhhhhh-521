#pragma once

#include "CoreMinimal.h"

struct FNBodySimParameters;

/**
 *	Holds input/output buffers of the NBodySim compute shader.
 */
struct FNBodySimCSBuffers
{
	// Buffers definition for the compute shader input/output.
	FBufferRHIRef MassesBuffer;
	FShaderResourceViewRHIRef MassesBufferSRV;

	FBufferRHIRef PositionsBuffer;
	FUnorderedAccessViewRHIRef PositionsBufferUAV;

	FBufferRHIRef VelocitiesBuffer;
	FUnorderedAccessViewRHIRef VelocitiesBufferUAV;


	///////////
	FBufferRHIRef LinkOffsetsBuffer;
	FShaderResourceViewRHIRef LinkOffsetsBufferSRV;

	FBufferRHIRef LinkCountsBuffer;
	FShaderResourceViewRHIRef LinkCountsBufferSRV;

	FBufferRHIRef LinkIndicesBuffer;
	FShaderResourceViewRHIRef LinkIndicesBufferSRV;

	FBufferRHIRef LinkStrengthsBuffer;
	FShaderResourceViewRHIRef LinkStrengthsBufferSRV;

	FBufferRHIRef LinkBiasesBuffer;
	FShaderResourceViewRHIRef LinkBiasesBufferSRV;

	FBufferRHIRef LinkinoutBuffer;
	FShaderResourceViewRHIRef LinkinoutBufferSRV;
	///////////
	
	
	void Initialize(const FNBodySimParameters& SimParameters);
	void Release();
};

/**************************************************************************************/
/* This is just an interface we use to keep all the compute shading code in one file. */
/**************************************************************************************/
class FNBodySimCSInterface
{
public:
	static void RunComputeBodyPositions_RenderThread(FRHICommandListImmediate& RHICmdList, FNBodySimParameters& SimParameters, FNBodySimCSBuffers Buffers);

private:
	static FIntVector ComputeGroupSize(uint32 NumBodies);
};