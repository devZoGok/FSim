#ifndef RUNWAY_H
#define RUNWAY_H

#include"building.h"
#include"abstractRunway.h"
#include<vector.h>

namespace fsim{
	class Runway : public Building, public AbstractRunway{
		public:
			Runway(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~Runway();
			void update();
		private:
	};
}

#endif
