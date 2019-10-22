#ifndef STRUCTURE_DATA_H
#define STRUCTURE_DATA_H

#include<string>

namespace fsim{
	const int numStructures=3;

	enum Type{
		FIGHTER,
		FIGHTER_BOMBER,
		HELICOPTER
	};
	static const std::string path[numStructures]={
		"Models/Aircraft/Fighters/jet00",
		"Models/Aircraft/Fighters/jet00",
		"Models/Aircraft/Helicopters/z-10"
	};
}

#endif
