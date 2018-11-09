
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "functions.h"
#include <chrono>;
#include <math.h>

using namespace std;
using namespace std::chrono;

static int capacity; // Ugly. To change. Do you have an idea how do it nicely?


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
			cout << "capacity "<<capacity << endl;
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
	for (int j = 0; j < shopDatabase.size(); j++)
	{
		shopDatabase[j].push_back(0);
	}
	cout << "print" << endl;
	printShops(shopDatabase);
}

vector<vector<int>> ResetVisitedFlag(vector<vector<int>> dataBase)
{
	for (int j = 0; j < dataBase.size(); j++)
	{
		dataBase[j].back() = 0;
	}
	return dataBase;
}


void printTrucks(vector<vector<int>> fleet)//unnecessary printShops is identical
{
    for (int j = 0; j < fleet.size(); j++)
	{
		for (int i = 0; i < fleet[0].size(); i++)
		{
			cout << fleet[j][i]<<" ";
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
            if(shopDatabase[j][7] == 0)
                return true;
        }
    return false;
}

void addTruck(vector<vector<int>> & trucks, int capacity, int xCoord, int yCoord)
{   // Add new truck to truckDatabase
    //truck = [capacity, xCoord, yCoord, actualTime ... shops visited ...]
    vector<int> truck;
    truck.push_back(capacity);
    truck.push_back(xCoord);
    truck.push_back(yCoord);
    truck.push_back(0);      //time
    trucks.push_back(truck);
}

int findTheBestFit(vector<vector<int>> & trucks, vector<vector<int>> & shopsDatabase)
{
    int theBestFitIndex;
    int xTrackCoord = trucks.back()[1];
    int yTrackCoord = trucks.back()[2];

    //Heuristic
    for (int j = 1; j < shopsDatabase.size(); j++)  //To implement.
        {                                           //To implement.
            if(shopsDatabase[j].back() == 0)        //To implement.
            {                                       //To implement.
                theBestFitIndex = j;                //To implement.
                return theBestFitIndex;             //To implement.
            }                                       //To implement.
        }                                           //To implement.
}


void QuickSort(vector<vector<double>> & distance, int i, int j)
{
	int left = i;
	int right = j;
	int index = (i + j) / 2;
	double pivot = distance[index][1];
	cout << "pivot" << pivot << endl;
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



void updateTheTrackDatabase(vector<int> & truck, vector<vector<int>> & shopsDatabase, int theBestFitIndex)
{
    truck[0] -= shopsDatabase[theBestFitIndex][3]; //capacity
    truck[1] = shopsDatabase[theBestFitIndex][1];  // xCoord
    truck[2] = shopsDatabase[theBestFitIndex][2]; // yCoord
    truck[3] += shopsDatabase[theBestFitIndex][6];  //time
    truck.push_back(theBestFitIndex);
}

void calculateDistance(vector<int> & truck, vector<vector<int>> & shopDatabase, vector<vector<double>> & distance)
{
	int truckX = truck[1];
	int truckY = truck[2];
	int dataX;
	int dataY;
	double dis;

	
	
	for (int i = 1; i < shopDatabase.size(); i++)
	{
		if (shopDatabase[i].back() == 0 && truck[0] >= shopDatabase[i][3])// shop must be not visited and truck has enough capacity
		{
			vector<double> record;
			dataX = shopDatabase[i][1];
			dataY = shopDatabase[i][2];
			dis = sqrt(pow((dataX - truckX), 2) + pow((dataY - truckY), 2));
			if ((double)shopDatabase[i][4] <= dis + (double)truck[3] <= (double)shopDatabase[i][5]) //open window <=dis+ current truck time < close window
			{
				record.push_back((double)i);
				record.push_back(dis);
				distance.push_back(record);
			}
			 
			
		}
		
	}

	printDistance(distance);
	
}



int main()
{
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	seconds fiveMinutes(300);
	vector<vector<int>> shopsDatabase;
	vector<vector<int>> trucks;
	int left, right;
	vector<vector<double>> distance;


	writeData(shopsDatabase);
	addFlag(shopsDatabase);

	
	

	
	cout << "shopsDatabase:" << endl;
    printShops(shopsDatabase);
	
	//while (duration_cast<seconds>(stop - start) < fiveMinutes) {
	addTruck(trucks, capacity, shopsDatabase[0][1], shopsDatabase[0][2]);

		//while (thereAreShopsToVisit(shopsDatabase))
		//{   //While there are no more 0 in last column.
			// Current track == tracks.back()

			calculateDistance(trucks.back(), shopsDatabase, distance);
			left = 0;
			right = distance.size() - 1;
			QuickSort(distance, left, right);
			// random dla 5 wynikow, dodac do trucka - pamietac o serwis time
			

			//sort
			//for (int j = 1; j < shopsDatabase.size(); j++) // condition to change
			//{
			//	int theBestFitIndex = findTheBestFit(trucks, shopsDatabase);
			//	updateTheTrackDatabase(trucks.back(), shopsDatabase, theBestFitIndex);
			//	//Mark shop as served
			//	shopsDatabase[theBestFitIndex].back() = 1;
			//}
		//}
		//cout << "Trucks:" << endl;
		//printTrucks(trucks);
		ResetVisitedFlag(shopsDatabase);
		stop = high_resolution_clock::now();
	//}


	system("pause");
	return 0;
}
