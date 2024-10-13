This file contains things to be organized

Could be ignored


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



----------------------------------------------------

Trying to use a single array to contain the whole OC tree 



--------------------------------------------------------------

Check weather parallel for will also crash in 5.4 version 

-------------------------------------------------------------

Look at the OC tree implementation. 

V:\UNREALLLINSTALLLL\UE_5.2\Engine\Source\Runtime\Core\Public\Math

--------------------------------------------------------------


Deal with importing a graph


Check the basic format of a very simple graph data. 

----------------------------------------------------

Calculate the TICK time for every frame

Provide a better way to check the average FPS and TICK time. 

Maybe for different experiments, we can draw a line or a graph in a python application. 


---------------------------------------------------

It seems there are too much overhead for every individual notes to be an actor. 
I will change every note to a text render component.

If there is snow on click listener function on each of these components, 
I can just use the following thing to detect what object is clicked. 

FHitResult Hit;    
GetWorld()->LineTraceSingleByChannel(Hit, WorldLocation, End, ECC_Visibility, Params);
----------------------------------------------------





I am in no way a professional in Unreal - this project is the only thing I ever done. So my total experience with Unreal is around one week. So I don't think I will be able to help you, you'll have to do your research.
But I did dig a bit into optimization during that week. Actually without the n-body physics around 20000k particles were rendered smoothly on a cheap laptop with a built-in graphics. The key to this is using "instanced static mesh" - google it, or look up what I did (I honestly don't remember what it was). The other thing - is n-body problem is always computation-intensive. I did parallelize the computation inner loop - you can also look it up in the code. Unreal has that stuff as loop execution policy or something like that. But if you really need to get the most for n-body problem, you have to implement some spatial hashing or octo-tree or some n-body specific stuff that allows you to not calculate each body-to-body interaction every frame. I think that is all I can help you with, good luck!

@aleksandrbazhin
Owner
aleksandrbazhin commented 1 hour ago
This is the parallelization part for outer, not inner loop https://github.com/aleksandrbazhin/Unreal_2D_NBodySim/blob/707fee4488fdfbb8d8b9258b74927d3b8fe1d748/Source/NBodySim/BodyManager.cpp#L52C5-L52C16
And instanced static mesh means you only load the mesh to the GPU once, and reuse it N times.
My machine ran ~2500 bodies at around 60 FPS with n-body gravity, and ~20000 without the gravity.

-----------------------------------------------------------------------







- https://github.com/profdambledore/UE4-N_Body_Simulation
    - https://profdambledore.artstation.com/projects/J9WXnz
    - 4.23
    - Does not implement OCTREE
    - The plug in could be disabled to start the project。 
- https://github.com/aleksandrbazhin/Unreal_2D_NBodySim/tree/master
    - https://www.youtube.com/watch?v=4J0xdB-CSnU&ab_channel=ShallowDive
    - The whole project could be converted to 5.3 unreal engine directly. 
    - In the function GravityStep, Which is supposed to update the velocity of all the Particle use a O(N Square) approach.  
      - ```void ABodyManager::GravityStep(float DeltaTime)
        {
        ParallelFor(Bodies.Num(), [&] (int32 Index) {
        FVector2D Acceleration(0.0f, 0.0f);
        for (const FBodyEntity& AffectingBody: Bodies) {
        if (AffectingBody.Index == Bodies[Index].Index)
        continue; // exclude self
        float Distance = FVector2D::Distance(
        Bodies[Index].Position,
        AffectingBody.Position
        );
        Distance = FMath::Max(
        Distance, MinimumGravityDistance
        ); // avoids division by zero
        Acceleration += AffectingBody.Mass /
        Distance * G /
        Distance *
        (AffectingBody.Position - Bodies[Index].Position) /
        Distance;
        }
        Bodies[Index].Velocity += Acceleration * DeltaTime ;
        });
        }
        ``` 

    - However, parallelfor is used. 
- https://github.com/martinpaule/DissertationProject/tree/main

---------------------------------------------------------------------------------

In CPP I want to use one array to represent OC tree. array contained eight elements, each element is either 1. a null pointer Or 2. A 3D data point Or 3.array Of the same type Please help me implement this OC tree. Do not use any pointer because I want to have everything in one array.


