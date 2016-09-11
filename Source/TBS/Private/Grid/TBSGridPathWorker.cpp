#include "TBS.h"
#include "TBSGridPathWorker.h"
#include "TBSGridPathFindingHelper.h"

TBSGridPathWorker* TBSGridPathWorker::Runnable = NULL;

TBSGridPathWorker::TBSGridPathWorker(TMap<FIntVector, FIntVector>& InPath, FIntVector InOrigin, FIntVector InDimensions, ATBSGrid* InGrid)
	: Grid(InGrid),
	Origin(InOrigin),
	Dimensions(InDimensions),
	StopTaskCounter(0),
	IsFinished(false)
{
	Path = &InPath;
	Thread = FRunnableThread::Create(this, TEXT("FGridPathWorker"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}


TBSGridPathWorker::~TBSGridPathWorker()
{
	delete Thread;
	Thread = NULL;
}

bool TBSGridPathWorker::Init()
{
	Path->Empty();
	return true;
}

uint32 TBSGridPathWorker::Run()
{
	FPlatformProcess::Sleep(0.03);

	TArray<PathStep> Frontier;
	Frontier.HeapPush(PathStep(Origin, 0.0), PathStepPredicate());

	Path->Add(Origin, FIntVector::NoneValue);

	TMap<FIntVector, float> CostSoFar;
	CostSoFar.Add(Origin, 0.0);

	while (Frontier.Num() > 0 && StopTaskCounter.GetValue() == 0)
	{
		PathStep Current;
		Frontier.HeapPop(Current, PathStepPredicate());

		for (auto& Next : TBSGridPathFindingHelper::GetNeighbourNodes(Grid, Current.Coordinates, Dimensions))
		{
			float NewCost;

			float MovementCost = TBSGridPathFindingHelper::CalculateMovementCost(Current, Next);
			NewCost = CostSoFar[Current.Coordinates] + MovementCost;

			// Limit path finding to some arbitrary cost now
			if (NewCost < 35)
			{
				if (!CostSoFar.Contains(Next) || NewCost < CostSoFar[Next])
				{
					CostSoFar.Add(Next, NewCost);
					float Priority = 1.0f;
					Frontier.HeapPush(PathStep(Next, Priority), PathStepPredicate());
					Path->Add(Next, Current.Coordinates);
				}
			}
		}
	}

	IsFinished = true;

	return 0;
}

void TBSGridPathWorker::Stop()
{
	StopTaskCounter.Increment();
}

TBSGridPathWorker* TBSGridPathWorker::JoyInit(TMap<FIntVector, FIntVector>& Path, FIntVector Origin, FIntVector Dimensions, ATBSGrid* Grid)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Starting a worker thread")));

	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new TBSGridPathWorker(Path, Origin, Dimensions, Grid);
	}
	return Runnable;
}


void TBSGridPathWorker::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void TBSGridPathWorker::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool TBSGridPathWorker::IsThreadFinished()
{
	if (Runnable)
	{
		return Runnable->IsFinished;
	}

	return true;
}


