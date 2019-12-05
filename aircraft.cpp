#include"aircraft.h"
#include"aircraftData.h"
#include"upgradeData.h"
#include"gameManager.h"
#include<camera.h>
#include<model.h>
#include<root.h>

using namespace vb01;

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
	}

	Aircraft::~Aircraft(){}

	void Aircraft::update(){
		Unit::update();
		if(cam){
			cam->setPosition(pos-dir*10);
			cam->lookAt(dir,up);
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
