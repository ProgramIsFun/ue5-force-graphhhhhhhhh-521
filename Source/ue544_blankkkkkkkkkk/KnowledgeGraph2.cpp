#include "KnowledgeGraph.h"

#include <random>


#include "utillllllssss.h"
// #include "Misc/FileHelper.h"
// #include "Serialization/JsonSerializer.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::White,text)


void AKnowledgeGraph::defaultGenerateGraphMethod()
{
	bool log = true;
	//Retrieving an array property and printing each field
	int jnodes11 = jnodes1;
	for (int32 i = 0; i < jnodes11; i++)
	{
		int jid = i;
		AKnowledgeNode* kn = GetWorld()->SpawnActor<AKnowledgeNode>();
		if (kn)
		{
			UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(
				kn->GetComponentByClass(UPrimitiveComponent::StaticClass()));
			if (PrimitiveComponent)
			{
				PrimitiveComponent->SetSimulatePhysics(false);
				PrimitiveComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
		AddNode1(jid, kn);
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
			int jsource = i ; // Ensures jsource is always valid within the index range

			// Connected to random node 
			int jtarget = i-1;
			AddEdge(jid, jsource, jtarget);
		}
	}
}


void AKnowledgeGraph::generateGraph()
{
	switch (wayofinitnodeslinks)
	{
	//json crap


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
			//Retrieving an array property and printing each field
			int jnodes = jnodes1;
			for (int32 i = 0; i < jnodes; i++)
			{
				int jid = i;
				AKnowledgeNode* kn = GetWorld()->SpawnActor<AKnowledgeNode>();
				// AddNode(jid, kn, FVector(0, 0, 0));
			}

			// Edge creation loop
			int jedges = jnodes; // Adjust the number of edges as needed to ensure coverage
			std::random_device rd;
			std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> dis(0, jnodes - 1); // Random number distribution

			for (int32 i = 0; i < jedges; i++)
			{
				int jid = i;
				int jsource = i % jnodes; // Ensures jsource is always valid within the index range
				int jtarget = (i + 1) % jnodes; // Connect each node to the next node; wraps around

				AddEdge(jid, jsource, jtarget);

				// Random additional edge to increase graph density
				if (i < jnodes - 1)
				{
					int random_target = dis(gen);
					// Ensure that we do not add self-loops
					if (random_target != jsource)
					{
						AddEdge(jid + jnodes, jsource, random_target); // Use jid+jnodes to keep distinct edge IDs
					}
				}
			};
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
	if (!GetWorld()) return;

	TArray<int32> ClusterCenterIDs;
	ClusterCenterIDs.Reserve(NumClusters);

	// Create Cluster Centers
	for (int32 i = 0; i < NumClusters; ++i)
	{
		FVector Location = FVector(
			0,
			0,
			0
		);
		AKnowledgeNode* Node = GetWorld()->SpawnActor<AKnowledgeNode>(AKnowledgeNode::StaticClass(), Location,
		                                                              FRotator::ZeroRotator);
		int32 nodeId = i; // Assign node ID, assumed incremented or derived
		// AddNode(nodeId, Node, Location);
		ClusterCenterIDs.Add(nodeId);
	}

	// Connect Cluster Nodes
	for (int32 i = 0; i < NumClusters; ++i)
	{
		for (int32 j = 1; j < NodesPerCluster; ++j)
		{
			FVector Location = FVector(i * 100.0f, j * 50.0f, 0); // Organize per cluster
			AKnowledgeNode* Node = GetWorld()->SpawnActor<AKnowledgeNode>(
				AKnowledgeNode::StaticClass(), Location, FRotator::ZeroRotator);
			int32 nodeId = i * NodesPerCluster + j; // Calculate unique node ID


			// AddNode(nodeId, Node, Location);

			if (1)
			{
				AddEdge(nodeId,
				        ClusterCenterIDs[i],
				        nodeId
				); // Use node IDs for connection
			}
			else
			{
				AddEdge(nodeId,
				        nodeId,

				        ClusterCenterIDs[i]
				); // Use node IDs for connection
			}
		}
	}

	// Inter-cluster Connections
	for (int32 i = 0; i < NumClusters - 1; ++i)
	{
		AddEdge(i, ClusterCenterIDs[i], ClusterCenterIDs[i + 1]); // Use node IDs to connect cluster centers
	}
}


void AKnowledgeGraph::calculate_link_force_and_update_velocity()
{
	bool log = true;

	int32 Index = 0;
	// link forces
	// After loop, the velocity of all notes have been altered a little bit because of the link force already. 
	for (auto& link : all_links)
	{
		ll("ApplyForcesssssssssssssssssssss Index: " + FString::FromInt(Index), log);
		auto source_node = all_nodes[link.Value->source];
		auto target_node = all_nodes[link.Value->target];

		FVector source_pos = source_node->GetActorLocation();
		ll("source_pos: " + source_pos.ToString(), log);
		FVector source_velocity = source_node->velocity;
		ll("source_velocity: " + source_velocity.ToString(), log);
		FVector target_pos = target_node->GetActorLocation();
		ll("target_pos: " + target_pos.ToString(), log);
		FVector target_velocity = target_node->velocity;
		ll("target_velocity: " + target_velocity.ToString(), log);


		FVector new_v = target_pos + target_velocity - source_pos - source_velocity;
		ll("new_v: " + new_v.ToString(), log);

		if (new_v.IsNearlyZero())
		{
			new_v = Jiggle(new_v, 1e-6f);
		}

		float l = new_v.Size();
		// UE_LOG(LogTemp, Warning, TEXT("!!!link.Value->distance: %f"), link.Value->distance);

		ll("l: " + FString::SanitizeFloat(l), log);
		// By looking at the javascript code, we can see strength Will only be computed when there is a change Of the graph structure to the graph.
		l = (l - link.Value->distance) /
			l * alpha * link.Value->strength;
		ll("l: " + FString::SanitizeFloat(l), log);
		new_v *= l;


		// Record targeted original velocity.
		FVector target_original_velocity = target_node->velocity;


		if (0)
		{
			target_node->velocity -= new_v * (1 - link.Value->bias);
		}
		else
		{
			ll("new_v: " + new_v.ToString(), log);
			ll("link.Value->bias: " + FString::SanitizeFloat(link.Value->bias), log);
			target_node->velocity -= new_v * (link.Value->bias);
		}

		// Log out the original velocity and the update velocity. 
		ll("TARGET VELOCITY: " + target_original_velocity.ToString() +
		   " -> " + target_node->velocity.ToString(), log);


		// Record source original velocity.
		FVector source_original_velocity = source_node->velocity;


		source_node->velocity += new_v * (1 - link.Value->bias);


		ll("SOURCE VELOCITY: " + source_original_velocity.ToString() + " -> " + source_node->velocity.ToString(), log);


		Index++;
	}
}

void AKnowledgeGraph::calculate_charge_force_and_update_velocity()
{
	bool log = true;
	bool log2 = false;
	if (1)
	{
		if (0)
		{
			// //charge forces
			// octree_node_strengths.Empty();
			//
			// // Instead of removing the elements we create a new tree again. 	
			// if (1)
			// {
			// 	ll("We now calculate a bound suitable. To be implemented.  ");
			//
			//
			//
			//
			// 	InitOctree(FBox(
			// 			FVector(-200, -200, -200),
			// 			FVector(200, 200, 200)
			// 		)
			// 	);
			// }
			// else
			// {
			// 	InitOctree(FBox(
			// 			FVector(0, 0, 0),
			// 			FVector(1, 1, 1)
			// 		)
			// 	);
			// }
			//
			//
			// for (auto& node : all_nodes)
			// {
			// 	int key = node.Key;
			// 	auto kn = node.Value;
			//
			//
			// 	// Because the actor location hasn't changed when we compute the link force, so These two lines could actually be put in the start of the function. 
			//
			// 	// If they are remove here，then why we do AddOctreeElement(ote) in AddNode?
			//
			//
			// 	if (0)
			// 	{
			// 		RemoveElement(node.Key); //need to remove then update with new location when adding
			// 		AddNode(key, kn, kn->GetActorLocation());
			// 	}
			// 	else
			// 	{
			// 	
			// 		AddNode(key, kn, kn->GetActorLocation());
			// 	
			// 	}
			//
			//
			// }
			//
			// Accumulate();
			//
			// for (auto& node : all_nodes)
			// {
			// 	ApplyManyBody(node.Value);
			// }
		}
		else
		{
			//
			OctreeData2 = new OctreeNode(
			);


			OctreeData2->AddAll1(all_nodes);

			OctreeData2->AccumulateStrengthAndComputeCenterOfMass();

			// lll("tttttttttttttttttttttttt");
			ll("!!!OctreeData2->CenterOfMass: " + OctreeData2->CenterOfMass.ToString(), log);
			ll("!!!OctreeData2->strength: " + FString::SanitizeFloat(OctreeData2->Strength), log);

			for (auto& node : all_nodes)
			{
				ll("--------------------------------------", log);
				ll(
					"Traverse the tree And calculate velocity on this Actor Kn, nodekey: -"
					+
					FString::FromInt(node.Key), log);
				TraverseBFS(OctreeData2, SampleCallback, alpha, node.Value);
				ll("Finished traversing the tree based on this Actor Kn. ", log);
			}
			ll("Finished traversing, now we can delete the tree. ", log);
			delete OctreeData2;
		}
	}
}

void AKnowledgeGraph::apply_center_force_and_move_the_node_directly()
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
	for (auto& node : all_nodes)
	{
		aggregation += node.Value->GetActorLocation();
	}
	for (auto& node : all_nodes)
	{
		node.Value->SetActorLocation(
			node.Value->GetActorLocation() - (aggregation / all_nodes.Num() - center) * 1
		);
	}
}


void AKnowledgeGraph::update_actor_location_based_on_velocity()
{
	for (auto& node : all_nodes)
	{
		auto kn = node.Value;


		if (0)
		{
			ll("POSITION! node: " + FString::FromInt(node.Key));
			ll("position: " + kn->GetActorLocation().ToString());
			ll("velocity: " + kn->velocity.ToString());
		}
		else
		{
		}


		kn->velocity *= velocityDecay;


		FVector NewLocation = kn->GetActorLocation() + kn->velocity;

		kn->SetActorLocation(
			NewLocation
		);
		if (0)
		{
			ll("FINAL POSITION! node: " + FString::FromInt(node.Key));
			ll("position: " + kn->GetActorLocation().ToString());
			ll("velocity: " + kn->velocity.ToString());
		}
		else
		{
		}


		if (0)
		{
			kn->velocity *= 0; //reset velocities
		}

		if (0)
		{
			ll("FINAL POSITION! node: " + FString::FromInt(node.Key));
			ll("position: " + kn->GetActorLocation().ToString());
			ll("velocity: " + kn->velocity.ToString());
		}
		else
		{
		}
	}
}

void AKnowledgeGraph::update_link_position()
{
	for (auto& link : all_links)
	{
		auto l = link.Value;

		l->ChangeLoc(
			all_nodes[l->source]->GetActorLocation(),
			all_nodes[l->target]->GetActorLocation()
		);
	}
}

void AKnowledgeGraph::ApplyForces()
{
	bool log = true;

	// In here velocity of all notes are zeroed
	// In the following for loop, In the first few loop, the velocity is 0. 



	ll("11111111111111111Warning printing out all things. ", log, 1);
	// Print out the position and velocity of all the nodes. 
	for (auto& node : all_nodes)
	{
		auto kn = node.Value;
		ll("node: " + FString::FromInt(node.Key), log);
		ll("position: " + kn->GetActorLocation().ToString(), log);
		ll("velocity: " + kn->velocity.ToString(), log);
	}

	
	ll("Ready to calculate link.--------------------------------------", log);
	calculate_link_force_and_update_velocity();
	ll("Finish calculating link.--------------------------------------", log);


	ll("222222222222222222Warning printing out all things. ", log, 1);
	// Print out the position and velocity of all the nodes. 
	for (auto& node : all_nodes)
	{
		auto kn = node.Value;
		ll("node: " + FString::FromInt(node.Key), log);
		ll("position: " + kn->GetActorLocation().ToString(), log);
		ll("velocity: " + kn->velocity.ToString(), log);
	}
	
	if (manybody)
	{
		ll("Ready to calculate charge.--------------------------------------", log);

		calculate_charge_force_and_update_velocity();
		ll("Finish calculating charge.--------------------------------------", log);
	}
	else
	{
	}

	apply_center_force_and_move_the_node_directly();
}

// NodeStrength AKnowledgeGraph::AddUpChildren(
// 	const FSimpleOctree::FNode& node,
// 	FString node_id
// )
// {
// 	ll("AddUpChildren--------------------------------------");
// 	if (!octree_node_strengths.Contains(node_id))
// 	{
// 		octree_node_strengths.Emplace(node_id, NodeStrength());
// 	}
// 	else
// 	{
// 		//reset
// 		octree_node_strengths[node_id].strength = 0;
// 		octree_node_strengths[node_id].direction = FVector(0, 0, 0);
// 	}
//
// 	if (!node.IsLeaf())
// 	{
// 		
// 		int count = 0;
// 		float c;
// 		float strength = 0.0;
// 		float weight = 0.0;
// 		FVector tempvec;
//
//
// 		FOREACH_OCTREE_CHILD_NODE3(ChildRef)
// 		{
// 			//go find the leaves
// 			if (
//
// 				// ChildRef is a special structure, but basically store a number from one to 7. 
// 				node.HasChild(ChildRef)
// 			)
// 			{
// 				NodeStrength ns = AddUpChildren(
// 					*node.GetChild(ChildRef),
// 					node_id + FString::FromInt(count)
// 				);
// 				//add up children
// 				//math for vector and strength
// 				c = abs(ns.strength);
//
// 				 
// 				strength += ns.strength;
//
// 				weight += c;
//
//
// 				tempvec += c * ns.direction;
//
// 				count++;
// 			}
// 		}
//
// 		// Check in the original source code. 
// 		// strength *= Math.sqrt(4 / numChildren); // scale accumulated strength according to number of dimensions
// 		strength *= sqrt(0.5f); // scale accumulated strength according to number of dimensions
// 		octree_node_strengths[node_id].strength = strength; //hash of ID of node for map
//
//
// 		
// 		octree_node_strengths[node_id].direction = tempvec / weight;
// 	}
// 	else
// 	{
// 		for (FSimpleOctree::ElementConstIt ElementIt(node.GetElementIt()); ElementIt; ++ElementIt)
// 		{
// 			//all the way down to elements
// 			const FOctreeElement& Sample = *ElementIt;
//
// 			// So basically we add up all the strength inside a leaf node
// 			octree_node_strengths[node_id].strength += Sample.MyActor->strength;
// 			// When we summing up all the actual locations as a vector, it seems that it take an average location. 
// 			octree_node_strengths[node_id].direction += Sample.MyActor->GetActorLocation();
// 		}
// 	}
//
// 	// octree_node_strengths[node_id].direction.ToString(); //?
// 	return octree_node_strengths[node_id];
// }
//
// //get weights for every node before applying
// void AKnowledgeGraph::Accumulate()
// {
// 	ll("Accumulate--------------------------------------");
// 	for (
// 		FSimpleOctree::TConstIterator<> NodeIt(*OctreeData);
// 		NodeIt.HasPendingNodes();
// 		NodeIt.Advance()
// 	)
// 	{
// 		const FSimpleOctree::FNode& CurrentNode = NodeIt.GetCurrentNode();
// 		// UE_LOG(LogTemp, Warning, TEXT("Ready to add up children"));
// 		AddUpChildren(CurrentNode, "0");
// 		break;
// 	}
// }
//
// //use nodes to find velocity
// void AKnowledgeGraph::FindManyBodyForce(
// 	AKnowledgeNode* kn,
// 	const FSimpleOctree::FNode& node,
// 	const FOctreeNodeContext3 CurrentContext,
// 	FString node_id
// )
// {
// 	
// 	NodeStrength ns = octree_node_strengths[node_id];
// 	ll("FindManyBodyForce--------------------------------------");
// 	
// 	//if no strength, ignore
// 	//    if(ns.strength == 0)
// 	//        return;
//
// 	const FBoxCenterAndExtent3& CurrentBounds = CurrentContext.Bounds;
// 	ll("CurrentBounds.Center: " + CurrentBounds.Center.ToString());
// 	ll("CurrentBounds.Extent: " + CurrentBounds.Extent.ToString());
// 	
//
// 	
// 	// FVector center = CurrentBounds.Center;
// 	FVector width = CurrentBounds.Extent;
//
//
// 	
// 	FVector dir = ns.direction - kn->GetActorLocation();
//
// 	// Remember that direction is the sum of all the Actor locations of the elements in that note. 
// 	float l = dir.Size() * dir.Size();
//
// 	// if size of current box is less than distance between nodes
// 	// This is used to stop recurring down the tree.
// 	if (width.X * width.X / theta2 < l)
// 	{
// 		//        print("GOING IN HERE");
// 		if (l < distancemax)
// 		{
// 			if (dir.X==0)
// 			{
// 				// Assign a random value   // return (random() - 0.5) * 1e-6;
// 				dir.X = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
// 				// l += x * x;
// 				l += dir.X * dir.X;
// 				
// 			}
// 			if (dir.Y==0)
// 			{
// 				// Assign a random value   // return (random() - 0.5) * 1e-6;
// 				dir.Y = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
// 				// l += x * x;
// 				l += dir.Y * dir.Y;
// 				
// 			}
// 			if (dir.Z==0)
// 			{
// 				// Assign a random value   // return (random() - 0.5) * 1e-6;
// 				dir.Z = (FMath::RandRange(0, 1) - 0.5f) * 1e-6;
// 				// l += x * x;
// 				l += dir.Z * dir.Z;
// 			}
// 			
//
//
// 			
// 			if (l < distancemin)
// 				l = sqrt(distancemin * l);
//
//
// 			if (kn->id == 7)
// 			{
// 				ll("aaaaaaaaaa");
// 				ll(
// 					(dir * ns.strength * alpha / l).ToString(), 2
// 				);
// 				ll("bbbbbbbbbb");
// 			}
// 			//print(FString::SanitizeFloat(ns.strength));
//
// 			// float mult = pow(ns.strength / nodeStrength, 1.0);
// 			kn->velocity += dir * ns.strength * alpha / l;
// 		}
// 		return;
// 	}
//
// 	// if not leaf, get all children
//
// 	// People do we have to check on L bigger than distance max?
// 	// FOREACH_OCTREE_CHILD_NODE3 Will not run if the note is LeaF note. , even if L is bigger than distance max. 
// 	if (!node.IsLeaf() || l >= distancemax)
// 	{
// 		//recurse down this dude
// 		//        print("IM NO LEAF");
// 		//print("NOT A LEAF");
// 		int count = 0;
// 		FOREACH_OCTREE_CHILD_NODE3(ChildRef)
// 		{
// 			if (node.HasChild(ChildRef))
// 			{
// 				FindManyBodyForce(
// 					kn,
// 					*node.GetChild(ChildRef),
// 					CurrentContext.GetChildContext(ChildRef),
// 					node_id + FString::FromInt(count)
// 				);
// 				count++;
// 			}
// 		}
// 	} //if leaf and close, apply elements directly
// 	else if (node.IsLeaf())
// 	{
// 		//print("IM LEAF");
// 		if (l < distancemin)
// 		{
// 			l = sqrt(distancemin * l);
// 		}
// 		for (FSimpleOctree::ElementConstIt ElementIt(node.GetElementIt()); ElementIt; ++ElementIt)
// 		{
// 			const FOctreeElement& Sample = *ElementIt;
// 			if (Sample.MyActor->id != kn->id)
// 			{
// 				dir = Sample.MyActor->GetActorLocation() - kn->GetActorLocation();
// 				l = dir.Size() * dir.Size();
// 				float mult = pow(Sample.MyActor->numberOfConnected, 3.0);
//
//
// 				if (0)
// 				{
// 					if (kn->id == 7 && alpha > 0.2)
// 					{
// 						// print(FString::FromInt(Sample.MyActor->id));
// 						// print((dir * Sample.MyActor->strength * alpha / l * mult).ToString());
// 					}
// 				}
//
// 				kn->velocity += dir * Sample.MyActor->strength * alpha / l * mult;
// 			}
// 		}
// 	}
// }

void AKnowledgeGraph::tttttttttttt()
{
}

// void AKnowledgeGraph::ApplyManyBody(AKnowledgeNode* kn)
// {
// 	FVector dir;
// 	if (kn->id == 7)
// 		ll(
// 			"ApplyManyBody--------------------------------------We are comparing all the other notes with this ID7 note. ");
// 	for (
// 		FSimpleOctree::TConstIterator<> NodeIt(*OctreeData);
// 		NodeIt.HasPendingNodes();
// 		NodeIt.Advance()
// 	)
// 	{
// 		FindManyBodyForce(kn,
// 		                  NodeIt.GetCurrentNode(),
// 		                  NodeIt.GetCurrentContext(),
// 		                  "0"
// 		);
// 		break;
// 	}
// }
//
//
// void FOctreeSematics::SetElementId(FOctreeSematics::FOctree& thisOctree, const FOctreeElement& Element,
//                                    FOctreeElementId3 Id)
// {
// 	((FSimpleOctree&)thisOctree).all_elements.Emplace(Element.MyActor->id, Id);
// }
//
//
// void AKnowledgeGraph::InitOctree(const FBox& inNewBounds)
// {
// 	//OctreeData = new FSimpleOctree(FVector(0.0f, 0.0f, 0.0f), 100.0f);
//
// 	// Check if OctreeData already exists and delete it to prevent memory leaks
// 	if (OctreeData != nullptr) {
// 		delete OctreeData;
// 		OctreeData = nullptr; // Not necessary but a good practice to avoid dangling pointer
// 	}
//
// 	ll("InitOctree--------------------------------------");
//
// 	FVector Vector1 = inNewBounds.GetCenter();
// 	float Max = inNewBounds.GetExtent().GetMax();
// 	ll("Vector1: " + Vector1.ToString() + " Max: " + FString::SanitizeFloat(Max));
//
// 	OctreeData = new FSimpleOctree(Vector1, Max);
// }
//
// FSimpleOctree::FSimpleOctree(const FVector& InOrigin, const float InExtent) :
// 	TOctree3(InOrigin, InExtent)
// {
// }
//
// void AKnowledgeGraph::RemoveElement(int key)
// {
// 	OctreeData->RemoveElement(OctreeData->all_elements[key]);
// 	all_nodes.Remove(key);
// }
//
// void AKnowledgeGraph::AddOctreeElement(const FOctreeElement& inNewOctreeElement)
// {
// 	ll("AddOctreeElement--------------------------------------");
// 	OctreeData->AddElement(inNewOctreeElement);
// }

void AKnowledgeGraph::initializeNodePosition()
{
	if (0)
	{
		for (auto& node : all_nodes)
		{
			float radius = initialRadius * sqrt(node.Key);
			float angle = node.Key * initialAngle;
			FVector init_pos = FVector(cos(angle), sin(angle), tan(angle)) * radius;

			// Remember that the note value stored the actual object. 
			node.Value->SetActorLocation(init_pos, false);
			//        print("init position");
			//        print(node.Value->GetActorLocation().ToString());
			node.Value->velocity = FVector(0, 0, 0);
		}
	}
	else
	{
		// To replicate the node indexing from the original JS function
		for (auto& node : all_nodes)
		{
			int index = node.Key;
			// Calculate index-based radius differently based on the number of dimensions
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

			// Set the initial position of the node Actor
			if (node.Value)
			{
				// Check if pointer is valid
				node.Value->SetActorLocation(init_pos, false);

				ll("index: " + FString::FromInt(index) + " init_pos: " + init_pos.ToString());

				// Log the initial position - Uncomment to use
				// UE_LOG(LogTemp, Warning, TEXT("Init position: %s"), *init_pos.ToString());

				// Set initial velocity to zero
				node.Value->velocity = FVector(0, 0, 0);
			}

			// Increment index for next node
			// index++;
		}
	}
}


void AKnowledgeGraph::CalculateBiasstrengthOflinks()
{
	bool log = true;
	//link forces
	float n = all_nodes.Num();
	float m = all_links.Num();

	for (auto& link : all_links)
	{
		all_nodes[link.Value->source]->numberOfConnected += 1;
		all_nodes[link.Value->target]->numberOfConnected += 1;
	}

	for (auto& link : all_links)
	{
		ll("all_nodes[link.Value->source]->numberOfConnected: " + FString::FromInt(
			   all_nodes[link.Value->source]->numberOfConnected), log);


		float ttttttttttt = all_nodes[link.Value->source]->numberOfConnected +
			all_nodes[link.Value->target]->numberOfConnected;

		float bias = all_nodes[link.Value->source]->numberOfConnected /
			ttttttttttt;


		ll("!!!!!!!!!!!!!!!!!!!!!!!bias: " + FString::SanitizeFloat(bias), log);


		if (biasinitway)
		{
			link.Value->bias = bias > 0.5 ? (1 - bias) * 0.5 + bias : bias * 0.5;
		}
		else
		{
			link.Value->bias = bias;
		}
		ll("!!!!!!!!!!!!!!!!!!!!!!!bias: " + FString::SanitizeFloat(link.Value->bias), log);

		link.Value->strength = 1.0 / fmin(all_nodes[link.Value->source]->numberOfConnected,
		                                  all_nodes[link.Value->target]->numberOfConnected);
	}

	if (0)
	{
		//charge forces
		for (auto& node : all_nodes)
		{
			node.Value->strength = node.Value->strength; // nothing for now
		}
	}

	init = true;
}

void AKnowledgeGraph::AddNode1(int32 id, AKnowledgeNode* kn)
{
	if (!all_nodes.Contains(id))
	{
		kn->id = id;
		kn->strength = nodeStrength;
		all_nodes.Emplace(id, kn);
	}
	else
	{
		ll("Fatal error: Node with ID " + FString::FromInt(id) + " already exists!");
	}
}

//
// void AKnowledgeGraph::AddNode(int32 id, AKnowledgeNode* kn, FVector location)
// {
// 	if (all_nodes.Contains(id))
// 	{
//
// 		FOctreeElement ote;
// 		ote.MyActor = kn;
// 		ote.strength = 1.0; // update with strength
//
// 		
// 		float bounds = 0.0000000001f;
//
// 		if (0)
// 		{
// 			bounds = 0.0000000001f;
// 		}
// 		else
// 		{
// 			bounds = 1.0f;
// 			
// 		}
// 		ote.BoxSphereBounds = FBoxSphereBounds(
// 			location,
// 			FVector(
// 				bounds,
// 				bounds,
// 				bounds
// 				),
// 			bounds
// 			);
// 		
// 		AddOctreeElement(ote);
// 	}
// }

void AKnowledgeGraph::AddEdge(int32 id, int32 source, int32 target)
{
	// UObject* SpawnClass = Cast<UObject>(
	// 	StaticLoadObject(UObject::StaticClass(),
	// 	                 NULL,
	// 	                 TEXT("Blueprint'/Game/cylinder.cylinder'")
	// 	)
	// );
	// UBlueprint* GeneratedObj = Cast<UBlueprint>(SpawnClass);
	AKnowledgeEdge* e;
	if (1)
	{
		UClass* bpClass;
		if (0)
		{
		}
		else
		{
			// bool PlayingInEditor = GEditor ? true : false;


			if (1)
			{
				// This approach works in both play and editor and package game. 

				UClass* loadedClass = StaticLoadClass(UObject::StaticClass(), nullptr,
				                                      TEXT(
					                                      // "Blueprint'/Game/Characters/Enemies/BP_LitchBoss1.BP_LitchBoss1_C'"
					                                      "Blueprint'/Game/kkkkk/NewBlueprint22222.NewBlueprint22222_C'"

				                                      ));
				if (loadedClass)
				{
					e = GetWorld()->SpawnActor<AKnowledgeEdge>(loadedClass);
					// SpawnedActor->TeleportTo(position, rotation.ToOrientationRotator());		
				}
				else
				{
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
		}
	}
	else
	{
	}


	e->source = source;
	e->target = target;
	e->strength = 1; //temp
	e->distance = edgeDistance;
	all_links.Emplace(id, e);
}
