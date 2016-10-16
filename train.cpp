// Author: Sushil Ravoori
#define MAX 2147483647

#include "TrainRunner.h"
#include "train.h"
#include <cstring>
#include <iostream>
#include <climits>
using namespace std;

StationNode::StationNode()
{
	visited = false;
}

Train::Train(Station *stations, int numStations, int numCars) 
{
	graph = new Station[numStations];
	carNum = numCars;
	stationsNum = numStations;
	memcpy(graph, stations, sizeof(*stations) * numStations);
	carsOnTrainID = new Queue<int>(50);
	distanceheap = *(new BinaryHeap<StationNode>(stationsNum*2));
	carsOnTrain = new int[carNum];
	for (short i = 0; i < carNum; i++)
	{
		carsOnTrain[i] = 0;
	}
	for (short i = 0; i < 20000; i++)
	{
		dvs[i] = SHRT_MAX;
	}
	deliveredCars = 0;
	size = 0;
} // Train()

StackAr<StationNode*> * Train::ShortestPathUnweighted(int startstation, int endstation) //run djikstra's
{
	Queue<StationNode*> distances(stationsNum);
	for (int i = 0; i < stationsNum; i++)
	{

		StationNode* node = new StationNode(); 
		node->number = i;

		if ( i == startstation)
		{
			node->distance = 0;
			node->simpledistance= 0;
		} //initialize starting point with distance 0
		else
		{
			node->simpledistance = MAX;
		}
		//initializes new node with distance infinity
		visitedarray[i] = node;
	} //set up min heap for djikstra's
	distances.enqueue(visitedarray[startstation]);

	StationNode* currentnode;

	while(!distances.isEmpty())
	{
		currentnode = distances.dequeue();
		for (int i = 0; i < graph[currentnode->number].adjCount; i++)
			{
				if (visitedarray[graph[currentnode->number].adjacent[i]]->simpledistance == MAX)
				{
					visitedarray[graph[currentnode->number].adjacent[i]]->distance = currentnode->distance + graph[currentnode->number].distances[i];
					visitedarray[graph[currentnode->number].adjacent[i]]->simpledistance = currentnode->simpledistance +1;
					visitedarray[graph[currentnode->number].adjacent[i]]->prevstation = currentnode->number;
					distances.enqueue(visitedarray[graph[currentnode->number].adjacent[i]]);
				}
			}
		if (currentnode->number == endstation)
		{
			distances.makeEmpty(); //leave loop if path found
		}
	}	
	

	StationNode *tempnode = visitedarray[endstation];
	StackAr <StationNode*> *path = new StackAr <StationNode*> (20000);
	while (tempnode->number != startstation) //while not at beginninng
	{
		path->push(tempnode);
		tempnode = visitedarray[tempnode->prevstation];

	} 
	return path;

	// access destionation node
	// go back through currentnode->number to find path 
	// return as array of ints(station #s)

}

StackAr<StationNode*> * Train::ShortestPath(int startstation, int endstation) //run djikstra's
{
	BinaryHeap<StationNode> distanceheap(stationsNum*2);
	StationNode* visitedarray[stationsNum];
	for (int i = 0; i < stationsNum; i++)
	{

		StationNode* node = new StationNode(); 
		node->number = i;

		if ( i == startstation)
		{
			node->distance = 0;
		} //initialize starting point with distance 0
		else
		{
			node->distance = MAX;
		}
		//initializes new node with distance infinity
		distanceheap.insert(*node); 
		visitedarray[i] = node;
	} //set up min heap for djikstra's

	
	while (!visitedarray[endstation]->visited && !distanceheap.isEmpty())
	{

		StationNode* currentnode = new StationNode();
		distanceheap.deleteMin(*currentnode); //gets min node and removes from heap

		if (!currentnode->visited)
		{

			for (int i = 0; i < graph[currentnode->number].adjCount; i++)
			{
				if ((visitedarray[graph[currentnode->number].adjacent[i]]->distance> 
					(graph[currentnode->number].distances[i] + currentnode->distance)))
				{
				StationNode* nextstation = new StationNode();
				nextstation->number = graph[currentnode->number].adjacent[i];
				nextstation->distance = graph[currentnode->number].distances[i] + currentnode->distance;
				nextstation->prevstation = currentnode->number;
				visitedarray[nextstation->number] = nextstation;
				distanceheap.insert(*nextstation);
				}

				//visit and update station (adjacents[i)] distances in heap to distances[i]+currentnode->distance
				//update previous node as currentnode->number;
			}
			currentnode->visited = true;
			visitedarray[currentnode->number]->visited = true;
		}	
	}
	
	// mark node visited in array
	// repeat while heap not empty

	StationNode *tempnode = visitedarray[endstation];
	StackAr <StationNode*> *path = new StackAr <StationNode*> (10000);
	while (tempnode->number != startstation) //while not at beginninng
	{
		path->push(tempnode);
		tempnode = visitedarray[tempnode->prevstation];

	} 
	return path;

	// access destionation node
	// go back through currentnode->number to find path 
	// return as array of ints(station #s)

}
bool StationNode::operator<(const StationNode& rhs) const
{
	if (distance < rhs.distance)
	{
		return true;
	}
	return false;
}
int Train::findNearestCarUnweighted(Queue<int> **carLocations, int trainLocation) //only call if there are cars left!
{
	Queue<StationNode*> distances(stationsNum);
	StationNode* visitedarray[stationsNum];
	for (int i = 0; i < stationsNum; i++)
	{

		StationNode* node = new StationNode(); 
		node->number = i;

		if ( i == trainLocation)
		{
			node->distance = 0;
			node->simpledistance= 0;
		} //initialize starting point with distance 0
		else
		{
			node->simpledistance = MAX;
		}
		//initializes new node with distance infinity
		visitedarray[i] = node;
	} //set up min heap for djikstra's
	distances.enqueue(visitedarray[trainLocation]);
	StationNode* currentnode;

	while(!distances.isEmpty())
	{
		currentnode = distances.dequeue();
		if ((!carLocations[currentnode->number]->isEmpty() && size <50)
		 || deliverylocations[currentnode->number])
		{
			return currentnode->number;
		}

			for (int i = 0; i < graph[currentnode->number].adjCount; i++)
			{
				if (visitedarray[graph[currentnode->number].adjacent[i]]->simpledistance == MAX)
				{
					visitedarray[graph[currentnode->number].adjacent[i]]->distance = currentnode->distance + graph[currentnode->number].distances[i];
					visitedarray[graph[currentnode->number].adjacent[i]]->simpledistance = currentnode->simpledistance +1;
					visitedarray[graph[currentnode->number].adjacent[i]]->prevstation = currentnode->number;
					distances.enqueue(visitedarray[graph[currentnode->number].adjacent[i]]);
				}
			}
	}
	return 0;

	// mark node visited in array
	// repeat while heap not empty   
}

