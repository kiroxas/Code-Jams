#include <iostream>
#include <sstream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <map>
#include <iomanip>

using namespace std;

int carstance(int xa, int ya, int xb, int yb) {
	return std::abs(xa - xb) + std::abs(ya - yb);
}


struct Ride
{
	int a;
	int b;
	int x;
	int y;
	int s;
	int f;
	int index;

	int distance() const
	{
		return carstance(a, b, x, y);
	}

	bool operator==(const Ride& o) const
	{
		return index == o.index;
	}
};

struct CarDuringSim
{
	int x;
	int y;
	int availableAt;
};

struct Car
{
	int m;
	std::vector<int> rides;
};

void ouputCar(const Car& c, ofstream& of)
{
	of << c.m << " ";

	for (int i : c.rides)
	{
		of << i << " ";
	}

	of << "\n";
}


int bestRideScore(const Ride& r, int bonus)
{
	return r.distance() + bonus;
}

int bestPossibleScore(const std::vector<Ride>& courses, int bonus)
{
	int maxScore = 0;

	for (const Ride& r : courses)
	{
		maxScore += bestRideScore(r, bonus);
	}

	return maxScore;
}

int timeToGoToStart(int x, int y, const Ride& r)
{
	return carstance(x, y, r.a, r.b);
}

int timeToGoToStart(const CarDuringSim& s, const Ride& r)
{
	return timeToGoToStart(s.x, s.y, r);
}

bool getBonus(const CarDuringSim& s, const Ride& r)
{
	int goThere = timeToGoToStart(s, r);

	return s.availableAt + goThere <= r.s;
}

int calculateFinishTime(const CarDuringSim& s, const Ride& r)
{
	int dis = r.distance();
	int goThere = timeToGoToStart(s, r);

	int begin = s.availableAt + goThere; //std::max(s.availableAt + goThere, r.s);

	return begin + dis;
}


// return index in carSim
int giveClosestFinish(std::vector<CarDuringSim>& sim, Ride& r, bool& bonus)
{
	int min = r.f, index = -1, distance = std::numeric_limits<int>().max();
	bonus = false;
	int i = 0;
	std::vector<int> indexes;

	indexes.push_back(-1);

	for (CarDuringSim& s : sim)
	{
		int f = calculateFinishTime(s, r);
		if (f < min) // will finish 
		{
			min = f;
			distance = timeToGoToStart(s.x, s.y, r);
			indexes.clear();
			indexes.push_back(i);
		}
		else if (f == min)
		{
			int dis = timeToGoToStart(s.x, s.y, r);
			if (dis < distance) // multiple available, take the closest one
			{
				indexes.clear();
				indexes.push_back(i);
				distance = dis;
			}
			else if (dis == distance)
			{
				indexes.push_back(i);
			}
			
		}
		++i;
	}

	/*
	CarDuringSim& s = sim[index];

	bonus = getBonus(s, r);

	s.availableAt = calculateFinishTime(s, r);*/
	random_shuffle(begin(indexes), end(indexes));

	return indexes[0];
}

struct Score
{
	int rideIndex;
	int score;
};


void solve(std::vector<CarDuringSim>& sim, std::vector<Ride> courses, std::vector<Car>& solution, int steps, int bonusScore)
{
	
	int count = courses.size();
	int oldCount = 45;
	while (count != 0 && count != oldCount)
	{
		oldCount = count;
		std::map<int, Score> score;
		int rideIndex = 0;
		for (int i = 0 ; i < count; ++i )
		{
			Ride& r = courses[i];
			bool bonus;
			int index = giveClosestFinish(sim, r, bonus);

			Score s;
			s.rideIndex = rideIndex;
			s.score = r.distance() + bonus ? bonusScore : 0;

			if (score.find(index) != score.end())
			{
				if (score[index].score < s.score)
					score[index] = s;
			}
			else
			{
				score[index] = s;
			}

			rideIndex++;
		}

		for (auto& m : score)
		{
			if (m.first != -1)
			{
				CarDuringSim& s = sim[m.first];
				s.availableAt = calculateFinishTime(s, courses[m.second.rideIndex]);

				solution[m.first].m++;
				solution[m.first].rides.push_back(m.second.rideIndex);
				std::remove(begin(courses), end(courses), courses[m.second.rideIndex]);
				count--;
				
			}
		}
	}
}

int calculateScore(const std::vector<Car>& solution, const std::vector<Ride>& rides, int bonus)
{
	int score = 0;

	for (const Car& c : solution)
	{
		int currentStep = 0;
		int carScore = 0 ;
		int x = 0, y = 0;
		for (int rid : c.rides)
		{
			const Ride& ride = *std::find_if(begin(rides), end(rides), [rid](const Ride& r) { return r.index == rid;});
			int stepToBegin = currentStep + timeToGoToStart(x, y, ride);

			if (stepToBegin <= ride.s)
				carScore += bonus;

			int dis = ride.distance();

			if (stepToBegin + dis < ride.f)
				carScore += dis;

			stepToBegin = std::max(stepToBegin, ride.s);

			currentStep = stepToBegin + ride.distance();
			x = ride.x;
			y = ride.y;
		}

		score += carScore;
	}

	return score;
}

int main()
{
	/*a_example*/
	/*b_should_be_easy*/
	/*c_no_hurry*/
	/*d_metropolis*/
	ifstream inputFile("d_metropolis.in");
	ofstream outputFile("d_example_out.txt");


	/* Handle input here */
	int rows = 0;
	int columns = 0;
	int vehicles = 0;
	int rides = 0;
	int bonus = 0;
	int steps = 0;

	inputFile >> rows >> columns >> vehicles >> rides >> bonus >> steps;

	std::vector<Ride> courses;

	for (int i = 1; i <= rides; ++i)
	{
		Ride r;
		inputFile >> r.a >> r.b >> r.x >> r.y >> r.s >> r.f;

		r.index = i - 1;

		courses.push_back(r);
	}

	std::sort(std::begin(courses), std::end(courses), [](const Ride& r1, const Ride& r2) 
	{
		//return r1.distance() < r2.distance();
		if (r1.s < r2.s)
			return true;
		else if (r1.s == r2.s)
			return r1.f < r2.f;

		return false;
	});


	//std::vector<CarDuringSim> simulation(vehicles);

	/* Solve here*/
	int maxAttainable = bestPossibleScore(courses, bonus);

	int bestScore = 5385799;
	int score = 0;

	std::vector<Car> solution(vehicles);

	//while (score <= bestScore)
	{
		solution = std::vector<Car>(vehicles);
		std::vector<CarDuringSim> simulation(vehicles);
		
		solve(simulation, courses, solution, steps, bonus);

		score = calculateScore(solution, courses, bonus);
		std::cout << score << "\n";
		//std::random_shuffle(std::begin(courses), std::end(courses));
	}

	/* Ouput here */
	for (Car& c : solution)
	{
		ouputCar(c, outputFile);
	}

	return 0;
}
