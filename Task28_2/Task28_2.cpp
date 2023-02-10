#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>
#include<string>
#include<vector>

std::mutex expectation, printOut;
class Train
{
private:
	int travelTime = 0;
	char name;
public:
	void setTime(int& inTime)
	{
		travelTime = inTime;
	}
	int getTime()
	{
		return travelTime;
	}
	void setName(char& inName)
	{
		name = inName;
	}
	void trainMovement()
	{
		std::string command;
		printOut.lock();
		std::cout << "The train "<< name <<" has left for its destination " << std::endl;
		printOut.unlock();
		while (travelTime >= 0)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			travelTime--;
			if (travelTime == 0)
			{
				printOut.lock();
				std::cout << "Train " << name << " is waiting" << std::endl;
				printOut.unlock();
				expectation.lock();
				std::cout << "Train " << name << " has arrived at the station" << std::endl;
				std::cout << "To depart the train, enter : depart - " << std::endl;
				std::cin >> command;
				while (command != "depart")
				{
					std::cout << "Incorrect input!!!" << std::endl;
					std::cout << "To depart the train, enter : depart - " << std::endl;
					std::cin >> command;
				}
				printOut.lock();
				std::cout << "Train " << name << " has left the station" << std::endl;
				printOut.unlock();
				expectation.unlock();
			}
		}

	}
};

int main()
{
	const int numTrain = 3;
	char name='A';
	int timeTravel = 0;
	std::vector<Train> train;
	for (int i = 0; i < numTrain; i++)
	{
		train.push_back(Train());
	}
	for (int i = 0; i < numTrain; i++, name++)
	{
		train[i].setName(name);
		std::cout << "Enter the travel time of train "<<name<<" to the railway station(sec) :";
		std::cin >> timeTravel;
		train[i].setTime(timeTravel);
	}
	std::vector<std::thread> threads;
	for (int i = 0; i < numTrain; i++)
	{
		threads.emplace_back(&Train:: trainMovement, train[i]);
	}
	for (auto& temp : threads)
	{
		temp.join();
	}
	return 0;
}