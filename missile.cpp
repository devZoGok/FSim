#include"missile.h"
#include"structure.h"
#include"defConfigs.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include<particleEmitter.h>
#include<node.h>
#include<material.h>
#include<model.h>
#include<ray.h>

using namespace vb01;
using namespace std;

namespace fsim{
	Missile::Missile(GameManager *gm,int id,Structure *structure,Vector3 pos,Quaternion rot,Structure *target) : Projectile(gm,id,structure,pos,rot){
		this->target=target;

		gas=new ParticleEmitter(50);
		Node *gasNode=new Node(Vector3(0,0,-.5));
		Material *gasMat=new Material(Material::MATERIAL_PARTICLE);
		gasMat->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
		gas->setMaterial(gasMat);
		gasNode->attachParticleEmitter(gas);
		mesh->attachChild(gasNode);

		gas->setStartColor(Vector4(1,1,0,1));
		gas->setEndColor(Vector4(.8,.8,0,1));
		gas->setStartSize(Vector2(.1,.1));
		gas->setEndSize(Vector2(1,1));
		gas->setLowLife(.1);
		gas->setHighLife(.15);
		gas->setSpread(1);
		gas->setDirection(Vector3(0,0,-.4));

		smoke=new ParticleEmitter(100);
		Node *smokeNode=new Node(Vector3(0,0,-.5));
		Material *smokeMat=new Material(Material::MATERIAL_PARTICLE);
		smokeMat->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
		smoke->setMaterial(smokeMat);
		smokeNode->attachParticleEmitter(smoke);
		mesh->attachChild(smokeNode);

		smoke->setStartColor(Vector4(.7,.7,.7,1));
		smoke->setEndColor(Vector4(.7,.7,.7,.4));
		smoke->setStartSize(Vector2(.1,.1));
		smoke->setEndSize(Vector2(3,3));
		smoke->setLowLife(.1);
		smoke->setHighLife(1);
		smoke->setSpread(20);
		smoke->setDirection(Vector3(0,0,-.1));
	}

	Missile::~Missile(){
		Node *gasNode=gas->getNode(),*smokeNode=smoke->getNode();
		mesh->dettachChild(gasNode);
		mesh->dettachChild(smokeNode);
		delete gasNode;
		delete smokeNode;
		/*
		*/
	}

	void Missile::update(){
		Projectile::update();

		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		vector<CollisionResult> res;

		for(Structure *s : inGameState->getStructures()){
			Model *hitbox=s->getHitbox();
			if(hitbox)
				retrieveCollisions(pos,dir,s->getHitbox(),res,length);
		}
		if(!res.empty())
			explode();
		pos=pos+dir*speed;
		if(target){
			Vector3 targetVec=(target->getPos()-pos).norm();
			Vector3 rotAxis=dir.cross(targetVec);
			if(rotAxis!=Vector3::VEC_ZERO){
				float angle=targetVec.getAngleBetween(dir),rotAngle=angle>turnSpeed?turnSpeed:angle;
				if(rotAngle<angle){
					Quaternion rotQuat=Quaternion(rotAngle,rotAxis.norm());
					dir=rotQuat*dir;
					rot=rotQuat*rot;
				}
			}
		}
		//gas->getNode()->setPosition(pos-dir*2);
		gas->setDirection(-dir*.4);
		//smoke->getNode()->setPosition(pos-dir*2);
		smoke->setDirection(-dir*.1);
	}
}
