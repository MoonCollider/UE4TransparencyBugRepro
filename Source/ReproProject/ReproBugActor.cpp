#include "ReproBugActor.h"

#if !UE_BUILD_SHIPPING
AReproBugActor::AReproBugActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ReproTranslucentBug = ObjectInitializer.CreateDefaultSubobject<UReproTranslucentBugComponent>(this, TEXT("ReproBug"));
	ReproTranslucentBug->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AReproBugActor::Tick(float DeltaTime)
{
	ReproTranslucentBug->MarkRenderStateDirty();
}

#else
AReproBugActor::AReproBugActor(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}
#endif
