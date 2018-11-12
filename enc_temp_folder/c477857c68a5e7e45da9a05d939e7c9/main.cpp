
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "functions.h"
#include <chrono>
#include <math.h>
#include <time.h> 

using namespace std;
using namespace std::chrono;

static int capacity; 

void writeData(vector<vector<int>> & shopDatabase) // to improve - fail if data is damaged 
{
	string line, newword; //temporary, only to help read proper data
	vector<int> shop;
	int data;

	ifstream file;

	file.open("file1.txt", ios::in);
	while (!file.eof())
	{

		file >> line;
		if (line == "CAPACITY")
		{
			for (int i = 0; i < 2; i++)
			{
				file >> capacity;
			}
		}

		else if (line == "SERVICE")
		{
			file >> newword;

			while (!file.eof())
			{
				file >> data;
				if (!file.eof())
				{
					shop.push_back(data);


					for (int i = 0; i < 6; i++)
					{
						file >> data;
						shop.push_back(data);

					}
					shopDatabase.push_back(shop);
					shop.clear();
				}
			}
		}
	}


	file.close();

}

void printShops(vector<vector<int>> dataBase)
{
	for (int j = 0; j < dataBase.size(); j++)
	{
		for (int i = 0; i < dataBase[0].size(); i++)
		{
			cout << dataBase[j][i] << " ";
		}
		cout << endl;
	}
}

void addFlag(vector<vector<int>> & shopDatabase)
{
	for (unsigned int j = 0; j < shopDatabase.size(); j++)
	{
		shopDatabase[j].push_back(0);
	}
}

vector<vector<int>> ResetVisitedFlag(vector<vector<int>> & dataBase)
{
	for (int j = 0; j < dataBase.size(); j++)
	{
		dataBase[j].back() = 0;
	}
	return dataBase;
}


void printTrucks(vector<vector<double>> fleet)//unnecessary printShops is identical
{
	for (int j = 0; j < fleet.size(); j++)
	{
		cout << "Truck " << j << endl;
		for (int i = 0; i < fleet[j].size(); i++)
		{
			cout << fleet[j][i] << " ";
		}
		cout << endl;
	}
}

void printDistance(vector<vector<double>> fleet)
{
	for (int j = 0; j < fleet.size(); j++)
	{
		for (int i = 0; i < fleet[0].size(); i++)
		{
			cout << fleet[j][i] << " ";
		}
		cout << endl;
	}
}

bool thereAreShopsToVisit(vector<vector<int>> shopDatabase)
{
	for (int j = 1; j < shopDatabase.size(); j++)
	{
		if (shopDatabase[j][7] == 0)
			return true;
	}
	return false;
}

void addTruck(vector<vector<double>> & trucks, int capacity, int xCoord, int yCoord, int openTime, int warehouseService)
{   // Add new truck to truckDatabase
	//truck = [capacity, xCoord, yCoord, actualTime ... shops visited ...]
	vector<double> truck;
	truck.push_back(capacity);
	truck.push_back(xCoord);
	truck.push_back(yCoord);
	truck.push_back(openTime + warehouseService);      //time
	trucks.push_back(truck);
}

void QuickSort(vector<vector<double>> & distance, int i, int j)
{
	int left = i;
	int right = j;
	int index = (i + j) / 2;
	double pivot = distance[index][1];
	while (i <= j)
	{
		while (distance[i][1] < pivot)
			i++;
		while (distance[j][1] > pivot)
			j--;

		if (i <= j)
		{
			distance[i].swap(distance[j]);
			i++;
			j--;
		}
	}
	if (left < j) QuickSort(distance, left, j);

	if (right > i) QuickSort(distance, i, right);
}


void updateTheTrackDatabase(vector<double> & truck, vector<vector<int>> & shopsDatabase, int theBestFitIndex, double distance)
{
	truck[0] -= shopsDatabase[theBestFitIndex][3]; //capacity
	truck[3] = truck[3] + shopsDatabase[theBestFitIndex][6] + distance;  //time: add distance and service time
	truck[1] = shopsDatabase[theBestFitIndex][1];  // xCoord
	truck[2] = shopsDatabase[theBestFitIndex][2]; // yCoord
	truck.push_back(theBestFitIndex);
}

