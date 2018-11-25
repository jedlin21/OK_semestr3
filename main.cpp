
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

void writeData(vector<vector<int>> & shopDatabase, string fileName) // to improve - fail if data is damaged 
{
	string line, newword; //temporary, only to help read proper data
	vector<int> shop;
	int data;

	ifstream file;

	file.open(fileName, ios::in);
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

void printTruck(vector<double> truck)//unnecessary printShops is identical
{
	for (int i = 0; i < truck.size(); i++)
	{
		cout << truck[i] << " ";
	}
	cout << endl;
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

		if (shopDatabase[i].back() == 0)// shop must be not visited 
		{
			thereAreshopsToVisitbool = true;
			if (truck[0] >= shopDatabase[i][3]) // truck has enough capacity
			{
				enoughCapacity = true;
				vector<double> record;
				dataX = shopDatabase[i][1];
				dataY = shopDatabase[i][2];
				distanceDouble = sqrt(pow((dataX - truckX), 2) + pow((dataY - truckY), 2));
				double depotDistance = sqrt(pow((dataX - shopDatabase[0][1]), 2) + pow((dataY - shopDatabase[0][2]), 2));

				if ((shopDatabase[i][4] <= distanceDouble + truck[3]) && (distanceDouble + truck[3] < shopDatabase[i][5]) && truck[3] + distanceDouble + shopDatabase[i][6] + depotDistance <= shopDatabase[0][5]) //open window <=dis+ current truck time < close window
				{
					record.push_back((double)i);
					record.push_back(distanceDouble);
					distance.push_back(record);
				}
				else if ((distanceDouble + truck[3] < shopDatabase[i][4]) && (truck[3] + distanceDouble + shopDatabase[i][4] - (truck[3] + distanceDouble) + shopDatabase[i][6] + depotDistance <= shopDatabase[0][5]))
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
		if (!enoughCapacity || (indexWaiting == -1 && distanceWaiting == -1 && distance.size() == 0))
		{
			trucks.back()[3] += calculateDistanceToBase(truck, shopDatabase[0]);
			addTruck(trucks, capacity, shopDatabase[0][1], shopDatabase[0][2], shopDatabase[0][4], shopDatabase[0][6]);
		}
		return true;
	}
	else
		return false;
}

double calculateDistanceToBase(vector<double> first, vector<int> second)
{
	double firstX = first[1];
	double firstY = first[2];
	int secondX = second[1];
	int secondY = second[2];
	return sqrt(pow((secondX - firstX), 2) + pow((secondY - firstY), 2));
}

double calculateDistance(int firstX, int firstY, vector<int> second)
{
	int secondX = second[1];
	int secondY = second[2];
	return sqrt(pow((secondX - firstX), 2) + pow((secondY - firstY), 2));
}

void selectBetterResult(vector<vector<double>> & bestResult, vector<vector<double>> trucksDatabase, string mode = "sumTime") {
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
		cout << "Wrong mode exception" << endl;
}

double calculateSumServiceTime(vector<vector<double>> trucksDatabase)
{
	double serviceTime = 0;
	for (int i = 0; i < trucksDatabase.size(); i++)
		serviceTime += trucksDatabase[i][3];
	return serviceTime;
}

int drawNextClient(int rangeDistance)
{
	int chosen = rand() % rangeDistance;
	return chosen;
}

void drawNextTrucksToMix(int range, vector<int> & chosen)
{
	chosen.clear();

	int chosen1 = rand() % range;
	int chosen2;
	while ((chosen2 = rand() % range) == chosen1)
		chosen2 = rand() % range;
	chosen.push_back(chosen1);
	chosen.push_back(chosen2);
}


void saveToFile(vector<vector<double>> bestResult, string fileName)
{
	ofstream file;
	file.precision(26);
	cout.precision(16);
	file.open(fileName);
	if (bestResult.size() == 0)
		file << "-1" << "\n";
	else
	{
		file << bestResult.size() << " " << calculateSumServiceTime(bestResult) << "\n";
		cout << bestResult.size() << " " << calculateSumServiceTime(bestResult) << "\n";
	}
	for (int i = 0; i < bestResult.size(); i++)
	{
		for (int j = 4; j < bestResult[i].size(); j++) {
			file << bestResult[i][j] << " ";
		}
		file << "\n";
	}
	file.close();
}

vector<vector<double>> findFirstTrucksDatabase(vector<vector<int>> shopsDatabase, int time)
{
	seconds worksTime(time);
	int theBestFitIndex = 0;
	vector<vector<double>> distance;
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	vector<vector<double>> bestResult;
	vector<vector<double>> trucksDatabase;

	while (duration_cast<seconds>(stop - start) < worksTime) {
		addTruck(trucksDatabase, capacity, shopsDatabase[0][1], shopsDatabase[0][2], shopsDatabase[0][4], shopsDatabase[0][6]);

		int indexWaiting = -1;
		double timewaiting = -1;

		while (makeDistanceVector(trucksDatabase, shopsDatabase, distance, indexWaiting, timewaiting))
		{   //While there are no more 0 in last column.
			 //Current track == trucksDatabase.back()
			if (distance.size() > 5)
			{
				QuickSort(distance, 0, distance.size() - 1);
				theBestFitIndex = drawNextClient(5);
				updateTheTrackDatabase(trucksDatabase.back(), shopsDatabase, (int)distance[theBestFitIndex][0], distance[theBestFitIndex][1]);
				shopsDatabase[(int)distance[theBestFitIndex][0]].back() = 1; //Mark shop as served
			}
			else if (0 < distance.size() && distance.size() <= 5)
			{
				theBestFitIndex = drawNextClient(distance.size());
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

		trucksDatabase.back()[3] += calculateDistanceToBase(trucksDatabase.back(), shopsDatabase[0]); //update last truck
		selectBetterResult(bestResult, trucksDatabase);
		//Clear before next iteration
		ResetVisitedFlag(shopsDatabase);
		trucksDatabase.clear();
		distance.clear();

		stop = high_resolution_clock::now();
	}
	return bestResult;
}

bool truckManage(vector<double> & truck, vector<vector<int>> shopsDatabase)
{
	double actualTime = shopsDatabase[0][4]; // depo open
	int actualX = shopsDatabase[0][1];       // depo X
	int actualY = shopsDatabase[0][2];       // depo Y
	int actualCapacity = 0;
	for (int i = 4; i < truck.size(); i++)
	{
		actualTime += calculateDistance(actualX, actualY, shopsDatabase[truck[i]]);
		actualCapacity += shopsDatabase[truck[i]][3];
		//check if truck fit to time window
		if (actualTime > shopsDatabase[truck[i]][5]) {
			//cout << actualTime << " "<< shopsDatabase[truck[i]][5] << "  1"<< "  " << i  << endl;
			return false;
		}

		if (actualTime < shopsDatabase[truck[i]][4])
			actualTime = shopsDatabase[truck[i]][4];
		actualTime += shopsDatabase[truck[i]][6];
		actualX = shopsDatabase[truck[i]][1];
		actualY = shopsDatabase[truck[i]][2];

	}
	if (actualCapacity > capacity)
		return false;

	actualTime += calculateDistance(actualX, actualY, shopsDatabase[0]);
	if (actualTime > shopsDatabase[0][5]) {
		//cout << "2" << endl;
		return false;
	}
	else
		truck[3] = actualTime;
		return true;
}


void mixTracks(vector<vector<double>> & trucksDatabase, vector<int> & chosen, vector<vector<int>> shopsDatabase)
{
	int firstSlice;
	while ((firstSlice = rand() % trucksDatabase[chosen[0]].size()) < 4)
		continue;
	int secondSlice;
	while ((secondSlice = rand() % trucksDatabase[chosen[1]].size()) < 4)
		continue;

	vector<double>::const_iterator first = trucksDatabase[chosen[0]].begin();
	vector<double>::const_iterator last = trucksDatabase[chosen[0]].begin() + firstSlice;
	vector<double> newFirstTruck(first, last); //only left part
	first = trucksDatabase[chosen[1]].begin() + secondSlice;
	last = trucksDatabase[chosen[1]].end();
	newFirstTruck.insert(newFirstTruck.end(), first, last);

	first = trucksDatabase[chosen[1]].begin();
	last = trucksDatabase[chosen[1]].begin() + secondSlice;
	vector<double> newSecondTruck(first, last); //only left part
	first = trucksDatabase[chosen[0]].begin() + firstSlice;
	last = trucksDatabase[chosen[0]].end();
	newSecondTruck.insert(newSecondTruck.end(), first, last);

	/*cout << "truck" << endl;
	cout << firstSlice << " " << secondSlice << endl;
	printTruck(trucksDatabase[chosen[0]]);
	printTruck(trucksDatabase[chosen[1]]);
	printTruck(newFirstTruck);
	printTruck(newSecondTruck);
	cout << endl ;*/


	if (truckManage(newFirstTruck, shopsDatabase) && truckManage(newSecondTruck, shopsDatabase))
	{
		trucksDatabase[chosen[0]] = newFirstTruck;
		trucksDatabase[chosen[1]] = newSecondTruck;
		/*cout << "change" << endl;
		cout << "truck" << endl;
		cout << firstSlice << " " << secondSlice << endl;
		printTruck(trucksDatabase[chosen[0]]);
		printTruck(trucksDatabase[chosen[1]]);
		printTruck(newFirstTruck);
		printTruck(newSecondTruck);
		cout << endl;*/
		cout.precision(16);
		cout << trucksDatabase.size() << " " << calculateSumServiceTime(trucksDatabase) << endl;
		/*printTruck(trucksDatabase[chosen[0]]);
		printTruck(trucksDatabase[chosen[1]]);*/
	}
	
}

int main(int argc, char * argv[])
{
	srand(time(NULL));
	string fileName = "input.txt";
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	seconds fiveMinutes(100);
	int howManySeondsForGRASP = 20;
	vector<vector<int>> shopsDatabase;
	vector<vector<double>> bestResult;
	vector<vector<double>> trucksDatabase;

	writeData(shopsDatabase, fileName);
	addFlag(shopsDatabase);

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

	if (firstCheck == 1)
		trucksDatabase = findFirstTrucksDatabase(shopsDatabase, howManySeondsForGRASP);
	cout << trucksDatabase.size() << " " << calculateSumServiceTime(trucksDatabase) << "\n";
	while (trucksDatabase.size() > 0 && (duration_cast<seconds>(stop - start) < fiveMinutes)) {

		//I have trucksDatabase here. What should I do next?
		//hmmm
		//You should take 2 trucks and mix them.
		//Then you check if database is correct?
		//when it is you check if that result is beter ... or you collect a few result and after X(tune-upped) try you chose which are better.
		//and when it isn't correct, then randomly chose next 
		//capacity
		//okno czasowe przy zmianie
		//

		int X = 0;
		int range = trucksDatabase.size();
		vector<int> chosen;
		vector<vector<double>> buforTrucksDatabase;


		buforTrucksDatabase = trucksDatabase;
		drawNextTrucksToMix(range, chosen);

		mixTracks(buforTrucksDatabase, chosen, shopsDatabase);
		selectBetterResult(trucksDatabase, buforTrucksDatabase);

		/*cout.precision(16);
		cout << trucksDatabase.size() << " " << calculateSumServiceTime(trucksDatabase) << "\n";*/

		stop = high_resolution_clock::now();
	}
	cout << "end" << endl;
	saveToFile(trucksDatabase, "file.txt");

	//printTrucks(trucksDatabase);
	system("pause");
	return 0;
}