short Train::findNearestCar(Queue<int> **carLocations, int trainLocation) //only call if there are cars left!
{
	short station;
	Index = 0;
	dvStack[Index++] = trainLocation; 
	pvs[trainLocation] = -1;
	dvs[trainLocation] = 0;
	StationNode* node = new StationNode;
	node->number = trainLocation;
	node->distance = 0;
	distanceheap.makeEmpty();
	distanceheap.insert(*node);
	while(true) //runs forever till break
	{
		do
		{
			distanceheap.deleteMin(*node);
		} while(node->distance > dvs[node->number]);
		station = node->number;

		if((size < 50 && !carLocations[station]->isEmpty())
		  || deliverylocations[station] > 0 )
		  break;

		for (short i = 0; i < graph[station].adjCount; i++)
		{
			short d = dvs[station] + graph[station].distances[i]; //updated new distance
			if (d < dvs[graph[station].adjacent[i]]) //if better then update
			{
				StationNode* newnode = new StationNode;
				newnode->number = graph[station].adjacent[i];
				newnode->distance = d;
				distanceheap.insert(*newnode);
				dvs[graph[station].adjacent[i]] = d;
				dvStack[Index++] = graph[station].adjacent[i];
				pvs[graph[station].adjacent[i]] = station;
			}
		}
	}
	for(short i = 0; i < Index; i++)
	{
    dvs[dvStack[i]] = SHRT_MAX;
	}
	return station;
}



void Train::run(Car *cars, Operation *operations, int *numOperations)
{
	int operationNum = 0;
	short trainlocation = 0;
	int time = 0;
	*numOperations = 0;

	for (short i = 0; i < stationsNum; i++)
	{
		deliverylocations[i] = 0;
		carLocations[i] = new Queue<int>(10); //array of queues for each station's cars
	}

	for (short i = 0; i < carNum; i++)
	{
		carLocations[cars[i].origin]->enqueue(i); //put cars at their locations
	}


	while(deliveredCars < carNum)
	{
		//cout << CarsAtStations << endl;
		int POIlocation = findNearestCar(carLocations, trainlocation);
		int i, j, path[200000];
		for (i = 0; POIlocation != trainlocation; i++)
		{
			//cout << POIlocation << " " << trainlocation << endl;
			path[i] = POIlocation;
			POIlocation = pvs[POIlocation];
		}
		for (--i; i>=0; i--)
		{
			for (j = 0; graph[trainlocation].adjacent[j] != path[i]; j++);

			time+= graph[trainlocation].distances[j];
			operations[operationNum++] = Operation(time, 'M', path[i]);
			//cout << operationNum << ". " <<  operations[operationNum].operation << " ID: " << operations[operationNum].ID << " time: " << operations[operationNum].time <<endl;
			*numOperations += 1;
			trainlocation = path[i];
		}

		int carID;
		while (size < 50 && !(carLocations[trainlocation]->isEmpty()))
			{
				carID = carLocations[trainlocation]->dequeue();
				carsOnTrain[carID] = 1;
				deliverylocations[cars[carID].destination] += 1;
				operations[operationNum++] = Operation(time, 'P', carID);
				//cout << operationNum << ". " <<  operations[operationNum].operation << " ID: " << operations[operationNum].ID << " time: " << operations[operationNum].time <<endl;
				*numOperations+= 1;
				size++;
			//	cout << CarsAtStations << endl;
			}
		if (deliverylocations[trainlocation] > 0)
				{
					int i;
					for (i = 0; i < carNum; i++)
					{
						if (carsOnTrain[i] && (cars[i].destination == trainlocation))
							break;
					}
						carID = i;
						operations[operationNum++] = Operation(time, 'D', carID);
						carsOnTrain[carID] = 0;
						deliverylocations[trainlocation] -=1;
						//cout << operationNum << ". " <<  operations[operationNum].operation << " ID: " << operations[operationNum].ID << " time: " << operations[operationNum].time <<endl;
						size--;
						*numOperations+= 1;
						deliveredCars++;
						//cout << deliveredCars << endl;
						//cout << size << endl;
				}
		}
	}

	