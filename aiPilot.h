#ifndef AI_PILOT_H
#define AI_PILOT_H

#include<vector.h>

namespace fsim{
	class GameManager;
	class Structure;
	class Aircraft;

	class AIPilot{
		public:
			AIPilot(GameManager*,Aircraft*);
			~AIPilot();
			void update();
			void turnVertically(float);
			void turnHorizontally(float);
			void advance();
		private:
			enum BombrunStage{NONE,DESCENDING,ASCENDING};
			GameManager *gm;
			BombrunStage bs=NONE;
			Structure *target=nullptr;
			Aircraft *leader=nullptr,*aircraft;
			vb01::Vector3 waypoint=vb01::Vector3::VEC_ZERO,initDir=vb01::Vector3::VEC_ZERO,initUp=vb01::Vector3::VEC_ZERO,initRight=vb01::Vector3::VEC_ZERO;
			bool canAltFire=true;
			float maxWaypointDist=9,lineOfSight=70,pitchAngle=0,rollAngle=0,pitchStr=0,rollStr=0,altitude;
	};
}

#endif
