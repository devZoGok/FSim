#ifndef STRUCTURE_DATA_H
#define STRUCTURE_DATA_H

#include<string>

namespace fsim{
	const int numStructures=11;

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
		AIRFIELD
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
		0
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
		"samSite.png",
		"airbase.png"
	};
}

#endif
