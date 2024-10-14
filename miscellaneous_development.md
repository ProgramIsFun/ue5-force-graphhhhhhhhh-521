This file contains things to be organized

Could be ignored



----------------------------------------------------

Calculate the TICK time for every frame

Provide a better way to check the average FPS and TICK time.

Maybe for different experiments, we can draw a line or a graph in a python application.



-------------------------------------------------

Using parallel For
will have the following error. 
Assertion failed: ComponentsThatNeedEndOfFrameUpdate_OnGameThread.IsValidIndex(ArrayIndex) [File:D:\build\++UE5\Sync\Engine\Source\Runtime\Engine\Private\LevelTick.cpp] [Line: 874]

Possibly because the things that is in the parallel for loop is not thread safe. 





LoginId:cb7510304f4a5445750fcfa49abaf5a3
EpicAccountId:32550fbdab1c434399c0ce8a86d54173

Assertion failed: !IsValid(Component) || Component->GetMarkedForEndOfFrameUpdateState() == EComponentMarkedForEndOfFrameUpdateState::MarkedForGameThread [File:D:\build\++UE5\Sync\Engine\Source\Runtime\Engine\Private\LevelTick.cpp] [Line: 1138]

UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor
UnrealEditor
UnrealEditor
UnrealEditor
UnrealEditor
UnrealEditor
kernel32
ntdll

-----------------------------------------------------

Use compute shaders to do the many body force.
https://github.com/MatthisL/UE5_NBodySimulation

----------------------------------------------------

Trying to use a single array to contain the whole OC tree 

-------------------------------------------------------------

Look at the OC tree implementation. 

V:\UNREALLLINSTALLLL\UE_5.2\Engine\Source\Runtime\Core\Public\Math

--------------------------------------------------------------


Deal with importing a graph

Check the basic format of a very simple graph data. 


---------------------------------------------------

It seems there are too much overhead for every individual notes to be an actor. 
I will change every note to a text render component.

If there is snow on click listener function on each of these components, 
I can just use the following thing to detect what object is clicked. 

FHitResult Hit;    
GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, End, ECC_Visibility, Params);
----------------------------------------------------


---------------------------------------------------------------------------------

In CPP I want to use one array to represent OC tree. array contained eight elements, each element is either 1. a null pointer Or 2. A 3D data point Or 3.array Of the same type Please help me implement this OC tree. Do not use any pointer because I want to have everything in one array.


