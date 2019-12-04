#include"helicopter.h"
#include<cmath>

using namespace vb01;

namespace fsim{
	Helicopter::Helicopter(GameManager *gm,int id, Vector3 pos, Quaternion rot, int *upgrades) : Aircraft(gm,id,pos,rot,upgrades){}

	Helicopter::~Helicopter(){}

	void Helicopter::update(){
		Aircraft::update();
		float weight=.1;
		if(rising<weight)rising=weight;
		if(landing<0)landing=0;
		if(rising>risingSpeed)rising=risingSpeed;
		if(landing>landingSpeed)landing=landingSpeed;
		Vector3 deltaPos=Vector3(0,-weight,0)-up*(landing-rising);
		pos=pos+deltaPos;
	}

}
