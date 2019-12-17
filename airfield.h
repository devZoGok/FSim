#ifndef AIRFIELD_H
#define AIRFIELD_H

#include"building.h"
#include"util.h"

namespace fsim{
	class Airfield : public Building{
		public:
			Airfield(GameManager*,int,int,vb01::Vector3,vb01::Quaternion);
			void update();
			inline vb01::Vector3 getRunwayPos(int i){return runwayPos[i];}
			inline vb01::Vector3 getHelipadPos(int i){return helipadPos[i];}
			inline vb01::Vector3 getRunwayDir(int i){return runwayDirs[i];}
			inline int getNumRunways(){return numRunways;}
			inline int getNumHelipads(){return numHelipads;}
		private:
			s64 totalLandingTime=0,landTime=200;
			int numRunways,numHelipads;
			vb01::Vector3 *runwayPos,*runwayDirs,*helipadPos;
	};
}

#endif
