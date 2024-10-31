#include "KnowledgeGraph.h"

#include <random>


#include "utillllllssss.h"
#include <map>
// #include "Misc/FileHelper.h"
// #include "Serialization/JsonSerializer.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)


void AKnowledgeGraph::defaultGenerateGraphMethod()
{
	bool log = true;

	

	nodePositions.SetNumUninitialized(jnodes1);
	nodeVelocities.SetNumUninitialized(jnodes1);
	for (FVector& velocity : nodeVelocities)
	{
		velocity.X = 0.0f;
		velocity.Y = 0.0f;
		velocity.Z = 0.0f;
	}

	//Retrieving an array property and printing each field
	int jnodes11 = jnodes1;
	for (int32 i = 0; i < jnodes11; i++)
	{
		int jid = i;

		if (use_actor_fornode)
		{
			AKnowledgeNode* kn = GetWorld()->SpawnActor<AKnowledgeNode>();

			if (kn)
			{
				UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(kn);
				MeshComp->AttachToComponent(
					kn->GetRootComponent(),
					FAttachmentTransformRules::SnapToTargetIncludingScale
				);
				MeshComp->RegisterComponent(); // Don't forget to register the component


				float sss = static_mesh_size;
				FVector NewScale = FVector(sss, sss, sss);
				MeshComp->SetWorldScale3D(NewScale);


				UStaticMesh* CubeMesh;
				// SelectedMesh1111111111111
				if (0)
				{
					CubeMesh = LoadObject<UStaticMesh>(
						nullptr,
						TEXT(
							"/Engine/BasicShapes/Cube.Cube"
						)
					);
				}
				else
				{
					CubeMesh = SelectedMesh1111111111111;
				}
				if (CubeMesh)
				{
					MeshComp->SetStaticMesh(CubeMesh);
				}
				else
				{
					ll("CubeMesh failed", log, 2);
					qq();
					return;
				}
			}
			AddNode1(jid, kn);
		}

		if (use_text_render_components_fornode)
		{
			UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(
				this, FName("TextComponent" + FString::FromInt(i))
			);
			if (TextComponent)
			{
				TextComponent->SetText(FText::FromString("Sample Text : " + FString::FromInt(i)));
				TextComponent->SetupAttachment(RootComponent);
				TextComponent->RegisterComponent(); // This is important to initialize the component

				TextComponents11111111111111111111.Add(TextComponent);
				// Assuming TextComponents is a valid TArray<UTextRenderComponent*>
			}
		}
	}


	// Edge creation loop
	int jedges11 = jnodes11; // Adjust the number of edges as needed to ensure coverage
	if (!connect_to_previous)
	{
		for (int32 i = 1; i < jedges11; i++)
		{
			int jid = i - 1;
			int jsource = i; // Ensures jsource is always valid within the index range

			// Connected to random node 
			int jtarget = FMath::RandRange(0, i - 1);
			AddEdge(jid, jsource, jtarget);
		}
	}
	else
	{
		ll("Randomly connected is disabled    will always connect to the previous node. ", log);
		for (int32 i = 1; i < jedges11; i++)
		{
			int jid = i - 1;
			int jsource = i; // Ensures jsource is always valid within the index range

			// Connected to random node 
			int jtarget = i - 1;
			AddEdge(jid, jsource, jtarget);
		}
	}
}


void AKnowledgeGraph::generateGraph()
{
	switch (wayofinitnodeslinks)
	{
	case 0:
		{
			// const FString JsonFilePath = FPaths::ProjectContentDir() + "/data/graph.json";
			// FString JsonString; //Json converted to FString
			//
			// FFileHelper::LoadFileToString(JsonString, *JsonFilePath);
			//
			// TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
			// TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
			//
			// if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			// {
			// 	//Retrieving an array property and printing each field
			// 	TArray<TSharedPtr<FJsonValue>> jnodes = JsonObject->GetArrayField("nodes");
			// 	for (int32 i = 0; i < jnodes.Num(); i++)
			// 	{
			// 		auto jobj = jnodes[i]->AsObject();
			// 		int jid = jobj->GetIntegerField("id");
			// 		AKnowledgeNode* kn = GetWorld()->SpawnActor<AKnowledgeNode>();
			//
			//
			// 		// AddNode(jid, kn, FVector(0, 0, 0));
			// 	}
			//
			// 	TArray<TSharedPtr<FJsonValue>> jedges = JsonObject->GetArrayField("edges");
			// 	for (int32 i = 0; i < jedges.Num(); i++)
			// 	{
			// 		auto jobj = jedges[i]->AsObject();
			// 		int jid = jobj->GetIntegerField("id");
			// 		int jsource = jobj->GetIntegerField("source");
			// 		int jtarget = jobj->GetIntegerField("target");
			//
			// 		AddEdge(jid, jsource, jtarget);
			// 	}
			// }
			// else
			// {
			// 	UE_LOG(LogTemp, Warning, TEXT("JSON PARSING FAILED"));
			// }
		}
		break;

	case 1:
		{
		}
		break;

	case 2:
		{
			defaultGenerateGraphMethod();
		}
		break;
	case 3:

		GenerateConnectedGraph(3, 10);
		break;

	default:
		{
			defaultGenerateGraphMethod();
		}
	}
}

