#ifndef STRUCTURE_DATA_H
#define STRUCTURE_DATA_H

#include<string>

namespace fsim{
	const int numStructures=5;

	enum Type{
		FIGHTER,
		FIGHTER_BOMBER,
		HELICOPTER,
		SAM_SITE,
		AIRFIELD
	};
	static const bool singleModel[numStructures]={
		1,
		1,
		1,
		1,
		0
	};
	static const std::string path[numStructures]={
		"Models/Aircraft/Fighters/jet00",
		"Models/Aircraft/Fighters/jet00",
		"Models/Aircraft/Helicopters/z-10",
		"Models/Buildings/SAMSite/samSite",
		""
	};
	static const std::string iconPath[numStructures]={
		"jet.png",
		"jet.png",
		"heli.png",
		"samSite.png",
		"airbase.png"
	};
}

#endif
