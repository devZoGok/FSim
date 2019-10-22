#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include"unit.h"

namespace vb01{
	class Camera;
}

namespace fsim{
	class Aircraft : public Unit{
		public:
			Aircraft(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			virtual ~Aircraft();
			virtual void update();
		private:
			vb01::Camera *cam=nullptr;
	};
}

#endif