bool makeDistanceVector(vector<vector<double>> & trucks, vector<vector<int>> & shopDatabase, vector<vector<double>> & distance, int & indexWaiting, double & distanceWaiting)
{
	vector<double> truck = trucks.back();
	bool thereAreshopsToVisitbool = false;
	bool enoughCapacity = false;
	int truckX = (int)truck[1];
	int truckY = (int)truck[2];
	int dataX;
	int dataY;
	double distanceDouble;
	double waiting = -1;
	
	

	for (int i = 1; i < shopDatabase.size(); i++)
	{
		if (shopDatabase[i].back() == 0 )// shop must be not visited 
		{
			thereAreshopsToVisitbool = true;
			if (truck[0] >= shopDatabase[i][3]) // truck has enough capacity
			{
				enoughCapacity = true;
				vector<double> record;
				dataX = shopDatabase[i][1];
				dataY = shopDatabase[i][2];
				distanceDouble = sqrt(pow((dataX - truckX), 2) + pow((dataY - truckY), 2));
				
				if ((shopDatabase[i][4] <= distanceDouble + truck[3] )&& (distanceDouble + truck[3] < shopDatabase[i][5])) //open window <=dis+ current truck time < close window
				{
					record.push_back((double)i);
					record.push_back(distanceDouble);
					distance.push_back(record);
				}
				else if (distanceDouble + truck[3] < shopDatabase[i][4])
				{
					double difference = shopDatabase[i][4] - (truck[3] + distanceDouble);
					if (waiting == -1 || difference < waiting)
					{
						waiting = difference;
						indexWaiting = i;
						distanceWaiting = distanceDouble + waiting;
					}
				}
			}
		}
	}
	if (thereAreshopsToVisitbool) {
		if (!enoughCapacity || (indexWaiting == -1 && distanceWaiting == -1  && distance.size()==0))
		{
			trucks.back()[3] += calculateDistance(truck, shopDatabase[0]);
			addTruck(trucks, capacity, shopDatabase[0][1], shopDatabase[0][2], shopDatabase[0][4], shopDatabase[0][6]);
		}
		return true;
	}
	else
		return false;
}

double calculateDistance(vector<double> first, vector<int> second)
{
	double firstX = first[1];
	double firstY = first[2];
	int secondX = second[1];
	int secondY = second[2];
	return sqrt(pow((secondX - firstX), 2) + pow((secondY - firstY), 2));
}

void selectBetterResult(vector<vector<double>> & bestResult, vector<vector<double>> trucksDatabase, string mode="sumTime") {
	// Compare previous best result with actual trucksDatabase and choose better result.
	if (bestResult.empty())
	{
		bestResult = trucksDatabase;
	}
	else if (mode == "theShortestTimeFromStartToEnd")
	{
		int maxTime = 0;
		for (int i = 0; i < trucksDatabase.size(); i++) {
			if (maxTime > trucksDatabase[i][3])
				maxTime = trucksDatabase[i][3];
		}
		if (maxTime < bestResult.back()[0]) {
			bestResult = trucksDatabase;
			bestResult.push_back((vector<double>)trucksDatabase.size());
			vector<double> maxT;
			maxT.push_back(maxTime);
			bestResult.push_back(maxT);
		}
	}
	else if (mode == "number") {
		if (trucksDatabase.size() < bestResult.size())
			bestResult = trucksDatabase;
	}
	else if (mode == "sumTime")
	{
		double previouslyShortestTime = calculateSumServiceTime(bestResult);
		double actualServiceTime = calculateSumServiceTime(trucksDatabase);
		if (actualServiceTime < previouslyShortestTime)
			bestResult = trucksDatabase;
	}
	else
		throw new exception("Wrong mode exception");
}

double calculateSumServiceTime(vector<vector<double>> trucksDatabase)
{
	double serviceTime = 0;
	for (int i = 0; i < trucksDatabase.size(); i++)
		serviceTime += trucksDatabase[i][3];
	return serviceTime;
}

int drawNextClient(vector<vector<double>> distance, int rangeDistance)
{
	srand(time(NULL));
	int chosen = rand() % rangeDistance;
	return chosen;
}



void saveToFile(vector<vector<double>> bestResult, string fileName)
{
	ofstream file;
	file.precision(26);
	cout.precision(16);
	file.open(fileName);
	/*if (bestResult.size() == 0)
		file << "-1" << "\n";
	else 
	{
		file << bestResult.size() << " " << calculateSumServiceTime(bestResult) << "\n";
		cout << bestResult.size() << " " << calculateSumServiceTime(bestResult) << "\n";
	}*/
	for (int i = 0; i < bestResult.size(); i++)
	{
		for (int j = 0; j < bestResult[i].size(); j++) {
			file << bestResult[i][j] << ";";
		}
		file << "\n";
	}
	file.close();
}


