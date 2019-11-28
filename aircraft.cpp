#include"aircraft.h"
#include"gameManager.h"
#include<camera.h>
#include<model.h>
#include<root.h>

using namespace vb01;

namespace fsim{
	Aircraft::Aircraft(GameManager *gm,int id, Vector3 pos, Quaternion rot) : Unit(gm,id,pos,rot){
		cam=gm->getRoot()->getCamera();
		this->rollSpeed=.05;
		this->yawSpeed=.05;
		this->pitchSpeed=.05;
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
		Quaternion rotQuat=Quaternion(angle,dir);
		left=rotQuat*left,up=rotQuat*up;	
		rot=rotQuat*rot;
	}

	void Aircraft::yaw(float angle){
		Quaternion rotQuat=Quaternion(angle,up);
		dir=rotQuat*dir,left=rotQuat*left;	
		rot=rotQuat*rot;
	}

	void Aircraft::pitch(float angle){
		Quaternion rotQuat=Quaternion(angle,left);
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
