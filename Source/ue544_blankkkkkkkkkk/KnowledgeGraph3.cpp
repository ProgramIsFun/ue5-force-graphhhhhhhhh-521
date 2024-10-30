#include "KnowledgeGraph.h"





void AKnowledgeGraph::UpdateBodiesPosition(float DeltaTime)
{
	// Retrieve GPU computed bodies position.
	TArray<FVector3f> GPUOutputPositions = FNBodySimModule::Get().GetComputedPositions();

	if (GPUOutputPositions.Num() != SimParameters.Bodies.Num())
	{
		UE_LOG(LogTemp, Warning,
		       TEXT(
			       "Size differ for GPU Velocities Ouput buffer and current Bodies instanced mesh buffer. Bodies (%d) Output(%d)"
		       ), SimParameters.Bodies.Num(), GPUOutputPositions.Num());
		qq();
		return;
	}else
	{
		ll("Size is same for GPU Velocities Ouput buffer and current Bodies instanced mesh buffer. Bodies (" + FString::FromInt(SimParameters.Bodies.Num()) + ") Output(" + FString::FromInt(GPUOutputPositions.Num()) + ")",true,2);
		ll("First element position is: " + GPUOutputPositions[0].ToString(),true,2);
	}
	
	// QUICK_SCOPE_CYCLE_COUNTER(STAT_SimulationEngine_UpdateBodiesPosition);

	// Update bodies visual with new positions.
	for (int i = 0; i < SimParameters.Bodies.Num(); i++)
	{
		if (use_instance_static_mesh_fornode)
		{
			BodyTransforms[i].SetTranslation(FVector(GPUOutputPositions[i]));
		}


		if (use_text_render_components_fornode)
		{
			TextComponents11111111111111111111[i]->SetWorldLocation(FVector(GPUOutputPositions[i]));
		}
	}

	if (use_instance_static_mesh_fornode)
	{
		InstancedStaticMeshComponent->BatchUpdateInstancesTransforms(0, BodyTransforms, false, true);
	}
}
