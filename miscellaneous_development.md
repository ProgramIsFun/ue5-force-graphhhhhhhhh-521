This file contains things to be organized

Could be ignored


-----------------------------------------------------

Use compute shaders to do the many body force.
https://github.com/MatthisL/UE5_NBodySimulation




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


----------------------------------------------------

In CPP I want to use one array to represent OC tree. array contained eight elements, each element is either 1. a null pointer Or 2. A 3D data point Or 3.array Of the same type Please help me implement this OC tree. Do not use any pointer because I want to have everything in one array.

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


简单总结就是需要优化一下我写的CPP代码

需要用到虚幻引擎5.21版本

你不是太需要用到虚幻引擎的功能, 当然如果你会的话更加好


因为里面主要还是C++代码,需要您有经验以及最好有算法的经验, 去分析如何优化

实现里面用到了八叉树（Octree）这个数据结构

这个数据结构是用来加速计算的

-----------------------------------------------------------------------------


这个项目就是想用虚幻引擎实现这样的效果
https://vasturiano.github.io/3d-force-graph/example/large-graph/

![image](https://github.com/user-attachments/assets/3fe8f8e9-1589-4bde-8285-d0fd365eb575)






--------------------------------------------------

我已经初步实现了

遇到的问题就是这个力量图的渲染很卡

疑似是代码写得不够好 计算量太大，导致渲染卡顿


1. 打开这个图 https://github.com/ProgramIsFun/ue5-force-graphhhhhhhhh-521/blob/2a1bceea68191bdb67747007ac87b01b514551f0/Content/NewMapppppppp.umap
   如果找不到的话，那就新增一个空的图, 然后把KnowledgeGraph 拖到这个图里面

2. 把KnowledgeGraph      节点数目jnode   设置成300 (反正就是越高越卡  但三百已经是明显卡  在我的系统上少于五十的话还算比较顺)

3. 把KnowledgeGraph      connect_to_previous取消勾选

![image](https://github.com/user-attachments/assets/ee1ed9fe-dc6d-421e-9d99-f05157af9baf)

4. 运行游戏

![image](https://github.com/user-attachments/assets/807c5dde-0d19-46eb-a8c2-95dbfce587b7)


你会发现帧率很低, FPS只有平均不到十。

-----------------------------------------------------------------







需要做的就是用C++去优化一下速度


可以尝试的方法包括使用多线程 或者使用更高效的算法

希望做到的效果是有五千个节点的话, FPS也能平均六十帧以上



如果你觉得能做的话，希望你可以告诉我价格多少

我的微信号是 Ttt888TP (全部都是小写)
qq: 3964305374



