#include"gpsBomb.h"
#include"projectileData.h"

using namespace std;
using namespace vb01;
using namespace fsim::projectileData;

namespace fsim{
	GPSBomb::GPSBomb(GameManager *gm,Structure *owner,Vector3 targetPos,Vector3 pos, Quaternion rot,float speed) : Bomb(gm,projectileData::GPS_BOMB,owner,pos,rot,speed){
		if(targetPos!=Vector3::VEC_ZERO){
			float y0=pos.y-targetPos.y;
			float D=speed*speed*sin(initAngle)*sin(initAngle)+2*g*y0;
			float maxTime=(speed*sin(initAngle)-sqrt(D))/g;
			if(maxTime<0)
				maxTime=(speed*sin(initAngle)+sqrt(D))/g;
			Vector3 targDir=Vector3(pos.x-targetPos.x,0,pos.z-targetPos.z);
			float targDist=targDir.getLength();
			float targetTime=sqrt(2/g*(y0+tan(initAngle)*targDist));
			maxDist=speed*cos(initAngle)*maxTime;
			if((Vector3(pos.x,0,pos.z)+targDir.norm()*targDist).getDistanceFrom(Vector3(pos.x,0,pos.z)+Vector3(dir.x,0,dir.z).norm()*targDist)<5)
				speed=targDist/cos(initAngle)*targetTime;
		}
	}

	GPSBomb::~GPSBomb(){
	}

	void GPSBomb::update(){
		Bomb::update();
	}
}
