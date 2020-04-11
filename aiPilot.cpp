#include"aiPilot.h"
#include"helicopter.h"
#include"structureData.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"structure.h"
#include<util.h>
#include<vector>
#include<cmath>
#include<iostream>

using namespace vb01;
using namespace std;

namespace fsim{
	AIPilot::AIPilot(GameManager *gm,Aircraft *aircraft){
		this->gm=gm;
		this->aircraft=aircraft;
		//waypoint=Vector3(30,20,60);
	}

	AIPilot::~AIPilot(){}

	void AIPilot::update(){
		AircraftType type=aircraft->getType();
		Vector3 pos=aircraft->getPos(),dir=aircraft->getDir();
		if(waypoint!=Vector3::VEC_ZERO){
			Vector3 waypointDir=Vector3(waypoint.x-pos.x,0,waypoint.z-pos.z);
			if(waypointDir.getLength()>maxWaypointDist&&dir.getAngleBetween(waypointDir.norm())>.6){
				if(type!=AircraftType::FIGHTER_BOMBER||(type==AircraftType::FIGHTER_BOMBER&&bs==NONE&&!target))
					turnHorizontally(aircraft->getYawSpeed());
			}
			if(type==AircraftType::HELICOPTER)
				advance();
		}
		else{
			if(type==AircraftType::FIGHTER||(type==AircraftType::FIGHTER_BOMBER&&bs==NONE&&!target))
				turnHorizontally(aircraft->getYawSpeed());
		}

		vector<Structure*> hostiles;
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		for(Structure *s : inGameState->getStructures()){
			Vector3 sPos=s->getPos();
			float dist=Vector3(sPos.x-pos.x,0,sPos.z-pos.z).getLength();
			if(dist<lineOfSight&&s->getFaction()!=aircraft->getFaction()){
				float angle=Vector3(sPos.x-pos.x,0,sPos.z-pos.z).norm().getAngleBetween(Vector3(dir.x,0,dir.z).norm());
				switch(type){
					case AircraftType::FIGHTER:
						if(s->getId()<=structureData::KOREAN_HELICOPTER&&angle<.3)
							hostiles.push_back(s);
						break;
					case AircraftType::FIGHTER_BOMBER:
						if(s->getId()>structureData::KOREAN_HELICOPTER&&angle<.05&&dist<20)
							hostiles.push_back(s);
						break;
					case AircraftType::HELICOPTER:
						if(s->getId()>structureData::KOREAN_HELICOPTER&&angle<1)
							hostiles.push_back(s);
						break;
				}
			}
		}
		target=!hostiles.empty()?hostiles[0]:nullptr;
		float diveAngle=.9,angle=Vector3(dir.x,0,dir.z).norm().getAngleBetween(dir);
		switch(type){
			case AircraftType::FIGHTER:
				/*
				if(target)
					aircraft->secondaryFire();
					*/
				break;
			case AircraftType::FIGHTER_BOMBER:
				if(bs==ASCENDING){
					if(pos.y<altitude){
						if(dir.y<0||(dir.y>0&&angle<diveAngle))
							turnVertically(-aircraft->getPitchSpeed());
					}
					else if(pos.y>=altitude){
						if(angle>.01)
							turnVertically(aircraft->getPitchSpeed());
						else{
							canAltFire=true;
							bs=NONE;
						}
					}
				}
				if(target){
					if(bs==NONE){
						Vector3 tPos=target->getPos();
						if(Vector3(tPos.x-pos.x,0,tPos.z-pos.z).getLength()<6){
							bs=DESCENDING;
							altitude=pos.y;
						}
					}
					else if(bs==DESCENDING){
						if(angle<diveAngle)
							turnVertically(aircraft->getPitchSpeed());
						else{
							if(canAltFire){
								aircraft->secondaryFire();
								canAltFire=false;
							}
							bs=ASCENDING;
						}
					}
				}
				else{
					if(bs==DESCENDING)
						bs=ASCENDING;
				}
				break;
			case AircraftType::HELICOPTER:
				if(target)
					aircraft->secondaryFire();
				break;
		}
		if(target){
			Vector3 tPos=target->getPos();
			/*
			if((tPos-pos).norm().getAngleBetween(dir)<.2)
				aircraft->primaryFire();
				*/
		}
	}

	void AIPilot::turnVertically(float angle){
		aircraft->pitch(angle);
	}

	void AIPilot::turnHorizontally(float angle){
		if(aircraft->getType()!=AircraftType::HELICOPTER){
			aircraft->yaw(angle);
		}
	}

	void AIPilot::advance(){
		Helicopter *heli=(Helicopter*)aircraft;
		float weight=aircraft->getWeight();
		Vector3 pos=heli->getPos(),dir=heli->getDir(),up=heli->getUp(),left=heli->getLeft();
		Vector3 newUp=Vector3(waypoint.x-pos.x,weight,waypoint.z-pos.z);
		float speed=.3,angle=asin(weight/speed);
		float dist=Vector3(waypoint.x-pos.x,0,waypoint.z-pos.z).getLength(),minDist=5;
		float pitchSpd=heli->getPitchSpeed(),rollSpd=heli->getRollSpeed(),yawSpd=heli->getYawSpeed();
		float upAngle=up.getAngleBetween(newUp.norm());
		float vertAngle=(-Vector3(left.x,0,left.z).norm()).getAngleBetween(Vector3(newUp.x,0,newUp.z).norm());
		if(Vector3(dir.x,0,dir.z).norm().getAngleBetween(Vector3(newUp.x,0,newUp.z).norm())>PI/2)
			vertAngle=2*PI-vertAngle;
		if(pitchStr==0)pitchStr=sin(vertAngle),rollStr=cos(vertAngle);
		if(dist>minDist){
			if(initUp==Vector3::VEC_ZERO)initUp=up;
			else if(initUp!=Vector3::VEC_ZERO&&initUp.getAngleBetween(up)+pitchSpd<initUp.getAngleBetween(newUp.norm())){
				heli->setRising(speed);
				heli->setPitch(pitchSpd*pitchStr);
				heli->setRoll(rollSpd*rollStr);
				pitchAngle+=pitchSpd*pitchStr;
				rollAngle+=rollSpd*rollStr;
			}
			/*
			else{
				if(Vector3(dir.x,0,dir.z).norm().getAngleBetween(Vector3(waypoint.x-pos.x,0,waypoint.z-pos.z).norm())>yawSpd)
					heli->setYaw(yawSpd);
			}
			*/
		}
		else if(dist<minDist&&up.getAngleBetween(newUp.norm())<initUp.getAngleBetween(newUp.norm())){
			heli->setRising(.1);
			heli->setPitch(-pitchSpd*pitchStr);
			heli->setRoll(-rollSpd*rollStr);
		}
	}
}
