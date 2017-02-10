#pragma once

#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "TableActor.generated.h"

UCLASS()
class ZURUTEST_API ATableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this Actor's properties
	ATableActor();

	// Called when the game starts or when sActored
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void StopResizeAction();
	void ResizeAction();

	void PossesClicked();
    void ResizeDownX(float axisValue);
    void ResizeUpX(float axisValue);
    void ResizeLeftY(float axisValue);
    void ResizeRightY(float axisValue);
	void GenerateChairSide1();
	void GenerateChairSide2();
	void GenerateChairSide3();
	void GenerateChairSide4();


	#if WITH_EDITOR
    	virtual void OnConstruction(const FTransform& Transform) override;
	#endif   // WITH_EDITOR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector Size = FVector(100.0f, 100.0f, 5.0f);
    FVector InitialSize = FVector(100.0f, 100.0f, 5.0f);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffX = Size.X / 2.0f - 2.5f;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffY = Size.Y / 2.0f - 2.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffZ = Size.Z * 5 + 2.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float ChairXLoc = -50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float ChairYLoc = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float ChairZLoc = -25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float ChairRollRotation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float ChairYawRotation = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	float ChairPitchRotation = 0.0f;

    // initial table offeset
    FVector OffsetPosT = FVector(0.0f, 0.0f, 0.0f);
    FVector InitialOffsetPosT = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
	FVector OffsetPosFirstLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosSecondLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosThirdLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosFourthLeg;

	// initial bouds for table clickable corner
	float InitialLowerBoundX = 40.0f;
	float InitialUpperBoundX = 60.0f;
	float InitialLowerBoundY = 0.0f;
	float InitialUpperBoundY = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector LegSize = FVector(5.0f, 5.0f, 50.0f);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    UMaterialInterface* Material;

protected:
    
    UPROPERTY()
    URuntimeMeshComponent* MeshComponent;

	int32 cntChairs = 0;
	int32 chairsPerSide12 = 0;
	FVector WorldLocation;
	FVector MousePosWhenClicked;
	FVector MouseDirectionWhenClicked;

private:
    void GenerateMesh();
    void GenerateCube(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector InSize, FVector OffsetPos);
    void BuildQuad(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, FPackedNormal Normal, FPackedNormal Tangent);
    // Mesh buffers
    void SetupMeshBuffers();
	AActor* GenerateChair();
	AActor* Chair4;
    bool bHaveBuffersBeenInitialized = false;
    TArray<FRuntimeMeshVertexSimple> Vertices;
    TArray<int32> Triangles;
	bool bResize;
	APlayerController *PlayerController;
	TArray<AActor*> FoundActors;
	UWorld* World;

};
