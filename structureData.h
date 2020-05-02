#ifndef STRUCTURE_DATA_H
#define STRUCTURE_DATA_H

#include<string>

namespace fsim{
	namespace structureData{
		const int numStructures=18;

		enum Type{
			CHINESE_FIGHTER,
			CHINESE_FIGHTER_BOMBER,
			CHINESE_HELICOPTER,
			JAPANESE_FIGHTER,
			JAPANESE_FIGHTER_BOMBER,
			JAPANESE_HELICOPTER,
			KOREAN_FIGHTER,
			KOREAN_FIGHTER_BOMBER,
			KOREAN_HELICOPTER,
			CARGO_PLANE,
			CARGO_TRUCK,
			SSM_TRUCK,
			SAM_SITE,
			RUNWAY,
			HELIPAD,
			CHINESE_CARRIER,
			JAPANESE_CARRIER,
			KOREAN_CARRIER
		};

		static const std::string name[numStructures]{
			"chineseFighter",
			"chineseFighterBomber",
			"chineseHelicopter",
			"japaneseFighter",
			"japaneseFighterBomber",
			"japaneseHelicopter",
			"koreanFighter",
			"koreanFighterBomber",
			"koreanHelicopter",
			"cargoPlane",
			"cargoTruck",
			"ssmTruck",
			"samSite",
			"hangar",
			"helipad",
			"chineseCarrier",
			"japaneseCarrier",
			"koreanCarrier"
		};

		static const int hp[numStructures]={
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100,
			100
		};
		static const bool singleModel[numStructures]={
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			1,
			0,
			1,
			1,
			1,
			1,
			1
		};
		static const std::string path[numStructures]={
			"Models/Aircraft/Fighters/fighter00",
			"Models/Aircraft/FighterBombers/fighterBomber00",
			"Models/Aircraft/Helicopters/helicopter00",
			"Models/Aircraft/Fighters/fighter01",
			"Models/Aircraft/FighterBombers/fighterBomber01",
			"Models/Aircraft/Helicopters/helicopter01",
			"Models/Aircraft/Fighters/fighter02",
			"Models/Aircraft/FighterBombers/fighterBomber02",
			"Models/Aircraft/Helicopters/helicopter02",
			"Models/Aircraft/CargoPlanes/cargoPlane",
			"Models/Vehicles/cargoTruck",
			"Models/Vehicles/ssmTruck",
			"Models/Buildings/SAMSite/samSite",
			"Models/Buildings/Airfield/hangar",
			"Models/Buildings/Airfield/helipad",
			"Models/Ships/aircraftCarrier01",
			"Models/Ships/aircraftCarrier01",
			"Models/Ships/aircraftCarrier02"
		};
		static const std::string hitboxPath[numStructures]={
			"Models/Aircraft/FighterBombers/fighterBomberHitbox00",
			"Models/Aircraft/FighterBombers/fighterBomberHitbox00",
			"Models/Aircraft/Helicopters/helicopterHitbox00",
			"Models/Aircraft/Fighters/fighterHitbox01",
			"Models/Aircraft/FighterBombers/fighterBomberHitbox01",
			"Models/Aircraft/Helicopters/helicopterHitbox01",
			"Models/Aircraft/Fighters/fighterHitbox02",
			"Models/Aircraft/FighterBombers/fighterBomberHitbox02",
			"Models/Aircraft/Helicopters/helicopterHitbox02",
			"Models/Aircraft/CargoPlanes/cargoPlaneHitbox",
			"Models/Vehicles/cargoTruckHitbox",
			"Models/Vehicles/ssmTruckHitbox",
			"Models/Buildings/SAMSite/samSiteHitbox",
			"Models/Buildings/Airfield/hangarHitbox",
			"Models/Buildings/Airfield/helipadHitbox",
			"Models/Ships/aircraftCarrierHitbox01",
			"Models/Ships/aircraftCarrierHitbox01",
			"Models/Ships/aircraftCarrierHitbox02"
		};
		static const std::string diffusePath[numStructures]={
			"Models/Aircraft/Fighters/fighter00",
			"Models/Aircraft/FighterBombers/fighterBomber00",
			"Models/Aircraft/Helicopters/helicopter00",
			"Models/Aircraft/Fighters/fighter01",
			"Models/Aircraft/FighterBombers/fighterBomber01",
			"Models/Aircraft/Helicopters/helicopter01",
			"Models/Aircraft/Fighters/fighter02",
			"Models/Aircraft/FighterBombers/fighterBomber02",
			"Models/Aircraft/Helicopters/helicopter02",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		};
		static const std::string iconPath[numStructures]={
			"jet.png",
			"jet.png",
			"heli.png",
			"jet.png",
			"jet.png",
			"heli.png",
			"jet.png",
			"jet.png",
			"heli.png",
			"target.png",
			"target.png",
			"target.png",
			"samSite.png",
			"runway.png",
			"helipad.png",
			"target.png",
			"target.png",
			"target.png"
		};
	}
}

#endif
