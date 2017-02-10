#include "ZuruTest.h"
#include "TableActor.h"
#include "ChairActor.h"

ATableActor::ATableActor()
{
    // Set this Actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
	
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    MeshComponent = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("ProceduralMesh"));
    MeshComponent->bShouldSerializeMeshData = false;
	MeshComponent->SetupAttachment(RootComponent);
	
   // Create a camera and a visible object
   UCameraComponent *OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
   // Attach our camera and visible object to our root component. Offset and rotate the camera.
   OurCamera->SetupAttachment(RootComponent);
   OurCamera->SetRelativeLocation(FVector(-250.0f, 0.0f, 250.0f));
   OurCamera->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
  
   // create material for mesh component
   ConstructorHelpers::FObjectFinder<UMaterialInterface> ReferenceVariable (TEXT("Material'/Game/StarterContent/Materials/M_Wood_Pine.M_Wood_Pine'")); 
   if(ReferenceVariable.Succeeded())
    {
        Material = ReferenceVariable.Object;
    }

   UE_LOG(LogTemp, Warning, TEXT("Created Table Actor "));

}

#if WITH_EDITOR  
void ATableActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateMesh();
}
#endif */// WITH_EDITOR

// Called when the game starts or when sActored
void ATableActor::BeginPlay()
{
    Super::BeginPlay();

	// init utilty
	World = GetWorld();
	PlayerController = Cast<APlayerController>(World->GetFirstPlayerController());
	
	// bind input actions
	UInputComponent* myInputComp = InputComponent;
	if (myInputComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component setted"));
		myInputComp->BindAction("ResizeAction", EInputEvent::IE_Pressed, this, &ATableActor::ResizeAction);
		myInputComp->BindAction("StopResizeAction", EInputEvent::IE_Released, this, &ATableActor::StopResizeAction);
	}
	
	GenerateMesh();
	GenerateChairSide1();
	GenerateChairSide2();
	chairsPerSide12++;
	GenerateChairSide3();
	GenerateChairSide4();
}

// Called every frame
void ATableActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	FVector MouseDirection, MousePos;
	if (bResize)
	{
		PlayerController->DeprojectMousePositionToWorld(MousePos, MouseDirection);
		// if y is > then y when clicked 
		// add the difference
		
		if (MousePos.Y > MousePosWhenClicked.Y) {
			UE_LOG(LogTemp, Warning, TEXT("mousePosWhenClicked x: %f , y: %f "), MousePosWhenClicked.X, MousePosWhenClicked.Y);
			UE_LOG(LogTemp, Warning, TEXT("mousePos x: %f , y: %f "), MousePos.X, MousePos.Y);

			ResizeRightY((MousePos.Y- MousePosWhenClicked.Y) * 2000 * DeltaTime);
			MousePosWhenClicked.Y = MousePos.Y;
		}// if is less resize to left ( till min size )
		else if (MousePos.Y < MousePosWhenClicked.Y) {
			ResizeLeftY((MousePosWhenClicked.Y - MousePos.Y) * 2000 * DeltaTime);
			MousePosWhenClicked.Y = MousePos.Y;
		}
		
		// if is the same don't do nothing

	}
}

void ATableActor::SetupMeshBuffers()
{
    int32 VertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
    Vertices.AddUninitialized(VertexCount);
    Triangles.AddUninitialized(6 * 2 * 3); // 2x triangles per cube side, 3 verts each
}

