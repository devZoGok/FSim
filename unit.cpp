#include"unit.h"
#include<model.h>

using namespace vb01;

namespace fsim{
	Unit::Unit(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot) : Structure(gm,id,faction,pos,rot){
	}

	Unit::~Unit(){}

	void Unit::update(){
		rot=rot.norm();
		model->setPosition(pos);
		model->setOrientation(rot);
	}
}
