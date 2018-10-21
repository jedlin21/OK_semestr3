#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

vector<vector<int>> write(vector<vector<int>> fleet)
{
	int capacity;
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
	for (int j = 0; j < fleet.size(); j++)
	{
		for (int i = 0; i < 7; i++)
		{
			cout << fleet[j][i]<<" ";
		}
		cout << endl;
	}

	return fleet;
}
int main()
{
	vector<vector<int>> first;

	
	write(first);

	system("pause");
	return 0;
}