void ATableActor::GenerateMesh()
{
    // The number of vertices or polygons wont change at runtime, so we'll just allocate the arrays once
    if (!bHaveBuffersBeenInitialized)
    {
        SetupMeshBuffers();
        bHaveBuffersBeenInitialized = true;
    }

    //////////////////////////
    // TABLE GENERATION
    /////////////////////////

	FBox BoundingBox = FBox(-Size/2, Size/2);
	
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

void ATableActor::GenerateCube(TArray<FRuntimeMeshVertexSimple> &InVertices, TArray<int32> &InTriangles, FVector InSize, FVector OffsetPos)
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

void ATableActor::BuildQuad(TArray<FRuntimeMeshVertexSimple> &InVertices, TArray<int32> &InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, int32 &VertexOffset, int32 &TriangleOffset, FPackedNormal Normal, FPackedNormal Tangent)
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
void ATableActor::GenerateChairSide1()
{
	ChairXLoc = -50.0f;
	ChairYawRotation = 180.0f;
	GenerateChair();
}
void ATableActor::GenerateChairSide2()
{
	ChairXLoc = 50.0f;
	ChairYawRotation = 0.0f;
	GenerateChair();
}

void ATableActor::GenerateChairSide3()
{
	ChairXLoc = 0.0f;
	ChairYLoc = -50.0f;
	ChairYawRotation = -90.0f;
	GenerateChair();
}

void ATableActor::GenerateChairSide4()
{
	ChairXLoc = 0.0f;
	ChairYLoc = 50.0f;
	ChairYawRotation = 90.0f;
	// save chair for moving it,
	// this way you don't need to
	// loop over actors when moving the table
	Chair4 = GenerateChair();
}

AActor* ATableActor::GenerateChair()
{
	AActor* GeneratedActor = nullptr;
	if (World != nullptr)
	{
		
		FVector Location = GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("Y location: %f"), Location.Y);
		UE_LOG(LogTemp, Warning, TEXT("chair Y location: %f"), ChairYLoc);
		Location.X +=  ChairXLoc;
		Location.Y +=  ChairYLoc;
		Location.Z +=  ChairZLoc;
	
		FRotator Rotation = GetActorRotation();
		Rotation.Roll += ChairRollRotation;
		Rotation.Yaw += ChairYawRotation;
		Rotation.Pitch += ChairPitchRotation;
		FActorSpawnParameters Params;
		//Params.Owner = this;
		//Params.Instigator = Instigator;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//FString PName("SActored Chair ").AppendInt(cntChairs);
		//Params.Name = TEXT("Chair");
		
		GeneratedActor = World->SpawnActor<AChairActor>(Location, Rotation, Params);
		// add chairs int cnt if generation ends correctly
		if (GeneratedActor) cntChairs++;
	}

	return GeneratedActor;
}

void ATableActor::ResizeUpX(float axisValue)
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

void ATableActor::ResizeDownX(float axisValue)
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

void ATableActor::ResizeLeftY(float axisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("axis value for left resize %f: "), axisValue);
	InitialLowerBoundY -= axisValue;
	InitialUpperBoundY -= axisValue;

    Size.Y = Size.Y - axisValue;
    FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    OffsetPosT.Y = OffsetPosT.Y-axisValue/2;
     // Initial table min size 
    if(OffsetPosT.Y < InitialOffsetPosT.Y  && Size.Y < InitialSize.Y){
        OffsetPosT.Y = InitialOffsetPosT.Y;
        Size.Y = InitialSize.Y;
        return;
    }

    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    MeshComponent->UpdateMeshSection(0, Vertices);

     // Move 1° leg left
    OffsetPosFirstLeg.Y = OffsetPosFirstLeg.Y-axisValue;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosFirstLeg);
    MeshComponent->UpdateMeshSection(1, Vertices);

    // Move 2° leg left
    OffsetPosSecondLeg.Y = OffsetPosSecondLeg.Y-axisValue;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->UpdateMeshSection(2, Vertices);

	// Move chair on side 4 to left
	if (Chair4) {
		FVector actualLocation = Chair4->GetActorLocation();
		actualLocation.Y -= axisValue;
		Chair4->SetActorLocation(actualLocation);
	}

	// remove extra chairs
	int32 width = (int32)(Size.Y / 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("table width: %d"), width);
	UE_LOG(LogTemp, Warning, TEXT("nr of chairs : %d"), cntChairs);
	if (width < chairsPerSide12) {
		ChairYLoc -= 40.0f;

		// destroy last 2 spawned actors
		UGameplayStatics::GetAllActorsOfClass(World, AChairActor::StaticClass(), FoundActors);
		
		if (World) {
			AActor* LastChair;
			// find exactly the last two spawned actors
			LastChair = FoundActors.Pop();
			UE_LOG(LogTemp, Warning, TEXT("name of destroyed chair from World: %s"), *LastChair->GetName());
			bool bDestr = World->DestroyActor(LastChair);
			if (bDestr) cntChairs--;

			LastChair = FoundActors.Pop();
			UE_LOG(LogTemp, Warning, TEXT("name of destroyed chair from World: %s"), *LastChair->GetName());
			bDestr = World->DestroyActor(LastChair);
			if (bDestr) cntChairs--;
		}

		chairsPerSide12--;
	}
}

