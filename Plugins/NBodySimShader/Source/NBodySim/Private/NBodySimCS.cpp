
#pragma once

#include "NBodySimCS.h"

#include "CanvasTypes.h"
#include "GlobalShader.h"
#include "PixelShaderUtils.h"
#include "RenderGraphResources.h"
#include "RHIGPUReadback.h"
#include "ShaderParameterStruct.h"
#include "StaticMeshResources.h"
#include "UnifiedBuffer.h"
#include "NBodySimModule.h"

DECLARE_STATS_GROUP(TEXT("NBodySimCS"), STATGROUP_NBodySimCS, STATCAT_Advanced);
DECLARE_CYCLE_STAT(TEXT("NBodySimCS Execute"), STAT_NBodySimCS_Execute, STATGROUP_NBodySimCS);

/**********************************************************************************************/
/* This class carries our parameter declarations and acts as the bridge between cpp and HLSL. */
/**********************************************************************************************/
class FNBodySimCS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FNBodySimCS);
	SHADER_USE_PARAMETER_STRUCT(FNBodySimCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_SRV(StructuredBuffer<float>, Masses)


		SHADER_PARAMETER_UAV(RWStructuredBuffer<FVector3f>, Positions)
		SHADER_PARAMETER_UAV(RWStructuredBuffer<FVector3f>, Velocities)


		SHADER_PARAMETER_SRV(StructuredBuffer<int>, LinkOffsets)
		SHADER_PARAMETER_SRV(StructuredBuffer<int>, LinkCounts)
		SHADER_PARAMETER_SRV(StructuredBuffer<int>, LinkIndices)
		SHADER_PARAMETER_SRV(StructuredBuffer<int>, Linkinout)
		SHADER_PARAMETER_SRV(StructuredBuffer<float>, LinkStrengths)
		SHADER_PARAMETER_SRV(StructuredBuffer<float>, LinkBiases)




	
		SHADER_PARAMETER(uint32, NumBodies)
		SHADER_PARAMETER(float, GravityConstant)
		SHADER_PARAMETER(float, CameraAspectRatio)
		SHADER_PARAMETER(float, ViewportWidth)
		SHADER_PARAMETER(float, DeltaTime)
		SHADER_PARAMETER(float, alpha)

	END_SHADER_PARAMETER_STRUCT()

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return true;
	}

	static bool ShouldCache(EShaderPlatform Platform)
	{
		return true;
	}

	static inline void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);

		// OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_X"), 256);
		// OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Y"), 1);
		// OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Z"), 1);
	}
};

// This will tell the engine to create the shader and where the shader entry point is.
//                      ShaderType              ShaderPath                     Shader function name    Type
IMPLEMENT_GLOBAL_SHADER(FNBodySimCS, "/NBodySimShaders/Private/NBodySim.usf", "CalculateVelocitiesCS", SF_Compute);



