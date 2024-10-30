


I don't understand why, but sometimes if gpu output A ray size is zero. You delete that actor in the map and change it to simulation engine actor. 
And then it will work as normal again. 

## Notice

This project is Mainly in git@gitee.com:nameeeee/aaaaaaaaa.git

The project in Other git platform is just a mirror of the project in gitee.

git push -u origin main521:main521github

git push -u origin3gitee main521:main521

## What is this project? 


A Force-Directed Graph, or Force-Based Graph, is a type of layout commonly used in a variety of application areas: network visualization, large graph visualization, knowledge representation, system management, or mesh visualization.

This contains an implementation of the force directed graph in unreal engine 5.

replicate the result of the popular library https://github.com/vasturiano/3d-force-graph.

![image](https://github.com/user-attachments/assets/4e648745-8ee3-4fc7-9008-fd1d53785711)


## Main reference of this project

Dependency link

https://vasturiano.github.io/3d-force-graph/example/large-graph/ -> https://github.com/vasturiano/three-forcegraph -> https://github.com/vasturiano/d3-force-3d -> https://github.com/vasturiano/d3-octree



## To do list

### 1

#### Problem description

The rendering part is very laggy when the number of nodes reach 100.  

To reproduce the issue,  
Create an empty level and then 
drag the cpp class KnowledgeGraph to the level.


Then set the number of nodes to 300  (jnode).

connect_to_previous should be unchecked.

![image](https://github.com/user-attachments/assets/ee1ed9fe-dc6d-421e-9d99-f05157af9baf)



Then run the game. 

You will find that the frame rate is very low, less than 10 fps.




#### Possible solution 
Number 5 is chosen. 

1. Need to time the exact time each function in order to find the bottleneck
2. Use the profiler to find the bottleneck
3. The reason of the laggy rendering is possibly because it takes too much time to calculate the charge force.
  my implementation is actually Reference from the javascript implementation https://github.com/vasturiano/three-forcegraph.
  the javascript version could render 1000 or few thousand nodes in real time smoothly.
  In the javascript implementation of the OcTree , there is no Pointer. The location information of all the nodes
  is directly stored in an array With eight elements, Each elements Is either NULL Or an array with eight elements, so on and so forth.
  Perhaps this is the reason why the javascript implementation is faster than mine.
  The way that I implement the tree is basically every node have 8 pointers, and every pointer Is either a null pointer or a pointer to another tree node. I am unsure whether this structure will prolong the calculation.  

4. Use some multi threading method to speed up the calculation

5. Use compute shader   


### 2

Applying an example in Github. 
Which is a shader to calculate Many body force in two dimensional. 
We want to change it to three dimensional. 

from Many examples In Google searches, we observe that the GPU calculation Could be Trigger in two ways. 
1. OnPostResolvedSceneColorHandle =
   RendererModule->
   GetResolvedSceneColorCallbacks()
   .AddRaw(this,
   &XXXXXXXXXXXX::YYYYYYYYYY_RenderThread
   ); 
2. manually triggered By using dispatch, 
   Passing in a callback function so that when gpu calculation is done, the callback function will be called.

I wonder if there are a way can do it in a synchronous block way,
https://github.com/MatthisL/UE5_NBodySimulation/issues/1,
But I will assume now it is very difficult and not efficient to do it in this way. 

So it seems we only have two main ways. But since I do not know how to use The callback function properly.
I will try to use the first way. 
Because if we want to use the first way, I suggest we only have one shader, because if we have multiple shaders, 
I'm not sure how they could execute in the correct order. 
Because I want to ensure that we always apply the link force first, 
and then the many body force, 
and then the center force.

But how could we compute the 3 forces in a single shader?
In this function in this commit https://github.com/ProgramIsFun/ue5-force-graphhhhhhhhh-521/blob/50ce2f2684cabef0576102ec612634937be007d5/Plugins/NBodySimShader/Shaders/Private/NBodySim.usf#L69
Only the many body force is computed. 
Perhaps we could add the link force and the center force in this function.
However, because the way that we are dispatching the shader, 
SV_DispatchThreadID Of any specific thread is likely to be not larger than the number of bodies. 
The number of links in the graph could be much larger than number of bodies. 
So a thread With a given SV_DispatchThreadID, 
Could be responsible for computing all the connected nodes of a specific node With that ID. 
So let's say the node ID 3 Has a lot of connecting nodes, 
Then the thread With SV_DispatchThreadID 3 will be responsible for computing all the connected nodes of node 3.
This might be a bad idea, because some thread might be computing with more times than 
other threads on the ID that have very less connected nodes. 

For the center force, This could be ignored for now. 





### 3 

Try to run the shaders when there are only two elements to see whether they compute the value correctly. 

Perhaps need to add additional debugger of the shader, because sometimes it returns NAN. 

### 4

add the link force 

Assuming the following is the correct workflow. 

We need to be able to retrieve the position and also the velocity array. 
we will change the position and velocity And write it as a parameter and update the shader.  

The question is shall we update the velocity thing to the shader 
before We get the position, or after we get the position. 

Because the shader will compute the position force according to the velocity, perhaps that should be done before. 

And because the shader will update The position according to the velocity, perhaps we should also pass The Alpha value to the shader.


does the shade only Once, if we set the TICK interval to be very large.

The result is If we set the TICK interval to maybe perhaps 1 second, it runs a few hundred times already. 
The possible reasons is The callback function is called every frame.  So the shader is called every frame.
Right now, we could assume that if we do not set the interval, and then it will run exactly once between each tick.

Second thing is Let's say we update a variable. Does the result immediately result from the update value or the previous update





Let's also test can we overwrite the position directly in the shader
It seems pretty hard because in every initialized phrase, if the buffer is already there, we will not overwritten it. 
Perhaps could reference the following project. 
- https://github.com/UE4-OceanProject/ComputeShaderDev


https://forums.unrealengine.com/t/loading-data-to-from-structured-buffer-compute-shaders/470083/2

The above seem pretty complicated. 
How about we just compute the link force also in the same shader? 

GPT give me two suggestions on computing the link force.


StructuredBuffer<uint> LinkIndices;                // Flatten 2D array of indices
const uint MaxLinksPerBody;                        // Maximum number of linked bodies per node

// Link forces calculation
for (uint j = 0; j < MaxLinksPerBody; j++)
{
        uint LinkedBodyIndex = LinkIndices[ID.x * MaxLinksPerBody + j];

        Check for sentinel value indicating no more connections
        
        if (LinkedBodyIndex == UINT_MAX) 
            break;  // This stops processing further links for this node

        float2 LinkDirection = normalize(Positions[LinkedBodyIndex] - Positions[ID.x]);
        float LinkStrength = 1.0f;  // Placeholder strength
        Acceleration += LinkDirection * LinkStrength;
}



Offset/Count Buffer: Each entry contains a starting index and the count of connections for the corresponding body.
Links Buffer: A flat buffer that contains all the links in a single array, sequenced as per the offsets and counts indicated in the first buffer.
Shader Code Adjustment:
StructuredBuffer<uint2> LinkInfo;       // Holds pairs (offset, count) for each body
StructuredBuffer<uint> LinkIndices;     // Flat array containing all links

// In your compute shader
for (uint j = 0; j < LinkInfo[ID.x].y; j++) // LinkInfo[ID.x].y gives the count of links
{
uint index = LinkInfo[ID.x].x + j;  // Starting index + offset
uint LinkedBodyIndex = LinkIndices[index];
// Calculate forces similar to previous examples
}





It take too much time for generating too much actor for each link. 
We use components instead of an actor for each link. 



This perhaps explain why flatten 1D array is better than 2D array. 
https://stackoverflow.com/questions/19759343/cuda-is-it-better-to-use-m-for-2d-static-arrays-or-flatten-them-to-m



To prevent race condition when calculating the link force in the shader, perhaps each thread will only deal with the change of position or velocity of that node only. 
So one link could affect 2 Threats. 

So we compute a flattened one D away, which store the bias of each link. 










## Reference

### https://github.com/thomaswall/ue4-force-graph

I regard this as an improved version of https://github.com/thomaswall/ue4-force-graph

It is improved in the following ways
1. It is Using the latest version of the unreal engine 5 instead of the unreal engine 4.24
2. It is using a different implementation of the charge force
3. It corrects some of the Mistakes such as calculating the bias of the link.

It is worse in the following ways
1. The implementation of the charge force, which is the many body force, use a custom implementation of the OCtree, Which seems to be slower than the library version of the unreal engine.






## A introduction to unreal engine 5

1.Characters. Pawns, and Actors: What's the Difference?

They are all CPP classes, which have a very detailed implementation by the epic games in the source code.

When working with Unreal Engine, you'll often encounter the terms Character, Pawn, and Actor. These classes are fundamental building blocks for creating interactive elements within your game world. Here's a brief overview of each class and how they differ:

In Unreal Engine, choosing between Characters, Pawns, and Actors is an important decision that depends on the needs of your game or project. Each has its own unique use case and capabilities:

Characters: Use the Character class when you need an entity that can walk, jump, or have built-in complex movement capabilities. It's useful for player-controlled avatars or NPCs that require dynamic interaction with the game world.

Pawns: Pawns are ideal when you need a controllable entity but do not require the full suite of movement capabilities provided by the Character class. Pawns offer a more lightweight and flexible option for entities that need user input but have custom or unconventional forms of movement.

Actors: Actors are the most general class and should be used for non-controlled objects in your game world. This could range from dynamic objects like moving platforms, to static scenery elements such as trees and buildings. Actors are highly versatile and form the base object for most elements within the game world.

Choosing the right class depends on the functionality you need and how you plan to interact with the object in your game.

2.Where to put the source code?

The source code should be put in the source folder of the project. In a lot of complicated projects, they want to separate the header files and the source files Into two different directories in the source folder.

But in this project, there is no such separations. All the header files and CPP are put in the same folder.

3.How is this project implementing force directed graph work in unreal engine 5?

Basically, it is a cpp class  and it inherits from the Pawns class.

Source/ue544_blankkkkkkkkkk/KnowledgeGraph.cpp

Source/ue544_blankkkkkkkkkk/KnowledgeGraph.h

Usually every actor and all its child's class have a constructor and a destructor, begin play, and tick function.

In the beginplay function, we will initialize everything that we need.

In the tick function, we will update the position of the nodes and the links.

So how to put this in the map that we want to play? We can drag the KnowledgeGraph From the content browser to the map in the middle. And then you see the actor is in the map, because you can look at the right hand side. There is a level editor. And you can see the actor in the world outliner.

![image](https://github.com/user-attachments/assets/2042450f-0d52-4c6f-97e2-a8dc2973de14)


4.How to open different maps?

You can open different maps by clicking the file in the content browser.

Content/CreatePlanetsSSS/NewMapPPPPPPP.umap   is the map that looks like in the universe.

5.Usual workflow

Every time we modify the source code, we need to recompile the project.

Because sometimes we want to change the default values of some member variables of some class,
In order to speed up the workflow, we can add a UPROPERTY() macro in front of the member variable.

https://benui.ca/unreal/uproperty/


