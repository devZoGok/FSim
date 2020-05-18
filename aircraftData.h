#ifndef AIRCRAFT_DATA_H
#define AIRCRAFT_DATA_H

#include"structureData.h"
#include"projectileData.h"

using namespace fsim::structureData;

namespace fsim{
	namespace aircraftData{
		static const int rateOfPrimaryFire[numStructures]{100,100,100,100,100,100,100,100,100};		
		static const int rateOfSecondaryFire[numStructures]{100,100,100,100,100,100,100,100,100};		
		static const int rateOfChaff[numStructures]{100,100,100,100,100,100,100,100,100};		
		static const int primaryAmmo[numStructures]{100,100,100,100,100,100,100,100,100};
		static const int secondaryAmmo[numStructures]{10,10,10,10,10,10,10,10,10};
		static const int chaff[numStructures]{10,10,10,10,10,10,10,10,10};

		static const int fuelCapacity[numStructures]{100,100,100,100,100,100,100,100,100};

		static const float pitchSpeed[numStructures]{
			.02,
			.01,
			.05,
			.02,
			.01,
			.05,
			.02,
			.01,
			.05,
		};
		static const float yawSpeed[numStructures]{
			.01,
			.01,
			.05,
			.05,
			.01,
			.01,
			.05,
			.01,
			.01,
		};
		static const float rollSpeed[numStructures]{
			.04,
			.03,
			.05,
			.04,
			.03,
			.05,
			.04,
			.03,
			.05,
		};

		static const int projectileId[numStructures]{
			projectileData::CHINESE_FIGHTER_AAM,
			projectileData::BOMB,
			projectileData::CHINESE_HELICOPTER_ASM,
			projectileData::JAPANESE_FIGHTER_AAM,
			projectileData::BOMB,
			projectileData::JAPANESE_HELICOPTER_ASM,
			projectileData::KOREAN_FIGHTER_AAM,
			projectileData::GPS_BOMB,
			projectileData::KOREAN_HELICOPTER_ASM
		};
	}
}

#endif
