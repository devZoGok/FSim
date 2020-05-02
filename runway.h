#ifndef RUNWAY_H
#define RUNWAY_H

#include"building.h"
#include<vector.h>

namespace fsim{
	class AbstractRunway;

	class Runway : public Building{
		public:
			Runway(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~Runway();
			void update();
			inline AbstractRunway* getAbstractRunway(){return runway;}
		private:
			AbstractRunway *runway;
	};
}

#endif
