#pragma once

#include "TBSGrid.h"

class TBSGridPathWorker : public FRunnable
{
public:

	int32				TotalPrimesToFind;

	//Done?
	bool IsFinished;

	//~~~ Thread Core Functions ~~~

	//Constructor / Destructor
	TBSGridPathWorker(TMap<FIntVector, FIntVector>& InPath, FIntVector InOrigin, FIntVector InDimensions, ATBSGrid* InGrid);
	virtual ~TBSGridPathWorker();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();



	//~~~ Starting and Stopping Thread ~~~



	/*
	Start the thread and the worker from static (easy access)!
	This code ensures only 1 Prime Number thread will be able to run at a time.
	This function returns a handle to the newly started instance.
	*/
	static TBSGridPathWorker* JoyInit(TMap<FIntVector, FIntVector>& Path, FIntVector Origin, FIntVector Dimensions, ATBSGrid* Grid);

	/** Shuts down the thread. Static so it can easily be called from outside the thread context */
	static void Shutdown();

	static bool IsThreadFinished();

private:

	/** Singleton instance, can access the thread any time via static accessor, if it is active! */
	static  TBSGridPathWorker* Runnable;

	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;

	/** Stop this thread? Uses Thread Safe Counter */
	FThreadSafeCounter StopTaskCounter;

	TMap<FIntVector, FIntVector>* Path;

	FIntVector Origin;

	FIntVector Dimensions;

	ATBSGrid* Grid;

};

