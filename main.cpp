#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "functions.h"

using namespace std;

static int capacity; // Ugly. To change. Do you have an idea how do it nicely?

// Should we use references?
vector<vector<int>> write(vector<vector<int>> fleet)
{
	string line, newword;
	vector<int> truck;
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
					truck.push_back(data);


					for (int i = 0; i < 6; i++)
					{
						file >> data;
						truck.push_back(data);

					}
					fleet.push_back(truck);
					truck.clear();
				}
			}

		}


	}

	file.close();
	return fleet;
}

vector<vector<int>> addVisitedFlag(vector<vector<int>> fleet)
{
	for (int j = 1; j < fleet.size(); j++)
	{
        fleet[j].push_back(0);
        cout << fleet[j][7] << "\n";
	}
	return fleet;
}

void printFleet(vector<vector<int>> fleet)
{
    for (int j = 0; j < fleet.size(); j++)
	{
		for (int i = 0; i <= fleet[0].size(); i++)
		{
			cout << fleet[j][i]<<" ";
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

bool thereAreShopsToVisit(vector<vector<int>> fleet)
{
    for (int j = 1; j < fleet.size(); j++)
        {
            if(fleet[j][7] == 0)
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
	vector<vector<int>> shopsDatabase;
	vector<vector<int>> trucks;


	shopsDatabase = write(shopsDatabase);
	shopsDatabase = addVisitedFlag(shopsDatabase);

	cout << "shopsDatabase:" << endl;
    printFleet(shopsDatabase);

    addTruck(trucks, capacity, shopsDatabase[0][1], shopsDatabase[0][2]);
    while(thereAreShopsToVisit(shopsDatabase))
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
    cout << "Trucks:" << endl;
    printTrucks(trucks);

	system("pause");
	return 0;
}