int main()
{
	
	seconds fiveMinutes(1);
	vector<vector<int>> shopsDatabase;
	vector<vector<double>> trucksDatabase;
	vector<vector<double>> distance;
	int theBestFitIndex;
	vector<vector<double>> bestResult;
	vector<vector<int>> mainShopsDatabase;

	writeData(mainShopsDatabase);
	addFlag(mainShopsDatabase);
	vector<vector<double>> resultVector;
	vector<double> resultVectorBufor;
	for (int instances = 8; instances < mainShopsDatabase.size(); instances += 8)
	{
		auto start = high_resolution_clock::now();
		auto stop = high_resolution_clock::now();
		shopsDatabase = mainShopsDatabase;
		shopsDatabase.resize(instances);
		cout << shopsDatabase.size() << endl;
		int firstCheck = 1;
		double distanceDouble;
		double wait;
		for (int i = 1; i < shopsDatabase.size(); i++)
		{
			wait = 0;
			if ((shopsDatabase[0][5] < shopsDatabase[i][4]) || (shopsDatabase[0][4] > shopsDatabase[i][5]))
			{
				firstCheck = 0;
				break;
			}
			distanceDouble = sqrt(pow((shopsDatabase[0][1] - shopsDatabase[i][1]), 2) + pow((shopsDatabase[0][2] - shopsDatabase[i][2]), 2));

			if (shopsDatabase[0][4] + shopsDatabase[0][6] + distanceDouble < shopsDatabase[i][4])
			{
				wait = shopsDatabase[i][4] - (shopsDatabase[0][4] + shopsDatabase[0][6] + distanceDouble);
			}
			if (shopsDatabase[0][4] + shopsDatabase[0][6] + 2 * distanceDouble + shopsDatabase[i][6] + wait >= shopsDatabase[0][5])
			{
				firstCheck = 0;
				break;
			}
		}


		while (firstCheck == 1 && (duration_cast<seconds>(stop - start) < fiveMinutes)) {
			addTruck(trucksDatabase, capacity, shopsDatabase[0][1], shopsDatabase[0][2], shopsDatabase[0][4], shopsDatabase[0][6]);

			int indexWaiting = -1;
			double timewaiting = -1;

			while (makeDistanceVector(trucksDatabase, shopsDatabase, distance, indexWaiting, timewaiting))
			{   //While there are no more 0 in last column.
				 //Current track == trucksDatabase.back()
				if (distance.size() > 5)
				{
					QuickSort(distance, 0, distance.size() - 1);
					theBestFitIndex = drawNextClient(distance, 5);
					updateTheTrackDatabase(trucksDatabase.back(), shopsDatabase, (int)distance[theBestFitIndex][0], distance[theBestFitIndex][1]);
					shopsDatabase[(int)distance[theBestFitIndex][0]].back() = 1; //Mark shop as served
				}
				else if (0 < distance.size() && distance.size() <= 5)
				{
					theBestFitIndex = drawNextClient(distance, distance.size());
					updateTheTrackDatabase(trucksDatabase.back(), shopsDatabase, (int)distance[theBestFitIndex][0], distance[theBestFitIndex][1]);
					shopsDatabase[(int)distance[theBestFitIndex][0]].back() = 1; //Mark shop as served
				}
				else if (distance.size() == 0 && indexWaiting != -1 && timewaiting != -1)
				{
					theBestFitIndex = indexWaiting;
					updateTheTrackDatabase(trucksDatabase.back(), shopsDatabase, theBestFitIndex, timewaiting);
					shopsDatabase[theBestFitIndex].back() = 1; //Mark shop as served
				}

				distance.clear();
				indexWaiting = -1;
				timewaiting = -1;
			}

			trucksDatabase.back()[3] += calculateDistance(trucksDatabase.back(), shopsDatabase[0]); //update last truck
			selectBetterResult(bestResult, trucksDatabase);
			//Clear before next iteration
			ResetVisitedFlag(shopsDatabase);
			trucksDatabase.clear();
			distance.clear();

			//
			stop = high_resolution_clock::now();
		}
		resultVectorBufor.clear();
		resultVectorBufor.push_back(instances);
		resultVectorBufor.push_back(bestResult.size());
		resultVectorBufor.push_back(calculateSumServiceTime(bestResult));
		resultVector.push_back(resultVectorBufor);

		bestResult.clear();
		//printTrucks(bestResult);
	}
	saveToFile(resultVector, "file.txt");
	//printTrucks(bestResult);
	system("pause");
	return 0;
}