void AKnowledgeGraph::GenerateConnectedGraph(int32 NumClusters, int32 NodesPerCluster)
{
}


void AKnowledgeGraph::calculate_link_force_and_update_velocity()
{
	bool log = true;

	int32 Index = 0;
	// link forces
	// After loop, the velocity of all notes have been altered a little bit because of the link force already. 
	for (auto& link : all_links2)
	{
		ll("ApplyForcesssssssssssssssssssss Index: " + FString::FromInt(Index), log);


		FVector source_pos = nodePositions[link.source];
		// ll("source_pos: " + source_pos.ToString(), log);
		FVector source_velocity = nodeVelocities[link.source];
		// ll("source_velocity: " + source_velocity.ToString(), log);
		FVector target_pos = nodePositions[link.target];
		// ll("target_pos: " + target_pos.ToString(), log);
		FVector target_velocity = nodeVelocities[link.target];
		// ll("target_velocity: " + target_velocity.ToString(), log);


		FVector new_v = target_pos + target_velocity - source_pos - source_velocity;
		// ll("new_v: " + new_v.ToString(), log);

		if (new_v.IsNearlyZero())
		{
			new_v = Jiggle(new_v, 1e-6f);
		}


		float l = new_v.Size();
		// UE_LOG(LogTemp, Warning, TEXT("!!!link.Value->distance: %f"), link.Value->distance);

		// ll("l: " + FString::SanitizeFloat(l), log);
		// By looking at the javascript code, we can see strength Will only be computed when there is a change Of the graph structure to the graph.
		l = (l - link.distance) /
			l * alpha * link.strength;
		// ll("l: " + FString::SanitizeFloat(l), log);
		new_v *= l;


		if (0)
		{
		}
		else
		{
			// ll("new_v: " + new_v.ToString(), log);
			// ll("link.Value->bias: " + FString::SanitizeFloat(link.Value->bias), log);
			// target_node->velocity -= new_v * (link.Value->bias);
			nodeVelocities[link.target] -= new_v * (link.bias);
		}

		// source_node->velocity += new_v * (1 - link.Value->bias);
		nodeVelocities[link.source] += new_v * (1 - link.bias);

		Index++;
	}
}

