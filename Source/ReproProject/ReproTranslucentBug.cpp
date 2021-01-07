#include "ReproTranslucentBug.h"

#define USE_TRANSPARENCY 1

#pragma optimize("",off)
class FReproTranslucentBugSceneProxy : public FPrimitiveSceneProxy
{
	static FHitProxyId HitProxyId;

public:
	FReproTranslucentBugSceneProxy(const UReproTranslucentBugComponent* InComponent)	: FPrimitiveSceneProxy(InComponent)
	{
		bWillEverBeLit = false;
		bVerifyUsedMaterials = false;
	}

	virtual ~FReproTranslucentBugSceneProxy() {}

	SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				FSimpleElementCollector* ElementCollector = static_cast<FSimpleElementCollector*>(Collector.GetPDI(ViewIndex));

				if (ElementCollector)
				{

					FVector4 P0(-1680.0, -1300, 330);
					FVector4 P1(250, 800, 330);
					FVector4 P2(1680.0, 800, 330);

					FLinearColor Color(1.0f, 1.0f, 1.0f, 0.2f);

					FBatchedElements& Elements = ElementCollector->BatchedElements;

					auto T0 = Elements.AddVertex(P0, FVector2D(0, 0), Color, HitProxyId);
					auto T1 = Elements.AddVertex(P1, FVector2D(0, 0), Color, HitProxyId);
					auto T2 = Elements.AddVertex(P2, FVector2D(0, 0), Color, HitProxyId);
#if !USE_TRANSPARENCY//(ENGINE_MAJOR_VERSION==4 && ENGINE_MINOR_VERSION==26)
					Elements.AddTriangle(T0, T1, T2, GWhiteTexture, SE_BLEND_Opaque);		// Workaround in 4.26
#else
					Elements.AddTriangle(T0, T1, T2, GWhiteTexture, SE_BLEND_AlphaBlend);	// Worked in 4.25, not drawn in 4.26
#endif
				}
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = true;
		Result.bShadowRelevance = true;
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = true;
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
		Result.bVelocityRelevance = IsMovable() && Result.bOpaque && Result.bRenderInMainPass;
		//memset(&Result, 0xFF, sizeof(Result));
		return Result;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize());	}
};

FHitProxyId FReproTranslucentBugSceneProxy::HitProxyId = FHitProxyId();

//-----------------------------------------------------------------------------

UReproTranslucentBugComponent::UReproTranslucentBugComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

FBoxSphereBounds UReproTranslucentBugComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	return FBox(LocalToWorld.TransformPosition(FVector(-HALF_WORLD_MAX1, -HALF_WORLD_MAX1, -HALF_WORLD_MAX1)), LocalToWorld.TransformPosition(FVector(HALF_WORLD_MAX1, HALF_WORLD_MAX1, HALF_WORLD_MAX1)));
}

FPrimitiveSceneProxy* UReproTranslucentBugComponent::CreateSceneProxy()
{
	return new FReproTranslucentBugSceneProxy(this);
}
#pragma optimize("",on)
