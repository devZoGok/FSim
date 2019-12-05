#include"bomb.h"

using namespace vb01;

namespace fsim{
	Bomb::Bomb(GameManager *gm,int id,Structure *structure,Vector3 pos,Quaternion rot,float speed) : Projectile(gm,id,structure,pos,rot){
		this->speed=speed;	
		this->baseDir=Vector3(dir.x,0,dir.z).norm();
		initAngle=dir.norm().getAngleBetween(baseDir)*(dir.y>0?1:-1);
		this->initTime=getTime();
	}

	Bomb::~Bomb(){
	}

	void Bomb::update(){
		Projectile::update();
		baseDir=Vector3(dir.x,0,dir.z).norm();
		float time=float(getTime()-initTime)/1000,g=.9;
		pos=pos+baseDir*speed*cos(initAngle)*time+Vector3(0,speed*sin(initAngle)*time-g*time*time*.5,0);
		float angle=atan((speed*sin(initAngle)-g*time)/(speed*cos(initAngle)));
		Quaternion rotQuat=Quaternion(angle,left);
		dir=rotQuat*dir;
		rot=rotQuat;
		/*
		*/
	}
}
