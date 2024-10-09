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
The rendering part is very laggy when the number of nodes reach 100.  Speed up the rendering part and make it less laggy.
The member variable is declared in https://github.com/everythingallaccount/ue4-force-graph-backupppp-11111/blob/3562cb5f74af1cfd5ff021711dbc703e5a4ead61/Source/ue544_blankkkkkkkkkk/KnowledgeGraph.h#L95-L95
Because it is Uproperty, you can directly change the value in the editor.




When solving this problem, we could just use the level https://github.com/everythingallaccount/ue4-force-graph-backupppp-11111/blob/3562cb5f74af1cfd5ff021711dbc703e5a4ead61/Content/Minimal_Default11111.umap
Which is just an empty world with only the graph in it.

And also in the process of solving this problem, we could have this variable set https://github.com/everythingallaccount/ue4-force-graph-backupppp-11111/blob/3562cb5f74af1cfd5ff021711dbc703e5a4ead61/Source/ue544_blankkkkkkkkkk/KnowledgeGraph2.cpp#L39
So that the Else part of the if else is executed.
so that every node will be connected to the previous node, Which eliminates any randomness To provide The same graph every time.


#### Possible solution
  - Check if the charge force is implemented correctly 
  - Need to time the exact time each function in order to find the bottleneck
  - Use the profiler to find the bottleneck
  - The reason of the laggy rendering is possibly because it takes too much time to calculate the charge force.
    my implementation is actually Reference from the javascript implementation https://github.com/vasturiano/three-forcegraph.
    the javascript version could render 1000 or few thousand nodes in real time smoothly.
    In the javascript implementation of the OcTree , there is no Pointer. The location information of all the nodes
    is directly stored in an array With eight elements, Each elements Is either NULL Or an array with eight elements, so on and so forth.
    Perhaps this is the reason why the javascript implementation is faster than mine.
    The way that I implement the tree is basically every node have 8 pointers, and every pointer Is either a null pointer or a pointer to another tree node. I am unsure whether this structure will prolong the calculation.  
  
### 2

When node equal 5, The traversal seems have some difference with the javascript implementation. 

```

Traverse the tree And calculate velocity on this Actor Kn, nodekey: -0
--------------------Right now, dealing with:  Lower boundX=-8.000 Y=-17.000 Z=0.000 Upper bound X=24.000 Y=15.000 Z=32.000
Prepare to call the call back functions with this node. 
-----------------
-8.0 -17.0 0.0 24.0 15.0 32.0
22222222222222222 You need to return false here. 
i2
Lower boundX=-8.000 Y=-1.000 Z=0.000 Upper boundX=8.000 Y=15.000 Z=16.000
i0
Lower boundX=-8.000 Y=-17.000 Z=0.000 Upper boundX=8.000 Y=-1.000 Z=16.000
--------------------Right now, dealing with:  Lower boundX=-8.000 Y=-17.000 Z=0.000 Upper bound X=8.000 Y=-1.000 Z=16.000
Prepare to call the call back functions with this node. 
-----------------
-8.0 -17.0 0.0 8.0 -1.0 16.0
velocity Updated: X=3.416 Y=10.925 Z=-2.224
11111111111111 Early termination. 
--------------------Right now, dealing with:  Lower boundX=-8.000 Y=-1.000 Z=0.000 Upper bound X=8.000 Y=15.000 Z=16.000
Prepare to call the call back functions with this node. 
-----------------
-8.0 -1.0 0.0 8.0 15.0 16.0
22222222222222222 You need to return false here. 
i6
Lower boundX=-8.000 Y=7.000 Z=8.000 Upper boundX=0.000 Y=15.000 Z=16.000
i4
Lower boundX=-8.000 Y=-1.000 Z=8.000 Upper boundX=0.000 Y=7.000 Z=16.000
i2
Lower boundX=-8.000 Y=7.000 Z=0.000 Upper boundX=0.000 Y=15.000 Z=8.000



Right now, dealing with -8 -17 0 24 15 32
visit.js:18 Prepare to call the call back functions with this node. 
manyBody.js:13 -----------------
manyBody.js:13 lower bound: -8 -17 0   upper bound: 24 15 32
manyBody.js:13 2222222222222222222 Need to return false here. 
visit.js:18 2 -8 -1 0 8 15 16
visit.js:18 0 -8 -17 0 8 -1 16
visit.js:18 Right now, dealing with -8 -17 0 8 -1 16
visit.js:18 Prepare to call the call back functions with this node. 
manyBody.js:13 -----------------
manyBody.js:13 lower bound: -8 -17 0   upper bound: 8 -1 16
manyBody.js:13 The following is updated velocity.  node.vx: 3.4155877922903866 node.vy: 10.924374461464797 node.vz: -2.223548889468486
manyBody.js:13 11111111111111111111 Early termination. Returning true. 
visit.js:18 Right now, dealing with -8 -1 0 8 15 16
visit.js:18 Prepare to call the call back functions with this node. 
manyBody.js:13 -----------------
manyBody.js:13 lower bound: -8 -1 0   upper bound: 8 15 16
manyBody.js:13 2222222222222222222 Need to return false here. 
visit.js:18 6 -8 7 8 0 15 16
visit.js:18 4 -8 -1 8 0 7 16
visit.js:18 3 0 7 0 8 15 8
visit.js:18 Right now, dealing with 0 7 0 8 15 8





```

### 3
Adding functions such as import graph or export graph in a Json format. 

## A shot introduction to unreal engine 5

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





## Reference

### https://github.com/thomaswall/ue4-force-graph

I regard this as an improved version of https://github.com/thomaswall/ue4-force-graph

It is improved in the following ways
1. It is Using the latest version of the unreal engine 5 instead of the unreal engine 4.24
2. It is using a different implementation of the charge force
3. It corrects some of the Mistakes such as calculating the bias of the link.

It is worse in the following ways
1. The implementation of the charge force, which is the many body force, use a custom implementation of the OCtree, Which seems to be slower than the library version of the unreal engine.