void FNBodySimCSBuffers::Initialize(const FNBodySimParameters& SimParameters)
{
	if (!MassesBuffer || !MassesBufferSRV)
	{
		TResourceArray<float> ResourceArray;
		ResourceArray.Init(0.0f, SimParameters.Bodies.Num());

		for (int i = 0; i < SimParameters.Bodies.Num(); i++)
		{
			ResourceArray[i] = SimParameters.Bodies[i].Mass;
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_MassesBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		MassesBuffer = RHICreateStructuredBuffer(sizeof(float), SimParameters.Bodies.Num() * sizeof(float), BUF_ShaderResource, CreateInfo);
		MassesBufferSRV = RHICreateShaderResourceView(MassesBuffer);
	}

	if (!PositionsBuffer || !PositionsBufferUAV)
	{
		TResourceArray<FVector3f> ResourceArray;
		ResourceArray.Init(FVector3f::ZeroVector, SimParameters.Bodies.Num());

		for (int i = 0; i < SimParameters.Bodies.Num(); i++)
		{
			ResourceArray[i] = SimParameters.Bodies[i].Position;
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_PositionsBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		PositionsBuffer = RHICreateStructuredBuffer(sizeof(FVector3f), SimParameters.Bodies.Num() * sizeof(FVector3f), BUF_UnorderedAccess | BUF_ShaderResource, CreateInfo);
		PositionsBufferUAV = RHICreateUnorderedAccessView(PositionsBuffer, false, true);
	}

	if (!VelocitiesBuffer || !VelocitiesBufferUAV)
	{
		TResourceArray<FVector3f> ResourceArray;
		ResourceArray.Init(FVector3f::ZeroVector, SimParameters.Bodies.Num());

		for (int i = 0; i < SimParameters.Bodies.Num(); i++)
		{
			ResourceArray[i] = SimParameters.Bodies[i].Velocity;
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_VelocitiesBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		VelocitiesBuffer = RHICreateStructuredBuffer(sizeof(FVector3f), SimParameters.Bodies.Num() * sizeof(FVector3f), BUF_UnorderedAccess | BUF_ShaderResource, CreateInfo);
		VelocitiesBufferUAV = RHICreateUnorderedAccessView(VelocitiesBuffer, false, true);
	}


	
	if (!LinkOffsetsBuffer || !LinkOffsetsBufferSRV)
	{
		TResourceArray<int> ResourceArray;
		ResourceArray.Init(0, SimParameters.LinkOffsets.Num());

		for (int i = 0; i < SimParameters.LinkOffsets.Num(); i++)
		{
			ResourceArray[i] = SimParameters.LinkOffsets[i];
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_LinkOffsetsBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		LinkOffsetsBuffer = RHICreateStructuredBuffer(sizeof(int), SimParameters.LinkOffsets.Num() * sizeof(int), BUF_ShaderResource, CreateInfo);
		LinkOffsetsBufferSRV = RHICreateShaderResourceView(LinkOffsetsBuffer);
	}

	if (!LinkCountsBuffer || !LinkCountsBufferSRV)
	{
		TResourceArray<int> ResourceArray;
		ResourceArray.Init(0, SimParameters.LinkCounts.Num());

		for (int i = 0; i < SimParameters.LinkCounts.Num(); i++)
		{
			ResourceArray[i] = SimParameters.LinkCounts[i];
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_LinkCountsBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		LinkCountsBuffer = RHICreateStructuredBuffer(sizeof(int), SimParameters.LinkCounts.Num() * sizeof(int), BUF_ShaderResource, CreateInfo);
		LinkCountsBufferSRV = RHICreateShaderResourceView(LinkCountsBuffer);
	}

	if (!LinkIndicesBuffer || !LinkIndicesBufferSRV)
	{
		TResourceArray<int> ResourceArray;
		ResourceArray.Init(0, SimParameters.LinkIndices.Num());

		for (int i = 0; i < SimParameters.LinkIndices.Num(); i++)
		{
			ResourceArray[i] = SimParameters.LinkIndices[i];
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_LinkIndicesBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		LinkIndicesBuffer = RHICreateStructuredBuffer(sizeof(int), SimParameters.LinkIndices.Num() * sizeof(int), BUF_ShaderResource, CreateInfo);
		LinkIndicesBufferSRV = RHICreateShaderResourceView(LinkIndicesBuffer);
	}

	if (!LinkStrengthsBuffer || !LinkStrengthsBufferSRV)
	{
		TResourceArray<float> ResourceArray;
		ResourceArray.Init(0.0f, SimParameters.LinkStrengths.Num());

		for (int i = 0; i < SimParameters.LinkStrengths.Num(); i++)
		{
			ResourceArray[i] = SimParameters.LinkStrengths[i];
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_LinkStrengthsBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		LinkStrengthsBuffer = RHICreateStructuredBuffer(sizeof(float), SimParameters.LinkStrengths.Num() * sizeof(float), BUF_ShaderResource, CreateInfo);
		LinkStrengthsBufferSRV = RHICreateShaderResourceView(LinkStrengthsBuffer);
	}

	if (!LinkBiasesBuffer || !LinkBiasesBufferSRV)
	{
		TResourceArray<float> ResourceArray;
		ResourceArray.Init(0.0f, SimParameters.LinkBiases.Num());

		for (int i = 0; i < SimParameters.LinkBiases.Num(); i++)
		{
			ResourceArray[i] = SimParameters.LinkBiases[i];
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_LinkBiasesBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		LinkBiasesBuffer = RHICreateStructuredBuffer(sizeof(float), SimParameters.LinkBiases.Num() * sizeof(float), BUF_ShaderResource, CreateInfo);
		LinkBiasesBufferSRV = RHICreateShaderResourceView(LinkBiasesBuffer);
	}

	if (!LinkinoutBuffer || !LinkinoutBufferSRV)
	{
		TResourceArray<int> ResourceArray;
		ResourceArray.Init(0, SimParameters.Linkinout.Num());

		for (int i = 0; i < SimParameters.Linkinout.Num(); i++)
		{
			ResourceArray[i] = SimParameters.Linkinout[i];
		}

		FRHIResourceCreateInfo CreateInfo(TEXT("RHICreateInfo_LinkinoutBuffer"));
		CreateInfo.ResourceArray = &ResourceArray;

		LinkinoutBuffer = RHICreateStructuredBuffer(sizeof(int), SimParameters.Linkinout.Num() * sizeof(int), BUF_ShaderResource, CreateInfo);
		LinkinoutBufferSRV = RHICreateShaderResourceView(LinkinoutBuffer);
	}
	

	
	//
	// TArray<int> LinkOffsets;  // Holds the offset for each body
	// TArray<int> LinkCounts;   // Holds the count of links for each body
	// TArray<int> LinkIndices;  // Flat array containing all links
	// TArray<float> LinkStrengths;  // Holds the strength of each link
	// TArray<float> LinkBiases;     // Holds the bias of each link
	// TArray<int> Linkinout;  

	
}

void FNBodySimCSBuffers::Release()
{
	if (MassesBuffer)			MassesBuffer.SafeRelease();
	if (MassesBufferSRV)		MassesBufferSRV.SafeRelease();
	
	if (PositionsBuffer)		PositionsBuffer.SafeRelease();
	if (PositionsBufferUAV)	PositionsBufferUAV.SafeRelease();

	if (VelocitiesBuffer)		VelocitiesBuffer.SafeRelease();
	if (VelocitiesBufferUAV)	VelocitiesBufferUAV.SafeRelease();

	
	if (LinkOffsetsBuffer)		LinkOffsetsBuffer.SafeRelease();
	if (LinkOffsetsBufferSRV)	LinkOffsetsBufferSRV.SafeRelease();

	if (LinkCountsBuffer)		LinkCountsBuffer.SafeRelease();
	if (LinkCountsBufferSRV)	LinkCountsBufferSRV.SafeRelease();

	if (LinkIndicesBuffer)		LinkIndicesBuffer.SafeRelease();
	if (LinkIndicesBufferSRV)	LinkIndicesBufferSRV.SafeRelease();

	if (LinkStrengthsBuffer)	LinkStrengthsBuffer.SafeRelease();
	if (LinkStrengthsBufferSRV)	LinkStrengthsBufferSRV.SafeRelease();

	if (LinkBiasesBuffer)		LinkBiasesBuffer.SafeRelease();
	if (LinkBiasesBufferSRV)	LinkBiasesBufferSRV.SafeRelease();

	if (LinkinoutBuffer)		LinkinoutBuffer.SafeRelease();
	if (LinkinoutBufferSRV)		LinkinoutBufferSRV.SafeRelease();
	

	
	

	
}

void FNBodySimCSInterface::RunComputeBodyPositions_RenderThread(FRHICommandListImmediate& RHICmdList, FNBodySimParameters& SimParameters, FNBodySimCSBuffers Buffers)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_ShaderPlugin_ComputeBodyPositions); // Used to gather CPU profiling data for the UE4 session frontend
	SCOPED_DRAW_EVENT(RHICmdList, ShaderPlugin_ComputeBodyPositions); // Used to profile GPU activity and add metadata to be consumed by for example RenderDoc

	// Transition setup
	ERHIAccess BarrierAccess = ERHIAccess::CopySrc | ERHIAccess::CopyDest | ERHIAccess::SRVCompute | ERHIAccess::SRVGraphics | ERHIAccess::UAVCompute | ERHIAccess::UAVGraphics;
	
	FRHITransitionInfo TransitionInfoPositions(Buffers.PositionsBuffer, BarrierAccess);
	RHICmdList.Transition(TransitionInfoPositions);
		
	FRHITransitionInfo TransitionInfoVelocities(Buffers.VelocitiesBuffer, BarrierAccess);
	RHICmdList.Transition(TransitionInfoVelocities);

	// Shader Parameters setup.
	FNBodySimCS::FParameters PassParameters;
	PassParameters.Masses = Buffers.MassesBufferSRV;
	PassParameters.Positions = Buffers.PositionsBufferUAV;
	PassParameters.Velocities = Buffers.VelocitiesBufferUAV;

	PassParameters.LinkOffsets = Buffers.LinkOffsetsBufferSRV;
	PassParameters.LinkCounts = Buffers.LinkCountsBufferSRV;
	PassParameters.LinkIndices = Buffers.LinkIndicesBufferSRV;
	PassParameters.LinkStrengths = Buffers.LinkStrengthsBufferSRV;
	PassParameters.LinkBiases = Buffers.LinkBiasesBufferSRV;
	PassParameters.Linkinout = Buffers.LinkinoutBufferSRV;

	
	
	PassParameters.NumBodies = SimParameters.NumBodies;
	PassParameters.GravityConstant = SimParameters.GravityConstant;
	PassParameters.CameraAspectRatio = SimParameters.CameraAspectRatio;
	PassParameters.ViewportWidth = SimParameters.ViewportWidth;
	PassParameters.DeltaTime = SimParameters.DeltaTime;
	PassParameters.alpha = SimParameters.alpha;

	// Dispatch.
	TShaderMapRef<FNBodySimCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	// FIntVector GroupCount = FComputeShaderUtils::GetGroupCount(ComputeGroupSize(SimParameters.NumBodies), FComputeShaderUtils::kGolden2DGroupSize);

	FComputeShaderUtils::Dispatch(RHICmdList, ComputeShader, PassParameters, ComputeGroupSize(SimParameters.NumBodies));
}

FIntVector FNBodySimCSInterface::ComputeGroupSize(uint32 NumBodies)
{
	const int ThreadCount = 256;

	int FinalCount = ((NumBodies - 1) / ThreadCount) + 1;

	return FIntVector(FinalCount, 1, 1);
}
