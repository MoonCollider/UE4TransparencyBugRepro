#pragma once

#include "Components/PrimitiveComponent.h"
#include "ReproTranslucentBug.generated.h"

UCLASS(hidecategories = Object)
class UReproTranslucentBugComponent : public UPrimitiveComponent
{
	GENERATED_UCLASS_BODY()

public:
	
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
};
