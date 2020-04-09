#include"bomb.h"
#include"map.h"
#include<ray.h>
#include<model.h>
#include"structure.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"projectileData.h"
#include<vector>
#include<iostream>

using namespace vb01;
using namespace std;
using namespace fsim::projectileData;

namespace fsim{
	Bomb::Bomb(GameManager *gm,int id,Structure *structure,Vector3 pos,Quaternion rot,float speed) : Projectile(gm,id,structure,pos,rot){
		this->speed=speed;	
		initDir=Vector3(dir.x,0,dir.z).norm();
		Vector3 baseLeft=Vector3(left.x,0,left.z).norm();
		initAngle=dir.norm().getAngleBetween(initDir)*(dir.y>0?1:-1);
		initRot=Quaternion(initAngle,baseLeft)*rot;
		this->initTime=getTime();
	}

	Bomb::~Bomb(){
	}

	void Bomb::update(){
		Projectile::update();
		Vector3 baseDir=Vector3(dir.x,0,dir.z).norm();
		float time=float(getTime()-initTime)/1000;
		pos=pos+initDir*speed*cos(initAngle)*time+Vector3(0,speed*sin(initAngle)*time-g*time*time*.5,0);
		float angle=atan((speed*sin(initAngle)-g*time)/(speed*cos(initAngle)));
		Quaternion rotQuat=Quaternion(angle,-left);
		rot=rotQuat*initRot;
		dir=rotQuat*initDir;

		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		Map *map=inGameState->getMap();
		vector<CollisionResult> results;
		retrieveCollisions(pos,dir,(Node*)map->getMapModel(),results,length);
		for(Structure *s : inGameState->getStructures()){
			Model *hitbox=s->getHitbox();
			if(hitbox)
				retrieveCollisions(pos,dir,s->getHitbox(),results);
		}
		for(CollisionResult r : results){
			for(Structure *s : inGameState->getStructures()){
				Model *hitbox=s->getHitbox();
				if(hitbox&&hitbox->getChild(0)->getMesh(0)==r.mesh){
					int hp=s->getHp();
					hp-=100;
					s->setHp(hp);
				}
			}
		}
		if(!results.empty())
			explode();
	}
}
