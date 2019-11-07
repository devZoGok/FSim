#include"aircraft.h"
#include"gameManager.h"
#include<camera.h>
#include<root.h>

using namespace vb01;

namespace fsim{
	Aircraft::Aircraft(GameManager *gm,int id, Vector3 pos, Quaternion rot) : Unit(gm,id,pos,rot){
		cam=gm->getRoot()->getCamera();
	}

	Aircraft::~Aircraft(){}

	void Aircraft::update(){
		cam->setPosition(pos-dir*15);
		cam->lookAt(dir,up);
	}
}
