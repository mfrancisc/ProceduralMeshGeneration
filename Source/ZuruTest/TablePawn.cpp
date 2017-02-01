#include "ZuruTest.h"
#include "TablePawn.h"

// Sets default values
ATablePawn::ATablePawn()
{
    // Set this Pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    MeshComponent = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("ProceduralMesh"));
    MeshComponent->bShouldSerializeMeshData = false;
    MeshComponent->SetupAttachment(RootComponent);
    //MeshComponent->bUseComplexAsSimpleCollision = false;

    // Create a camera and a visible object
    UCameraComponent *OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
    // Attach our camera and visible object to our root component. Offset and rotate the camera.
    OurCamera->SetupAttachment(RootComponent);
    OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
    OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
   ConstructorHelpers::FObjectFinder<UMaterialInterface> ReferenceVariable (TEXT("Material'/Game/StarterContent/Materials/M_Wood_Pine.M_Wood_Pine'")); 
   if(ReferenceVariable.Succeeded())
    {
        Material = ReferenceVariable.Object;
    }

    UE_LOG(LogTemp, Warning, TEXT("Created Table Pawn "));
}

// Called when the game starts or when sPawned
void ATablePawn::BeginPlay()
{
    Super::BeginPlay();
    GenerateMesh();
}

// Called every frame
void ATablePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATablePawn::SetupMeshBuffers()
{
    int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
    Vertices.AddUninitialized(VertexCount);
    Triangles.AddUninitialized(6 * 2 * 3); // 2x triangles per cube side, 3 verts each
}

void ATablePawn::GenerateMesh()
{
    // The number of vertices or polygons wont change at runtime, so we'll just allocate the arrays once
    if (!bHaveBuffersBeenInitialized)
    {
        SetupMeshBuffers();
        bHaveBuffersBeenInitialized = true;
    }

    FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    
    FVector OffsetPos = FVector(0.0f, 0.0f, 0.0f);
    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    MeshComponent->ClearAllMeshSections();
    MeshComponent->CreateMeshSection(0, Vertices, Triangles, BoundingBox, true, EUpdateFrequency::Frequent);
    MeshComponent->SetMaterial(0, Material);

    // create 1° leg
    OffsetPosFirstLeg = FVector(offsetDiffX, offsetDiffY, -offsetDiffZ);
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosFirstLeg);
    MeshComponent->CreateMeshSection(1, Vertices, Triangles, BoundingBox, true, EUpdateFrequency::Frequent);
    MeshComponent->SetMaterial(1, Material);

    // create 2° leg
    OffsetPosSecondLeg = FVector(-offsetDiffX, offsetDiffY, -offsetDiffZ);
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->CreateMeshSection(2, Vertices, Triangles, BoundingBox, true, EUpdateFrequency::Frequent);
    MeshComponent->SetMaterial(2, Material);

    // create 3° leg
    OffsetPosThirdLeg = FVector(offsetDiffX, -offsetDiffY, -offsetDiffZ);
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosThirdLeg);
    MeshComponent->CreateMeshSection(3, Vertices, Triangles, BoundingBox, true, EUpdateFrequency::Frequent);
    MeshComponent->SetMaterial(3, Material);

    // create 4° leg
    OffsetPosFourthLeg = FVector(-offsetDiffX, -offsetDiffY, -offsetDiffZ);
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosFourthLeg);
    MeshComponent->CreateMeshSection(4, Vertices, Triangles, BoundingBox, true, EUpdateFrequency::Frequent);
    MeshComponent->SetMaterial(4, Material);
}

