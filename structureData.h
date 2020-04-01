#ifndef STRUCTURE_DATA_H
#define STRUCTURE_DATA_H

#include<string>

namespace fsim{
	namespace structureData{
		const int numStructures=12;

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
			SAM_SITE,
			RUNWAY,
			HELIPAD
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
			1
		};
		static const std::string path[numStructures]={
			"Models/Aircraft/FighterBombers/fighterBomber00",
			"Models/Aircraft/FighterBombers/fighterBomber00",
			"Models/Aircraft/Helicopters/helicopter00",
			"Models/Aircraft/Fighters/fighter01",
			"Models/Aircraft/FighterBombers/fighterBomber01",
			"Models/Aircraft/Helicopters/helicopter01",
			"Models/Aircraft/Fighters/fighter02",
			"Models/Aircraft/FighterBombers/fighterBomber02",
			"Models/Aircraft/Helicopters/helicopter02",
			"Models/Buildings/SAMSite/samSite",
			"Models/Buildings/Airfield/hangar",
			"Models/Buildings/Airfield/helipad"
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
			"Models/Buildings/SAMSite/samSiteHitbox",
			"Models/Buildings/Airfield/hangarHitbox",
			"Models/Buildings/Airfield/helipadHitbox"
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
			"samSite.png",
			"runway.png",
			"helipad.png"
		};
	}
}

#endif
