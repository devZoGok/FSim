#ifndef HELICOPTER_H
#define HELICOPTER_H

#include"aircraft.h"

namespace fsim{
	class Helicopter : public Aircraft{
		public:
			Helicopter(GameManager*,int,int,vb01::Vector3,vb01::Quaternion,bool,int* = nullptr);
			~Helicopter();
			void update();
			inline float getRising(){return rising;}
			inline float getRisingSpeed(){return risingSpeed;}
			inline void setRising(float r){this->rising=r;}
		private:
			float rising=.1,risingSpeed=.8;
	};
}

#endif