void ATablePawn::GenerateCube(TArray<FRuntimeMeshVertexSimple> &InVertices, TArray<int32> &InTriangles, FVector InSize, FVector OffsetPos)
{
    // Calculate a half offset so we get correct center of object
    float OffsetX = InSize.X / 2.0f;
    float OffsetY = InSize.Y / 2.0f;
    float OffsetZ = InSize.Z / 2.0f;

    // Define the 8 corners of the cube
    FVector p0 = FVector(OffsetX + OffsetPos.X, OffsetY + OffsetPos.Y, -OffsetZ + OffsetPos.Z);
    FVector p1 = FVector(OffsetX + OffsetPos.X, -OffsetY + OffsetPos.Y, -OffsetZ + OffsetPos.Z);
    FVector p2 = FVector(OffsetX + OffsetPos.X, -OffsetY + OffsetPos.Y, OffsetZ + OffsetPos.Z);
    FVector p3 = FVector(OffsetX + OffsetPos.X, OffsetY + OffsetPos.Y, OffsetZ + OffsetPos.Z);
    FVector p4 = FVector(-OffsetX + OffsetPos.X, OffsetY + OffsetPos.Y, -OffsetZ + OffsetPos.Z);
    FVector p5 = FVector(-OffsetX + OffsetPos.X, -OffsetY + OffsetPos.Y, -OffsetZ + OffsetPos.Z);
    FVector p6 = FVector(-OffsetX + OffsetPos.X, -OffsetY + OffsetPos.Y, OffsetZ + OffsetPos.Z);
    FVector p7 = FVector(-OffsetX + OffsetPos.X, OffsetY + OffsetPos.Y, OffsetZ + OffsetPos.Z);

    // Now we create 6x faces, 4 vertices each
    int32 VertexOffset = 0;
    int32 TriangleOffset = 0;
    FPackedNormal Normal = FPackedNormal::ZeroNormal;
    FPackedNormal Tangent = FPackedNormal::ZeroNormal;

    // Front (+X) face: 0-1-2-3
    Normal = FVector(1, 0, 0);
    Tangent = FVector(0, 1, 0);
    BuildQuad(InVertices, InTriangles, p0, p1, p2, p3, VertexOffset, TriangleOffset, Normal, Tangent);

    // Back (-X) face: 5-4-7-6
    Normal = FVector(-1, 0, 0);
    Tangent = FVector(0, -1, 0);
    BuildQuad(InVertices, InTriangles, p5, p4, p7, p6, VertexOffset, TriangleOffset, Normal, Tangent);

    // Left (-Y) face: 1-5-6-2
    Normal = FVector(0, -1, 0);
    Tangent = FVector(1, 0, 0);
    BuildQuad(InVertices, InTriangles, p1, p5, p6, p2, VertexOffset, TriangleOffset, Normal, Tangent);

    // Right (+Y) face: 4-0-3-7
    Normal = FVector(0, 1, 0);
    Tangent = FVector(-1, 0, 0);
    BuildQuad(InVertices, InTriangles, p4, p0, p3, p7, VertexOffset, TriangleOffset, Normal, Tangent);

    // Top (+Z) face: 6-7-3-2
    Normal = FVector(0, 0, 1);
    Tangent = FVector(0, 1, 0);
    BuildQuad(InVertices, InTriangles, p6, p7, p3, p2, VertexOffset, TriangleOffset, Normal, Tangent);

    // Bottom (-Z) face: 1-0-4-5
    Normal = FVector(0, 0, -1);
    Tangent = FVector(0, -1, 0);
    BuildQuad(InVertices, InTriangles, p1, p0, p4, p5, VertexOffset, TriangleOffset, Normal, Tangent);
}

void ATablePawn::BuildQuad(TArray<FRuntimeMeshVertexSimple> &InVertices, TArray<int32> &InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, int32 &VertexOffset, int32 &TriangleOffset, FPackedNormal Normal, FPackedNormal Tangent)
{
    int32 Index1 = VertexOffset++;
    int32 Index2 = VertexOffset++;
    int32 Index3 = VertexOffset++;
    int32 Index4 = VertexOffset++;
    InVertices[Index1].Position = BottomLeft;
    InVertices[Index2].Position = BottomRight;
    InVertices[Index3].Position = TopRight;
    InVertices[Index4].Position = TopLeft;
    InVertices[Index1].UV0 = FVector2D(0.0f, 1.0f);
    InVertices[Index2].UV0 = FVector2D(1.0f, 1.0f);
    InVertices[Index3].UV0 = FVector2D(1.0f, 0.0f);
    InVertices[Index4].UV0 = FVector2D(0.0f, 0.0f);
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index2;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index1;
    InTriangles[TriangleOffset++] = Index3;
    InTriangles[TriangleOffset++] = Index4;
    // On a cube side, all the vertex normals face the same way
    InVertices[Index1].Normal = InVertices[Index2].Normal = InVertices[Index3].Normal = InVertices[Index4].Normal = Normal;
    InVertices[Index1].Tangent = InVertices[Index2].Tangent = InVertices[Index3].Tangent = InVertices[Index4].Tangent = Tangent;
}

// Called to bind functionality to input
void ATablePawn::SetupPlayerInputComponent(class UInputComponent *InputComp)
{

    UE_LOG(LogTemp, Warning, TEXT("Create Input component "));
    check(InputComponent);
    Super::SetupPlayerInputComponent(InputComp);
    
    // TODO use mouse to resize objects
    InputComp->BindAction("ResizeAction", EInputEvent::IE_Pressed, this, &ATablePawn::ResizeAction);
    
    // resize objects using keyboard
    InputComp->BindAxis("ResizeUpX", this, &ATablePawn::ResizeUpX);
    InputComp->BindAxis("ResizeDownX", this, &ATablePawn::ResizeDownX);
    InputComp->BindAxis("ResizeLeftY", this, &ATablePawn::ResizeLeftY);
    InputComp->BindAxis("ResizeRightY", this, &ATablePawn::ResizeRightY);
}

