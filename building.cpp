#include"building.h"

using namespace vb01;

namespace fsim{
	Building::Building(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot) : Structure(gm,id,faction,pos,rot){
	
	}

	Building::~Building(){}

	void Building::update(){}
}
