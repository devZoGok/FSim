#include"jet.h"

using namespace vb01;

namespace fsim{
	Jet::Jet(GameManager *gm,int id, Vector3 pos, Quaternion rot) : Aircraft(gm,id,pos,rot){}

	Jet::~Jet(){}

	void Jet::update(){}
}
