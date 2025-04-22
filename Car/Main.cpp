#include<iostream>
using namespace std;

#define MIN_TANK_CAPACITY 20
#define MAX_TANK_CAPACITY 120

#define MIN_CONSUMPTION	5
#define MAX_CONSUMPTION 30
#define BASE_CONSUMPTION 10

class Tank
{
	double fuel_level;
public:
	const int CAPACITY;
	double get_fuel_level()const
	{
		return fuel_level;
	}
	Tank(int capacity) : CAPACITY
	(
		capacity < MIN_TANK_CAPACITY ? MIN_TANK_CAPACITY :
		capacity > MAX_TANK_CAPACITY ? MAX_TANK_CAPACITY :
		capacity
	),
		fuel_level(0)
	{
		cout << "Tank:";
		if (capacity < MIN_TANK_CAPACITY)cout << "Min capacity was applied" << endl;
		if (capacity > MAX_TANK_CAPACITY)cout << "Max capacity was applied" << endl;

		//this->fuel_level = 0;
		cout << "Tank is ready" << endl;
	}
	~Tank()
	{
		cout << "Tank is over" << endl;
	}
	double fill(double amount)
	{
		if (amount < 0)return fuel_level;
		fuel_level += amount;
		if (fuel_level > CAPACITY)
		{
			fuel_level = CAPACITY;
			cout << "Tank is full" << endl;
		}
		return fuel_level;
	}
	void info()const
	{
		cout << "Capacity:\t" << CAPACITY << " liters\n";
		cout << "Fuel level:\t" << fuel_level << " liters\n";
	}
};

class Engine
{
	double consumption_per_sec;
public:
	const double CONSUMPTION_PER_100KM;

	Engine(double consumption) : CONSUMPTION_PER_100KM
	(
		consumption < MIN_CONSUMPTION ? MIN_CONSUMPTION :
		consumption > MAX_CONSUMPTION ? MAX_CONSUMPTION :
		consumption
	)
	{
		consumption_per_sec = (CONSUMPTION_PER_100KM >= BASE_CONSUMPTION) ? CONSUMPTION_PER_100KM * 0.00003 : 0.0003;
		cout << "Engine is started" << endl;
	}
	~Engine()
	{
		cout << "Engine is stopped" << endl;
	}
	
	void info()const
	{
		cout << "Consumption per 100 km:\t" << CONSUMPTION_PER_100KM << " liters\n";
		cout << "Consumption per second:\t" << consumption_per_sec << " liters\n";
	}
};
void main()
{
	setlocale(LC_ALL, "");
	/*Tank tank(80);
	double fuel;
	do
	{
		cout << "На сколько запрявляемся? "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);*/
	
	Engine min_engine(5);
	min_engine.info();
	Engine base_engine(10);
	base_engine.info();
	Engine max_engine(30);
	max_engine.info();
}