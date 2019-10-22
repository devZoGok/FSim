#include"unit.h"

using namespace vb01;

namespace fsim{
	Unit::Unit(GameManager *gm,int id, Vector3 pos, Quaternion rot) : Structure(gm,id,pos,rot){
	}

	Unit::~Unit(){}

	void Unit::update(){}
}
