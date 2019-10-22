#include"helicopter.h"

using namespace vb01;

namespace fsim{
	Helicopter::Helicopter(GameManager *gm,int id, Vector3 pos, Quaternion rot) : Aircraft(gm,id,pos,rot){}

	Helicopter::~Helicopter(){}

	void Helicopter::update(){}
}
