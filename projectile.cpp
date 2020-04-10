#include"defConfigs.h"
#include"projectile.h"
#include"projectileData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"map.h"
#include"structure.h"
#include"inGameAppState.h"
#include<ray.h>
#include<root.h>
#include<model.h>
#include<material.h>

using namespace vb01;
using namespace std;

namespace fsim{
	Projectile::Projectile(GameManager *gm,int id,Structure *owner,Vector3 pos,Quaternion rot){
		this->gm=gm;
		this->id=id;
		this->owner=owner;
		this->pos=pos;
		this->rot=rot;
		this->speed=projectileData::speed[id];
		this->turnSpeed=projectileData::turnSpeed[id];
		this->length=projectileData::length[id];
		
		Node *rootNode=gm->getRoot()->getRootNode();
		mesh=new Model(PATH+"Models/Projectiles/"+projectileData::meshPath[id]);
		Material *mat=new Material();
		mat->addDiffuseMap(defaultTexture);
		mesh->setMaterial(mat);
		rootNode->attachChild(mesh);

		Vector3 initDir=Vector3(0,0,1);
		Vector3 initLeft=Vector3(1,0,0);
		Vector3 initUp=Vector3(0,1,0);
		this->dir=rot*initDir;
		this->left=rot*initLeft;
		this->up=rot*initUp;
	}
	
	Projectile::~Projectile(){
		mesh->getParent()->dettachChild(mesh);
		delete mesh;
	}

	void Projectile::update(){
		mesh->setPosition(pos);
		mesh->setOrientation(rot);
		checkForCollision();
	}

	void Projectile::checkForCollision(){
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		Map *map=inGameState->getMap();
		vector<CollisionResult> results;
		retrieveCollisions(pos,dir,(Node*)map->getMapModel(),results,length);
		for(Structure *s : inGameState->getStructures()){
			Model *hitbox=s->getHitbox();
			if(hitbox)
				retrieveCollisions(pos,dir,s->getHitbox(),results,length);
		}
		if(!results.empty())
			sortResults(results);
		for(CollisionResult r : results){
			for(Structure *s : inGameState->getStructures()){
				Model *hitbox=s->getHitbox();
				if(s!=owner&&hitbox&&hitbox->getChild(0)->getMesh(0)==r.mesh){
					int hp=s->getHp();
					hp-=100;
					s->setHp(hp);
					explode();
				}
			}
		}
	}

	void Projectile::explode(){
		exploded=true;
	}
}
