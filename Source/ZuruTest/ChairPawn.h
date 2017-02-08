#pragma once

#include "GameFramework/Pawn.h"
#include "RuntimeMeshComponent.h"
#include "ChairPawn.generated.h"

UCLASS()
class ZURUTEST_API AChairPawn : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this Pawn's properties
	AChairPawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/**#if WITH_EDITOR
    	virtual void OnConstruction(const FTransform& Transform) override;
	#endif  */ // WITH_EDITOR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector ChairSize = FVector(25.0f, 25.0f, 2.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosChair = FVector(-0.0f, 0.0f, -3.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector ChairBackSize = FVector(3.0f, 25.0f, 25.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosBackChair = FVector(11.0f, 0.0f, 10.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector ChairLegSize = FVector(2.5f, 2.5f, 25.0f);

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffLegX = 10.5f;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffLegY = 10.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffLegZ = 17.0f;

	FVector OffsetPosFirstLeg;
    FVector OffsetPosSecondLeg;
    FVector OffsetPosThirdLeg;
    FVector OffsetPosFourthLeg;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    UMaterialInterface* Material;

    UPROPERTY(EditAnywhere)
    USceneComponent* OurVisibleComponent;

protected:
    
    UPROPERTY()
    URuntimeMeshComponent* MeshComponent;

private:
    void GenerateMesh();
    void GenerateCube(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector InSize, FVector OffsetPos);
    void BuildQuad(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, FPackedNormal Normal, FPackedNormal Tangent);
    // Mesh buffers
    void SetupMeshBuffers();
    bool bHaveBuffersBeenInitialized = false;
    TArray<FRuntimeMeshVertexSimple> Vertices;
    TArray<int32> Triangles;
	
	
};
