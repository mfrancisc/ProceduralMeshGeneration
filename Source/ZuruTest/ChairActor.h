#pragma once

#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "ChairActor.generated.h"

UCLASS()
class ZURUTEST_API AChairActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChairActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	#if WITH_EDITOR
    	virtual void OnConstruction(const FTransform& Transform) override;
	#endif   // WITH_EDITOR

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector ChairSize = FVector(25.0f, 25.0f, 2.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosChair = FVector(0.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector ChairBackSize = FVector(2.5f, 25.0f, 25.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector OffsetPosBackChair = FVector(0.0f, 0.0f, 0.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector ChairLegSize = FVector(2.5f, 2.5f, 25.0f);

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffLegX = ChairSize.X / 2.0f - 2.5f;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffLegY = ChairSize.Y / 2.0f - 2.5f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffLegZ = ChairSize.Z * 5 + 2.5f;

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
