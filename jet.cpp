#include"jet.h"

using namespace vb01;

namespace fsim{
	Jet::Jet(GameManager *gm,int id, Vector3 pos, Quaternion rot,int *upgrades) : Aircraft(gm,id,pos,rot,upgrades){}

	Jet::~Jet(){}

	void Jet::update(){
		Aircraft::update();
		pos=pos+dir*.1;
	}
}