void ATableActor::ResizeRightY(float axisValue)
{
	UE_LOG(LogTemp, Warning, TEXT("axis value: %f"), axisValue);
	InitialLowerBoundY += axisValue;
	InitialUpperBoundY += axisValue;

    Size.Y = Size.Y + axisValue;
	FBox BoundingBox = FBox(-Size / 2.0f, Size / 2.0f);
    OffsetPosT.Y = OffsetPosT.Y+axisValue/2;
    GenerateCube(Vertices, Triangles, Size, OffsetPosT);
    // This version updates only the vertices
    MeshComponent->UpdateMeshSection(0, Vertices);

     // Move 1° leg right
    OffsetPosFirstLeg.Y = OffsetPosFirstLeg.Y+axisValue;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosFirstLeg);
    MeshComponent->UpdateMeshSection(1, Vertices);

     // Move 2° leg right
    OffsetPosSecondLeg.Y = OffsetPosSecondLeg.Y+axisValue;
    GenerateCube(Vertices, Triangles, LegSize, OffsetPosSecondLeg);
    MeshComponent->UpdateMeshSection(2, Vertices);

	// Move chair on side 4 to right
	if (Chair4) {
		FVector actualLocation = Chair4->GetActorLocation();
		actualLocation.Y += axisValue;
		Chair4->SetActorLocation(actualLocation);
	}

	// add chairs if width allows it
	int32 width = (int32)(Size.Y / 100.0f);
	UE_LOG(LogTemp, Warning, TEXT("table width: %d"), width);
	UE_LOG(LogTemp, Warning, TEXT("nr of chairs : %d"), cntChairs);
	if (width > chairsPerSide12) {
		ChairYLoc += 40.0f;
		GenerateChairSide1();
		GenerateChairSide2();
		chairsPerSide12++;
	}

}
void ATableActor::ResizeAction()
{
	UE_LOG(LogTemp, Warning, TEXT("Resize Right action "));
	PossesClicked();
}

void ATableActor::StopResizeAction()
{
	UE_LOG(LogTemp, Warning, TEXT("STOP Resize!!"));
	bResize = false;
}

void ATableActor::PossesClicked()
{
    if (PlayerController != nullptr)	
    {
        // Get the coordinates of the mouse from our controller
        float LocationX;
        float LocationY;
        PlayerController->GetMousePosition(LocationX, LocationY);

        UE_LOG(LogTemp, Warning, TEXT("Maouse locations x: %f , y: %f"), LocationX, LocationY);
        FVector2D MousePosition(LocationX, LocationY);
		
        FHitResult HitResult;
        const bool bTraceComplex = false;
        if (PlayerController->GetHitResultAtScreenPosition(MousePosition, ECC_Visibility, bTraceComplex, HitResult) == true)
        {
            UE_LOG(LogTemp, Warning, TEXT("hit point x: %f , y: %f"), HitResult.Location.X, HitResult.Location.Y);
			
          
            // If the Actor we intersected with is a controller posses it
			ATableActor *ClickedActor = Cast<ATableActor>(HitResult.GetActor());
            if (ClickedActor)
            {
				UE_LOG(LogTemp, Warning, TEXT("point of impact x: %f , y: %f"), HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y);
							
                UE_LOG(LogTemp, Warning, TEXT("hit point in Actor"));
				 //check if click is in vertex bottom right table corner
				if ((InitialLowerBoundX <= HitResult.ImpactPoint.X && HitResult.ImpactPoint.X <= InitialUpperBoundX) && (InitialLowerBoundY <=HitResult.ImpactPoint.Y && HitResult.ImpactPoint.Y <= InitialUpperBoundY)) {
					
					UE_LOG(LogTemp, Warning, TEXT("hit point in Actor bottom right corner!!!"));
				
					// convert mouse position from screen to world
					// and save mouse position on corner click
					PlayerController->DeprojectMousePositionToWorld(MousePosWhenClicked, MouseDirectionWhenClicked);
					bResize = true;
				}
	
            }
        }
    }
}