void AKnowledgeGraph::calculate_charge_force_and_update_velocity()
{
	bool log = true;
	bool log2 = false;


	if (!many_body_use_brute_force)
	{
		//
		OctreeData2 = new OctreeNode(
		);


		OctreeData2->AddAll1(all_nodes1, nodePositions);

		OctreeData2->AccumulateStrengthAndComputeCenterOfMass();

		// lll("tttttttttttttttttttttttt");
		ll("!!!OctreeData2->CenterOfMass: " + OctreeData2->CenterOfMass.ToString(), log);
		ll("!!!OctreeData2->strength: " + FString::SanitizeFloat(OctreeData2->Strength), log);


		if (!use_parallel)
		{
			for (auto& node : all_nodes1)
			{
				ll("--------------------------------------", log);
				ll(
					"Traverse the tree And calculate velocity on this Actor Kn, nodekey: -"
					+
					FString::FromInt(node.Key), log);
				TraverseBFS(OctreeData2, SampleCallback, alpha, node.Key, nodePositions, nodeVelocities);
				ll("Finished traversing the tree based on this Actor Kn. ", log);
			}
		}
		else
		{
			ParallelFor(all_nodes1.Num(), [&](int32 Index)
			{
				TraverseBFS(OctreeData2, SampleCallback, alpha, Index, nodePositions, nodeVelocities);
			});
		}


		ll("Finished traversing, now we can delete the tree. ", log);
		delete OctreeData2;
	}
	else
	{
		if (!use_parallel)
		{
			// Brute force
			for (auto& node : all_nodes1)
			{
				auto kn = node.Value;

				for (auto& node2 : all_nodes1)
				{
					auto kn2 = node2.Value;
					if (kn != kn2)
					{
						// FVector dir = kn2->GetActorLocation() - kn->GetActorLocation();
						FVector dir = nodePositions[node2.Key] - nodePositions[node.Key];

						float l = dir.Size() * dir.Size();
						if (l < distancemin)
						{
							l = sqrt(distancemin * l);
						}
						nodeVelocities[node.Key] += dir * nodeStrength * alpha / l;
						// kn->velocity += dir * nodeStrength * alpha / l; 
					}
				}
			}
		}
		else
		{
			ParallelFor(all_nodes1.Num(), [&](int32 Index)
			{
				auto node = all_nodes1[Index];
				for (auto& node2 : all_nodes1)
				{
					auto kn2 = node2.Value;
					if (node != kn2)
					{
						// FVector dir = kn2->GetActorLocation() - kn->GetActorLocation();
						FVector dir = nodePositions[node2.Key] - nodePositions[Index];

						float l = dir.Size() * dir.Size();
						if (l < distancemin)
						{
							l = sqrt(distancemin * l);
						}
						nodeVelocities[Index] += dir * nodeStrength * alpha / l;
						// kn->velocity += dir * nodeStrength * alpha / l; 
					}
				}
			});
		}
	}
}

void AKnowledgeGraph::calculate_centre_force_and_update_position()
{
	// Following is javascript implementation of Center Force
	// for (i = 0; i < n; ++i) {
	// 	node = nodes[i],
	// 		sx += node.x || 0,
	// 		sy += node.y || 0,
	// 		sz += node.z || 0;
	// }
	//
	// for (sx = (sx / n - x) * strength,
	// 		 sy = (sy / n - y) * strength,
	// 		 sz = (sz / n - z) * strength,
	// 		 i = 0;
	// 	 i < n;
	// 	 ++i
	// ) {
	// 	node = nodes[i];
	// 	if (sx) {
	// 		node.x -= sx
	// 	}
	// 	if (sy) {
	// 		node.y -= sy;
	// 	}
	// 	if (sz) {
	// 		node.z -= sz;
	// 	}
	// }
	FVector center = FVector(0, 0, 0);
	FVector aggregation = FVector(0, 0, 0);
	for (auto& node : all_nodes1)
	{
		// aggregation += node.Value->GetActorLocation();
		aggregation += nodePositions[node.Key];
	}
	for (auto& node : all_nodes1)
	{
		nodePositions[node.Key] =
			nodePositions[node.Key] - (
				aggregation / all_nodes1.Num() - center
			) * 1;
		// node.Value->SetActorLocation(
		// 	node.Value->GetActorLocation() - (aggregation / all_nodes.Num() - center) * 1
		// );
	}


	ll("Ignoring the update position step for now in the center force. ");
}


void AKnowledgeGraph::update_position_array_according_to_velocity_array()
{
	if (!use_parallel)
	{
		for (auto& node : all_nodes1)
		{
			auto kn = node.Value;

			nodeVelocities[node.Key] *= velocityDecay;
			// kn->velocity *= velocityDecay;

			// FVector NewLocation = kn->GetActorLocation() + kn->velocity;

			// 	kn->SetActorLocation(
			// 	NewLocation
			// );

			nodePositions[node.Key] = nodePositions[node.Key] + nodeVelocities[node.Key];
		}
	}
	else
	{
		// Assertion failed: ComponentsThatNeedEndOfFrameUpdate_OnGameThread.IsValidIndex(ArrayIndex) [File:D:\build\++UE5\Sync\Engine\Source\Runtime\Engine\Private\LevelTick.cpp] [Line: 872]

		ParallelFor(all_nodes1.Num(), [&](int32 Index)
		{
			nodeVelocities[Index] *= velocityDecay;
			nodePositions[Index] = nodePositions[Index] + nodeVelocities[Index];
		});
	}
}

