
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "functions.h"
#include <chrono>;

using namespace std;
using namespace std::chrono;

static int capacity; // Ugly. To change. Do you have an idea how do it nicely?

// Should we use references?
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

void addFlag(vector<vector<int>> & shopDatabase)
{
	for (int j = 0; j < shopDatabase.size(); j++)
	{
		shopDatabase[j].push_back(0);
	}
	
}

vector<vector<int>> ResetVisitedFlag(vector<vector<int>> dataBase)
{
	for (int j = 0; j < dataBase.size(); j++)
	{
		dataBase[j].back() = 0;
	}
	return dataBase;
}

void printShops(vector<vector<int>> dataBase)
{
    for (int j = 0; j < dataBase.size(); j++)
	{
		for (int i = 0; i < dataBase[0].size(); i++)
		{
			cout << dataBase[j][i]<<" ";
		}
		cout << endl;
	}
}

void printTrucks(vector<vector<int>> fleet)
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

//int[] keySort(vector<int> & truck, vector<vector<int>> & shopsDatabase)
//{
//void change(int *a, int *b) {
//	int tmp = *a;
//	*a = *b;
//	*b = tmp;
//}
//
//void QuickSort(int *tab, int i, int j)
//{
//	int left = i;
//	int right = j;
//	int index = (i + j) / 2;
//	int pivot = tab[index];
//	while (i <= j)
//	{
//		while (tab[i] < pivot)
//			i++;
//		while (tab[j] > pivot)
//			j--;
//
//		if (i <= j)
//		{
//			change(&tab[i], &tab[j]);
//			i++;
//			j--;
//		}
//	}
//	if (left < j) QuickSort(tab, left, j);
//
//	if (right > i) QuickSort(tab, i, right);
//}
//
//int main() {
//	int amount, range;
//	int *tab;
//
//	
//	int left = 0;
//	int right = amount - 1;
//	
//	QuickSort(tab, left, right);
//	
//}
//    int distans[shopsDatabase.size()];
//    // make distance table
//    // delete visited shops
//    // sort distance table
//    // return the best 5 of them
//}

void updateTheTrackDatabase(vector<int> & truck, vector<vector<int>> & shopsDatabase, int theBestFitIndex)
{
    truck[0] -= shopsDatabase[theBestFitIndex][3]; //capacity
    truck[1] = shopsDatabase[theBestFitIndex][1];  // xCoord
    truck[2] = shopsDatabase[theBestFitIndex][2]; // yCoord
    truck[3] += shopsDatabase[theBestFitIndex][6];  //time
    truck.push_back(theBestFitIndex);
}

int main()
{
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	seconds fiveMinutes(300);
	vector<vector<int>> shopsDatabase;
	vector<vector<int>> trucks;
	int left, right;


	writeData(shopsDatabase);
	addFlag(shopsDatabase);
	left = 0;
	right = shopsDatabase.size() - 1;

	cout << "shopsDatabase:" << endl;
    printShops(shopsDatabase);
	addTruck(trucks, capacity, shopsDatabase[0][1], shopsDatabase[0][2]);
	while (duration_cast<seconds>(stop - start) < fiveMinutes) {
		

		while (thereAreShopsToVisit(shopsDatabase))
		{   //While there are no more 0 in last column.
			// Current track == tracks.back()
			for (int j = 1; j < shopsDatabase.size(); j++) // condition to change
			{
				int theBestFitIndex = findTheBestFit(trucks, shopsDatabase);
				updateTheTrackDatabase(trucks.back(), shopsDatabase, theBestFitIndex);
				//Mark shop as served
				shopsDatabase[theBestFitIndex].back() = 1;
			}
		}
		//cout << "Trucks:" << endl;
		//printTrucks(trucks);
		ResetVisitedFlag(shopsDatabase);
		stop = high_resolution_clock::now();
	}


	system("pause");
	return 0;
}
