#include"jet.h"
#include"missile.h"
#include"bomb.h"
#include"projectileData.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"structureData.h"
#include<vector>

using namespace vb01;
using namespace std;

namespace fsim{
	Jet::Jet(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot,int *upgrades) : Aircraft(gm,id,faction,pos,rot,upgrades){}

	Jet::~Jet(){}

	void Jet::update(){
		Aircraft::update();
		pos=pos+dir*.1;
	}

}
