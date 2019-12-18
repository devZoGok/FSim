#include"aircraft.h"
#include"aircraftData.h"
#include"upgradeData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"defConfigs.h"
#include"projectileData.h"
#include"missile.h"
#include"bomb.h"
#include<camera.h>
#include<model.h>
#include<quad.h>
#include<root.h>

using namespace vb01;
using namespace std;

namespace fsim{
	Aircraft::Aircraft(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot, int *upgrades) : Unit(gm,id,faction,pos,rot){
		if(!upgrades){
			this->upgrades=new int[numUpgrades];
			for(int i=0;i<numUpgrades;i++)
				this->upgrades[i]=0;
		}
		else
			this->upgrades=upgrades;
		cam=gm->getRoot()->getCamera();
		this->rollSpeed=.05;
		this->yawSpeed=.05;
		this->pitchSpeed=.05;
		this->rateOfPrimaryFire=aircraftData::rateOfPrimaryFire[id];
		this->rateOfSecondaryFire=aircraftData::rateOfSecondaryFire[id];
		this->primaryAmmo=aircraftData::primaryAmmo[id];
		this->secondaryAmmo=aircraftData::secondaryAmmo[id];
		this->fuel=aircraftData::fuelCapacity[id];

		/*
		Quad *quad=new Quad(Vector3(100,100,.5),false,true);
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->addDiffuseMap(PATH+"Icons/Hitmarker/hitmarker.png");
		mat->setDiffuseColorEnabled(true);
		mat->setDiffuseColor(Vector4(0,0,1,1));
		quad->setMaterial(mat);
		model->attachMesh(quad);
		*/
	}

	Aircraft::~Aircraft(){}

	void Aircraft::update(){
		Unit::update();
		if(cam){
			cam->setPosition(pos-dir*10);
			cam->lookAt(dir,up);
		}
		if(primaryFiring)
			primaryFire();
		if(getTime()-lastConsumption>rateOfFuelConsumption){
			fuel--;
			lastConsumption=getTime();
		}
	}

	void Aircraft::primaryFire(){
		if(canPrimaryFire()){
			primaryAmmo--;
			lastPrimaryFire=getTime();
		}
	}

	void Aircraft::secondaryFire(){
		if(canSecondaryFire()){
			InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
			Projectile *projectile;
			AircraftType type=getType();
			if(type!=FIGHTER_BOMBER){
				float maxAngle=.3;
				vector<Aircraft*> hostileTargets;
				for(Structure *s : inGameState->getStructures())
					if(s->getFaction()!=faction&&dir.getAngleBetween((s->getPos()-pos).norm())<=maxAngle){
						if((type==FIGHTER&&s->getId()<=Type::KOREAN_HELICOPTER)||(type==HELICOPTER&&s->getId()>=KOREAN_HELICOPTER))
							hostileTargets.push_back((Aircraft*)s);
					}
				Structure *target=hostileTargets.size()>0?hostileTargets[0]:nullptr;
				projectile=new Missile(gm,type==FIGHTER?projectileData::AAM:projectileData::ASM,this,pos,rot,target);
			}
			else{
				projectile=new Bomb(gm,projectileData::BOMB,this,pos,rot,.1);
			}
			inGameState->addProjectile(projectile);
			secondaryAmmo--;
			lastSecondaryFire=getTime();
		}
	}

	void Aircraft::roll(float angle){
		Quaternion rotQuat=Quaternion(angle,dir.norm());
		left=rotQuat*left,up=rotQuat*up;	
		rot=rotQuat*rot;
	}

	void Aircraft::yaw(float angle){
		Quaternion rotQuat=Quaternion(angle,up.norm());
		dir=rotQuat*dir,left=rotQuat*left;	
		rot=rotQuat*rot;
	}

	void Aircraft::pitch(float angle){
		Quaternion rotQuat=Quaternion(angle,left.norm());
		dir=rotQuat*dir,up=rotQuat*up;	
		rot=rotQuat*rot;
	}

	void Aircraft::setPitch(float p){
		this->pitchVal=p;
		if(abs(pitchVal)>pitchSpeed)
			pitchVal=pitchVal/pitchVal*pitchSpeed;
		pitch(p);
	}

	void Aircraft::setRoll(float r){
		this->rollVal=r;
		if(abs(rollVal)>rollSpeed)
			rollVal=rollVal/rollVal*rollSpeed;
		roll(r);
	}

	void Aircraft::setYaw(float y){
		this->yawVal=y;
		if(abs(yawVal)>yawSpeed)
			yawVal=yawVal/yawVal*yawSpeed;
		yaw(y);
	}

}
