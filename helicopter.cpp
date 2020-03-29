#include"helicopter.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"

using namespace vb01;

namespace fsim{
	Helicopter::Helicopter(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot, bool ai, int *upgrades) : Aircraft(gm,id,faction,pos,rot,upgrades,ai){}

	Helicopter::~Helicopter(){}

	void Helicopter::update(){
		Aircraft::update();
		if(rising>risingSpeed)rising=risingSpeed;
		else if(rising<0)rising=0;
		Vector3 deltaPos=Vector3(0,-weight,0)+up*rising;
		pos=pos+deltaPos;
	}
}
