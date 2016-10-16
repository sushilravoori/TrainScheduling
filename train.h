#ifndef TRAIN_H
#define	TRAIN_H

#include "TrainRunner.h"
#include "BinaryHeap.h"
#include "QueueAr.h"
#include "StackAr.h"

#define MAX_STATIONS 20000
class StationNode
{
public:
	StationNode();
	int number;
	int distance;
	int prevstation;
	int simpledistance;
	bool visited; 
	bool operator<(const StationNode& rhs) const;
};


class Train
{
	Queue<int> *carLocations[MAX_STATIONS];
	short Index;
	short dvs[MAX_STATIONS];
 	short dvStack[MAX_STATIONS];
 	short pvs[MAX_STATIONS];
	Station *graph;
	int deliveredCars;
	int carNum;
	int stationsNum;
	Queue <int> *carsOnTrainID;
	int deliverylocations[20000];
	int *carsOnTrain;
	BinaryHeap<StationNode> distanceheap;
	StationNode**visitedarray;
	int size;
public:
  Train(Station *stations, int numStations, int numCars);
  void run(Car *cars, Operation *operations, int *numOperations);
  StackAr<StationNode*> * ShortestPath(int startstation, int endstation);
  StackAr<StationNode*> * ShortestPathUnweighted(int startstation, int endstation);
  short findNearestCar(Queue<int> **carLocations,  int trainLocation);
  int findNearestCarUnweighted(Queue<int> **carLocations, int trainLocation);
}; // class train 




#endif	// TRAIN_H

