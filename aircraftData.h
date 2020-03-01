#ifndef AIRCRAFT_DATA_H
#define AIRCRAFT_DATA_H

#include"structureData.h"
namespace fsim{
	namespace aircraftData{
		static const int rateOfPrimaryFire[numStructures]{100,100,100,100,100,100,100,100,100};		
		static const int rateOfSecondaryFire[numStructures]{100,100,100,100,100,100,100,100,100};		
		static const int rateOfChaff[numStructures]{100,100,100,100,100,100,100,100,100};		
		static const int primaryAmmo[numStructures]{100,100,100,100,100,100,100,100,100};
		static const int secondaryAmmo[numStructures]{10,10,10,10,10,10,10,10,10};
		static const int chaff[numStructures]{10,10,10,10,10,10,10,10,10};

		static const int fuelCapacity[numStructures]{100,100,100,100,100,100,100,100,100};
	}
}

#endif
