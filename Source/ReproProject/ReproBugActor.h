#pragma once

#include "GameFramework/Actor.h"
#include "ReproTranslucentBug.h"
#include "ReproBugActor.generated.h"

UCLASS(HideCategories = (Input, Rendering, Tags, Transform, Cooking,LOD, Collision, Actor, Layers, Replication), Blueprintable)
class AReproBugActor : public AActor
{
	GENERATED_BODY()

public:
	AReproBugActor(const FObjectInitializer& ObjectInitializer);

	UReproTranslucentBugComponent* ReproTranslucentBug;

#if !UE_BUILD_SHIPPING
	void Tick(float DeltaTime) override;
#endif
};
