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

	 // Called to bind functionality to input
    //virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent);

	void ResizeAction();
    void PossesClicked();
    void ResizeDownX(float axisValue);
    void ResizeUpX(float axisValue);
    void ResizeLeftY(float axisValue);
    void ResizeRightY(float axisValue);

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

    // initial table offeset
    FVector OffsetPosT = FVector(0.0f, 0.0f, 0.0f);
    FVector InitialOffsetPosT = FVector(0.0f, 0.0f, 0.0f);
    FVector OffsetPosFirstLeg;
    FVector OffsetPosSecondLeg;
    FVector OffsetPosThirdLeg;
    FVector OffsetPosFourthLeg;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector LegSize = FVector(5.0f, 5.0f, 50.0f);
    
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
