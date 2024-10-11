This file contains things to be organized

Could be ignored


- https://github.com/profdambledore/UE4-N_Body_Simulation
    - https://profdambledore.artstation.com/projects/J9WXnz
    - 4.23
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



In CPP I want to use one array to represent OC tree. array contained eight elements, each element is either 1. a null pointer Or 2. A 3D data point Or 3.array Of the same type Please help me implement this OC tree. Do not use any pointer because I want to have everything in one array.


