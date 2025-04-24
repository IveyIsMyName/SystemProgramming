#include<Windows.h>
#include<iostream>
#include<conio.h>
#include<thread>
#include<atomic>
#include<chrono>
using namespace std;

#define Escape 27
#define Enter 13
#define Space 32


#define MIN_TANK_CAPACITY 20
#define MAX_TANK_CAPACITY 120

#define MIN_CONSUMPTION	3
#define MAX_CONSUMPTION 25
#define BASE_CONSUMPTION 10

class Tank
{
	const int CAPACITY;
	double fuel_level;
public:
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
		fuel_level(CAPACITY * 0.5)
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
	double give_fuel(double amount)
	{
		if (amount < 0)return 0;
		if (fuel_level >= amount)
		{
			fuel_level -= amount;
			return amount;
		}
		else
		{
			double remaining = fuel_level;
			fuel_level = 0;
			return remaining;
		}
	}
	void info()const
	{
		cout << "Capacity:\t" << CAPACITY << " liters\n";
		cout << "Fuel level:\t" << fuel_level << " liters\n";
	}
};

class Engine
{
	const double CONSUMPTION; //расход на 100 км.
	const double DEFAULT_CONSUMPTION_PER_SECOND;
	double consumption_per_second;
	bool is_started;
public:
	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}
	Engine(double consumption) :
		CONSUMPTION
		(
			consumption < MIN_CONSUMPTION ? MIN_CONSUMPTION :
			consumption > MAX_CONSUMPTION ? MAX_CONSUMPTION :
			consumption
		),
		DEFAULT_CONSUMPTION_PER_SECOND(CONSUMPTION * 3e-5),
		consumption_per_second(DEFAULT_CONSUMPTION_PER_SECOND)
	{
		is_started = false;
		cout << "Engine is ready" << endl;
	}
	~Engine()
	{
		cout << "Engine is over" << endl;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption: " << CONSUMPTION << " liters/100km\n";
		cout << "Consumption per second: " << DEFAULT_CONSUMPTION_PER_SECOND << " liters/s\n";
		cout << "Consumption: " << consumption_per_second << " liters/s\n";
	}
};

#define MAX_SPEED_LOWER_LIMIT  130
#define MAX_SPEED_HIGHER_LIMIT 408
class Car
{
	Engine engine;
	Tank tank;
	bool engine_running;
	bool driver_inside;
	bool panel_active;
	int speed;
	const int MAX_SPEED;

public:
	Car(double consumption, int capacity, int max_speed = 250) :
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed > MAX_SPEED_HIGHER_LIMIT ? MAX_SPEED_HIGHER_LIMIT :
			max_speed
		),
		engine(consumption),
		tank(capacity),
		speed(0),
		engine_running(false),
		driver_inside(false),
		panel_active(false)
	{
		driver_inside = false;
		cout << "Your car is ready to go, press 'Enter' to get in ;-)" << endl;
	}
	~Car()
	{
		cout << "The car is over" << endl;
	}
	void engine_working() 
	{
		while (engine_running) 
		{
			if (tank.get_fuel_level() <= 0) 
			{
				engine_running = false;
				break;
			}
			double consumption = engine.get_consumption_per_second();
			tank.give_fuel(consumption);
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
	void panel_working() 
	{
		while (panel_active) 
		{
			system("cls");
			cout << "Fuel level: " << tank.get_fuel_level() << " liters\n";
			cout << "Engine status: " << (engine_running ? "RUNNING" : "STOPPED") << "\n";
			cout << "Consumption: " << engine.get_consumption_per_second() << " liters/s\n";
			cout << "Speed: " << speed << " km/h\n";
			cout << "Press SPACE to stop engine\n";
			cout << "Press ENTER to exit car\n";
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
	void info()const
	{
			tank.info();
			engine.info();
			cout << "Speed: " << speed << " km/h\n";
			cout << "MaxSpeed: " << MAX_SPEED << " km/h\n";
	}

	/*void get_in()
	{
		driver_inside = true;
		panel();
	}
	void get_out()
	{
		driver_inside = false;
	}*/
	void control() {
		char key = 0;
		do 
		{
			key = _getch();
			switch (key) 
			{
			case Enter:
				if (!driver_inside) 
				{
					driver_inside = true;
					panel_active = true;
					engine_running = true;
					thread(&Car::panel_working, this).detach();
					thread(&Car::engine_working, this).detach();
				}
				else 
				{
					driver_inside = false;
					panel_active = false;
					engine_running = false;
				}
				break;
			case Space:
				engine_running = !engine_running;
				break;
			}

		} while (key != Escape);
	}
};




//#define TANK_CHECK
//#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	Tank tank(80);
	double fuel;
	do
	{
		cout << "На сколько запрявляемся? "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (true);
#endif // DEBUG

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK
	Car bmw(10, 80, 270);
	bmw.control();
	//bmw.info();
}