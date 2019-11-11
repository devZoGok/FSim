#ifndef HELICOPTER_H
#define HELICOPTER_H

#include"aircraft.h"

namespace fsim{
	class Helicopter : public Aircraft{
		public:
			Helicopter(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~Helicopter();
			void update();
			inline float getRising(){return rising;}
			inline float getLanding(){return landing;}
			inline float getRisingSpeed(){return risingSpeed;}
			inline float getLandingSpeed(){return landingSpeed;}
			inline void setRising(float r){this->rising=r;}
			inline void setLanding(float l){this->landing=l;}
		private:
			float rising=0,landing=0,risingSpeed=.8,landingSpeed=.5;
	};
}

#endif
