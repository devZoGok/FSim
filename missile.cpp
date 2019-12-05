#include"missile.h"
#include"structure.h"

using namespace vb01;

namespace fsim{
	Missile::Missile(GameManager *gm,int id,Structure *structure,Vector3 pos,Quaternion rot,Structure *target) : Projectile(gm,id,structure,pos,rot){
		this->target=target;
	}

	Missile::~Missile(){
	}

	void Missile::update(){
		Projectile::update();
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
	}
}