void AKnowledgeGraph::update_link_position()
{
	for (auto& link : all_links2)
	{
		auto l = link.edge;

		l->ChangeLoc(
			all_nodes1[link.source]->GetActorLocation(),
			all_nodes1[link.target]->GetActorLocation()
		);
	}
}

void AKnowledgeGraph::ApplyForces()
{
	bool log = true;

	// In here velocity of all notes are zeroed
	// In the following for loop, In the first few loop, the velocity is 0. 


	ll("11111111111111111Warning printing out all things. ", log, 1);

	


	ll("Ready to calculate link.--------------------------------------", log);

	double start = FPlatformTime::Seconds();
	calculate_link_force_and_update_velocity();
	double end = FPlatformTime::Seconds();


	ll("Finish calculating link.--------------------------------------", log);



	if (manybody)
	{
		ll("Ready to calculate charge.--------------------------------------", log);

		calculate_charge_force_and_update_velocity();
		ll("Finish calculating charge.--------------------------------------", log);
	}
	else
	{
	}

	calculate_centre_force_and_update_position();
}



void AKnowledgeGraph::initializeNodePosition()
{
	
	
	if (!use_parallel)
	{
		// for (
		// 	auto& node : all_nodes11111111111
		// 	)
		// {
		// 	int index = node.Key;
		// 	initializeNodePosition_Individual(
		// 		index);
		// }
		for (
			int32 index = 0; index < jnodes1; index++
		)
		{
			
			initializeNodePosition_Individual(
				index);
		}
	}
	else
	{
		// ParallelFor(
		// 	all_nodes11111111111.Num()
		// 	, [&](int32 index)
		//             {
		// 	            initializeNodePosition_Individual(
		// 	            	index);
		//             }
		// );

		ParallelFor(
			jnodes1, [&](int32 index)
					{
						initializeNodePosition_Individual(
							index);
					}
		);
	}
}

void AKnowledgeGraph::initializeNodePosition_Individual( int index)
{
	if (use_shaders)
	{
		float RandomMass = FMath::FRandRange(
		20.0
			,
			50.0);

		FVector3f RandomPosition;
		if (!initialize_with_zero_position)
		{
			RandomPosition= FVector3f(RandPointInCircle(
			1000.0
				));
		}
		else
		{
			RandomPosition = FVector3f(0,0,0);
		}


		FVector3f RandomVelocity
		{
			0, 0, 0
		};

		float MeshScale = FMath::Sqrt(RandomMass) * 2.0;
		
		FTransform MeshTransform(
			FRotator(),
			FVector(RandomPosition),
			FVector(MeshScale, MeshScale, MeshScale)
		);

		BodyTransforms[index] = MeshTransform;
		SimParameters.Bodies[index] = FBodyData(RandomMass, RandomPosition, RandomVelocity);
	}
	

	
	// Calculate index-based radius
	float radius;
	int nDim = 3;
	if (nDim > 2)
	{
		radius = initialRadius * cbrt(0.5f + index);
	}
	else if (nDim > 1)
	{
		radius = initialRadius * sqrt(0.5f + index);
	}
	else
	{
		radius = initialRadius * index;
	}

	float initialAngleRoll = PI * (3 - sqrt(5)); // Roll angle

	// Following will be Math.PI * 20 / (9 + Math.sqrt(221));
	float initialAngleYaw = PI * 20 / (9 + sqrt(221)); // Yaw angle if needed (3D)


	float rollAngle = index * initialAngleRoll; // Roll angle
	float yawAngle = index * initialAngleYaw; // Yaw angle if needed (3D)

	FVector init_pos;

	if (nDim == 1)
	{
		// 1D: Positions along X axis
		init_pos = FVector(radius, 0, 0);
	}
	else if (nDim == 2)
	{
		// 2D: Circular distribution
		init_pos = FVector(radius * cos(rollAngle), radius * sin(rollAngle), 0);
	}
	else
	{
		// 3D: Spherical distribution
		init_pos = FVector(radius * sin(rollAngle) * cos(yawAngle), radius * cos(rollAngle),
		                   radius * sin(rollAngle) * sin(yawAngle));
	}

	
	// Check if pointer is valid
	// node->SetActorLocation(init_pos, false);


	nodePositions[index] = init_pos;


	ll("index: " + FString::FromInt(index) + " init_pos: " + init_pos.ToString());

}