void ATablePawn::ResizeUpX(float axisValue)
{
    Size.X = Size.X - axisValue*2;
    FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    OffsetPosT.X = OffsetPosT.X+axisValue;
    // Initial table min size 
    if(InitialOffsetPosT.X < OffsetPosT.X && Size.X < InitialSize.X){
        OffsetPosT.X = InitialOffsetPosT.X;
        Size.X = InitialSize.X;
        return;
    }

    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    MeshComponent->UpdateMeshSection(0, Vertices);
    
    // Move 2° leg up
    OffsetPosSecondLeg.X = OffsetPosSecondLeg.X+axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->UpdateMeshSection(2, Vertices);
     
     // Move 4° leg up
    OffsetPosFourthLeg.X = OffsetPosFourthLeg.X+axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosFourthLeg);
    MeshComponent->UpdateMeshSection(4, Vertices);
}

void ATablePawn::ResizeDownX(float axisValue)
{
    // Resize Table
    Size.X = Size.X + axisValue*2;
    FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    OffsetPosT.X = OffsetPosT.X-axisValue;
    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    MeshComponent->UpdateMeshSection(0, Vertices);

    // Move 2° leg down
    OffsetPosSecondLeg.X = OffsetPosSecondLeg.X-axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->UpdateMeshSection(2, Vertices);

    // Move 4° leg down
    OffsetPosFourthLeg.X = OffsetPosFourthLeg.X-axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosFourthLeg);
    MeshComponent->UpdateMeshSection(4, Vertices);
}

void ATablePawn::ResizeLeftY(float axisValue)
{
    Size.Y = Size.Y - axisValue*2;
    FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    OffsetPosT.Y = OffsetPosT.Y-axisValue;
     // Initial table min size 
    if(OffsetPosT.Y < InitialOffsetPosT.Y  && Size.Y < InitialSize.Y){
        OffsetPosT.Y = InitialOffsetPosT.Y;
        Size.Y = InitialSize.Y;
        return;
    }

    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    MeshComponent->UpdateMeshSection(0, Vertices);

     // Move 2° leg left
    OffsetPosThirdLeg.Y = OffsetPosThirdLeg.Y-axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosThirdLeg);
    MeshComponent->UpdateMeshSection(3, Vertices);

    // Move 2° leg left
    OffsetPosSecondLeg.Y = OffsetPosSecondLeg.Y-axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->UpdateMeshSection(2, Vertices);
}

void ATablePawn::ResizeRightY(float axisValue)
{
    Size.Y = Size.Y + axisValue*2;
    FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    OffsetPosT.Y = OffsetPosT.Y+axisValue;
    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    // This version updates only the vertices
    MeshComponent->UpdateMeshSection(0, Vertices);

     // Move 3° leg right
    OffsetPosThirdLeg.Y = OffsetPosThirdLeg.Y+axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosThirdLeg);
    MeshComponent->UpdateMeshSection(3, Vertices);

     // Move 2° leg right
    OffsetPosSecondLeg.Y = OffsetPosSecondLeg.Y+axisValue*2;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->UpdateMeshSection(2, Vertices);
}

void ATablePawn::ResizeAction()
{
    UE_LOG(LogTemp, Warning, TEXT("Resize Right action "));
    PossesClicked();
}

void ATablePawn::PossesClicked()
{
    APlayerController *PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController != nullptr)
    {
        // Get the coordinates of the mouse from our controller
        float LocationX;
        float LocationY;
        PlayerController->GetMousePosition(LocationX, LocationY);

        // Do a trace and see if there the position intersects something in the world
        UE_LOG(LogTemp, Warning, TEXT("Maouse locations x: %f , y: %f"), LocationX, LocationY);
        FVector2D MousePosition(LocationX, LocationY);
        FHitResult HitResult;
        const bool bTraceComplex = false;
        if (PlayerController->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, bTraceComplex, HitResult) == true)
        {
            UE_LOG(LogTemp, Warning, TEXT("hit point x: %f , y: %f"), HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y);
              // Get the current location  
  
          
            
            // If the Pawn we intersected with is a controller posses it
            APawn *ClickedPawn = Cast<APawn>(HitResult.GetActor());
            if (ClickedPawn != nullptr)
            {

                // TODO scale Pawn based on mouse dragging

                UE_LOG(LogTemp, Warning, TEXT("hit point in Pawn"));
                // Unposses ourselves
                PlayerController->UnPossess();
                // Posses the controller we clicked on
                PlayerController->Possess(ClickedPawn);
            }
        }
    }
}

