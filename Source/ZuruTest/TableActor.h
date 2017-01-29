// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshes.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "TableActor.generated.h"


UCLASS()
class ZURUTEST_API ATableActor : public AActor
{
    GENERATED_BODY()
    
public:
    // Sets default values for this actor's properties
    ATableActor();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector Size = FVector(100.0f, 100.0f, 5.0f);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffX;
   
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffY;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    float offsetDiffZ;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    FVector LegSize = FVector(5.0f, 5.0f, 50.0f);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Parameters")
    UMaterialInterface* Material;

    UPROPERTY(EditAnywhere)
    USceneComponent* OurVisibleComponent;
    
    virtual void BeginPlay() override;
    
#if WITH_EDITOR
    virtual void OnConstruction(const FTransform& Transform) override;
#endif   // WITH_EDITOR
    
protected:
    
    UPROPERTY()
    URuntimeMeshComponent* MeshComponent;
    
private:
    void GenerateMesh();
    void GenerateCube(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector InSize, FVector OffsetPos);
   // void GenerateLeg(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector InSize);
    void BuildQuad(TArray<FRuntimeMeshVertexSimple>& InVertices, TArray<int32>& InTriangles, FVector BottomLeft, FVector BottomRight, FVector TopRight, FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, FPackedNormal Normal, FPackedNormal Tangent);
    // Mesh buffers
    void SetupMeshBuffers();
    bool bHaveBuffersBeenInitialized = false;
    TArray<FRuntimeMeshVertexSimple> Vertices;
    TArray<int32> Triangles;
    
    
};