void AKnowledgeGraph::update_Node_world_position_according_to_position_array()
{
	for (auto& node : all_nodes1)
	{
		int index = node.Key;
		node.Value->SetActorLocation(nodePositions[index]);
	}
}

void AKnowledgeGraph::CalculateBiasstrengthOflinks()
{
	bool log = true;
	//link forces
	float n = all_nodes1.Num();
	float m = all_links2.Num();

	// std::map<int32, int32> map1;


	std::map<int32, int32> Nodeconnection;
	
	for (auto& link : all_links2)
	{
		
		Nodeconnection[link.source] += 1;
		Nodeconnection[link.target] += 1;
		
	}

	for (auto& link : all_links2)
	{
		int s1=Nodeconnection[link.source];
		int s2=Nodeconnection[link.target];
		
		float ttttttttttt = s1 + s2;
		float bias = s1 / ttttttttttt;
		
		link.bias = bias;
		
		link.strength = 1.0 / fmin(s1,
		                                  s2);
	}


	
}

void AKnowledgeGraph::AddNode1(int32 id, AKnowledgeNode* kn)
{
	if (!all_nodes1.Contains(id))
	{
		nodeVelocities[id] = FVector(0, 0, 0);

		all_nodes1.Emplace(id, kn);
	}
	else
	{
		ll("Fatal error: Node with ID " + FString::FromInt(id) + " already exists!");
	}
}


void AKnowledgeGraph::AddEdge(int32 id, int32 source, int32 target)
{
	AKnowledgeEdge* e;
	Link link=Link(source, target);
	if (useactorforlink)
	{
		UClass* bpClass;
		if (1)
		{
			// This approach works in both play and editor and package game. 

			UClass* loadedClass = StaticLoadClass(UObject::StaticClass(), nullptr,
			                                      TEXT(
				                                      // "Blueprint'/Game/Characters/Enemies/BP_LitchBoss1.BP_LitchBoss1_C'"
				                                      "Blueprint'/Game/arttttttt/iii9.iii9_C'"

			                                      ));
			if (loadedClass)
			{
				e = GetWorld()->SpawnActor<AKnowledgeEdge>(loadedClass);
				// SpawnedActor->TeleportTo(position, rotation.ToOrientationRotator());		
			}
			else
			{
				qq();
				return;
				ll("error loading classsssssssssssssssssssssss");

				e = GetWorld()->SpawnActor<AKnowledgeEdge>(
					AKnowledgeEdge::StaticClass()
				);
				// GEngine->AddOnScreenDebugMessage(-1, 4, FColor::Red, "error loading class");
			}
		}
		else
		{
			// This approach works in Only play in editor
			// Load the Blueprint
			UBlueprint* LoadedBP = Cast<UBlueprint>(StaticLoadObject(
					UBlueprint::StaticClass(),
					nullptr,
					TEXT(
						// "Blueprint'/Game/NewBlueprint22222.NewBlueprint22222'"
						"Blueprint'/Game/kkkkk/NewBlueprint22222.NewBlueprint22222'"
					)
				)
			);
			if (!LoadedBP)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load the Blueprint."));
				eeeee();
			}
			// Check if the Blueprint class is valid
			bpClass = LoadedBP->GeneratedClass;
			if (!bpClass)
			{
				UE_LOG(LogTemp, Error, TEXT("Generated class from Blueprint is null."));
				eeeee();
			}
			e = GetWorld()->SpawnActor<AKnowledgeEdge>(
				bpClass
			);
		}

		//
		// e->strength = 1;
		//
		link.strength = 1;
		//
		// e->distance = edgeDistance;
		//
		link.distance = edgeDistance;

		// all_links1.Emplace(id, e);
		link.edge = e;


	}
	else
	{
		// ULineBatchComponent* LineBatch = NewObject<ULineBatchComponent>(this);
		// LineBatch->RegisterComponent();
		// LineBatch->DrawLine(StartPosition, EndPosition, FLinearColor::Green, SDPG_World, 10.0f, 10.0f);



		// all_links2.Add(ALLLink(source, target));

		ll("Right now We only have actor option for link. ", true, 2);
		qq();
	}

	all_links2.Add(link);



}
