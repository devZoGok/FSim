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

	void Jet::primaryFire(){
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		vector<Aircraft*> hostileAircraft;
		for(Structure *s : inGameState->getStructures())
			if(s->getId()<=Type::KOREAN_HELICOPTER&&s->getFaction()!=faction&&dir.getAngleBetween((s->getPos()-pos).norm())<=3)
				hostileAircraft.push_back((Aircraft*)s);
		Structure *target=hostileAircraft.size()>0?hostileAircraft[0]:nullptr;
		if(canPrimaryFire()){
			inGameState->addProjectile(new Missile(gm,projectileData::AAM,this,pos,rot,target));
			primaryAmmo--;
			lastPrimaryFire=getTime();
		}
	}

	void Jet::secondaryFire(){
		if(canSecondaryFire()){
			InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
			inGameState->addProjectile(new Bomb(gm,projectileData::BOMB,this,pos,rot,.1));
			secondaryAmmo--;
			lastSecondaryFire=getTime();
		}
	}
}
