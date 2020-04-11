#include"sam.h"
#include"projectileData.h"
#include"structureData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"aircraft.h"
#include"missile.h"
#include<model.h>
#include<material.h>
#include<cmath>
#include<algorithm>
#include<SFML/Audio.hpp>

using namespace std;
using namespace vb01;
using namespace fsim::structureData;

namespace fsim{
	SAM::SAM(GameManager *gm,int faction,Vector3 pos,Quaternion rot) : Building(gm,SAM_SITE,faction,pos,rot){
		horDir=left;
		vertDir=dir;

		string files[3]{"base.obj","stem.obj","top.obj"};
		for(int i=0;i<3;i++){
			Model *model=new Model(PATH+"Models/Buildings/SAMSite/"+files[i]);
			Material* mat=new Material();
			mat->addDiffuseMap(defaultTexture);
			mat->setLightingEnabled(true);
			model->setMaterial(mat);
			model->setPosition(i==0?pos:Vector3::VEC_ZERO);
			model->setOrientation(i==0?rot:Quaternion(0,0,0,1));
			model->setCastShadow(false);
			(i==0?rootNode:parts[i-1])->attachChild(model);
			parts[i]=model;
		}
		this->model=parts[0];

		hitbox=new Model(PATH+hitboxPath[id]+".obj");
		hitbox->setWireframe(true);
		Material *mat=new Material();
		mat->setTexturingEnabled(false);
		mat->setDiffuseColor(Vector4(0,0,1,1));
		hitbox->setMaterial(mat);
		hitbox->setVisible(false);
		model->attachChild(hitbox);

		sfxBuffer=new sf::SoundBuffer();
		sfxBuffer->loadFromFile(PATH+"Sounds/missile.ogg");
		sfx=new sf::Sound();
		sfx->setBuffer(*sfxBuffer);
		sfx->setMinDistance(5);
		sfx->setAttenuation(5);
	}

	SAM::~SAM(){
		delete sfxBuffer;
		delete sfx;
	}

	void SAM::update(){
		Building::update();
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		float maxDist=500;
		Vector3 tPos=target?target->getPos():Vector3::VEC_ZERO;
		if(!target){
			vector<Aircraft*> hostileAircraft;
			for(Structure *s : inGameState->getStructures()){
				Vector3 sPos=s->getPos();
				if(s->getId()<=Type::KOREAN_HELICOPTER&&s->getFaction()!=faction&&Vector3(sPos.x,0,sPos.z).getDistanceFrom(pos)<maxDist)
					hostileAircraft.push_back((Aircraft*)s);
			}
			for(int i=0;i<hostileAircraft.size();i++){
				for(int j=i;j<hostileAircraft.size()-1;j++){
					Vector3 p0=hostileAircraft[j]->getPos();
					Vector3 p1=hostileAircraft[j+1]->getPos();
					float d0=Vector3(p0.x,0,p0.z).getDistanceFrom(pos);
					float d1=Vector3(p1.x,0,p1.z).getDistanceFrom(pos);
					if(d0>d1)
						swap(hostileAircraft[j],hostileAircraft[j+1]);
				}
			}
			if(!hostileAircraft.empty())
				target=hostileAircraft[0];
		}
		else{
			if(Vector3(tPos.x,0,tPos.z).getDistanceFrom(pos)<maxDist){
				Vector3 vertDirClamped=Vector3(vertDir.x,0,vertDir.z).norm();
				Vector3 tDir=(tPos-pos).norm();
				Vector3 tDirClamped=Vector3(tDir.x,0,tDir.z).norm();
				float minTAngle=.1,rotSpeed=.01;
				float tVertAngle=tDir.getAngleBetween(tDirClamped);
				float tHorAngle=tDirClamped.getAngleBetween(dir);
				bool right=horDir.getAngleBetween(tDirClamped)>PI/2;
				bool up=tVertAngle>vertDirClamped.getAngleBetween(vertDir);
				bool canTurnHor=vertDirClamped.getAngleBetween(tDirClamped)-rotSpeed>minTAngle;
				bool canTurnVert=vertDir.getAngleBetween(tDir)>minTAngle;
				if(canTurnHor)
					turnHorizontally(rotSpeed*(right?-1:1));
				if(canTurnVert)
					turnVertically(rotSpeed*(up?-1:1));
				if(!canTurnHor&&!canTurnVert)
					fire();
			}
			else
				target=nullptr;
		}
		parts[1]->setOrientation(Quaternion(horAngle,Vector3(0,-1,0))*Quaternion(0,0,0,1));
		parts[2]->setOrientation(Quaternion(vertAngle,horDir)*Quaternion(0,0,0,1));
		/*
		*/
	}

	void SAM::turnHorizontally(float angle){
		Quaternion rotQuat=Quaternion(angle,Vector3(0,1,0));
		horDir=rotQuat*horDir,vertDir=rotQuat*vertDir;
		horAngle+=angle;
	}

	void SAM::turnVertically(float angle){
		if(minVertAngle/180.*PI<vertAngle+angle&&vertAngle+angle<maxVertAngle/180.*PI){
			Quaternion rotQuat=Quaternion(angle,horDir);
			vertDir=rotQuat*vertDir;
			vertAngle+=angle;
		}
	}

	void SAM::fire(){
		if(canFire()){
			Quaternion rotQuat=Quaternion(vertAngle,horDir)*(Quaternion(horAngle,Vector3(0,1,0)));
			InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
			inGameState->addProjectile(new Missile(gm,(int)projectileData::SAM,this,pos+vertDir.norm()*1,rotQuat,target));
			sfx->play();
			lastFire=getTime();
		}
	}
}
