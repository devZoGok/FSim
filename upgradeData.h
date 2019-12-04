#ifndef UPGRADE_DATA_H
#define UPGRADE_DATA_H

#include<string>

namespace fsim{
	static const int numUpgrades=5,numLevels=4;

	static const std::string tabNames[]={"Fighter","Fighter_bomber","Helicopter"};

	static const std::string upgrades[][numUpgrades]={
	   {"Airframe","Fuel_tank","Machinegun","AAMs","Countermeasures"},
	   {"Airframe","Fuel_tank","Machinegun","Bombs","Countermeasures"},
	   {"Airframe","Fuel_tank","Machinegun","ASMs","Countermeasures"}
	};

	static const int price=100;
}

#endif
