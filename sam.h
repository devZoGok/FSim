#ifndef SAM_H
#define SAM_H

#include"building.h"
#include<util.h>

namespace sf{
	class SoundBuffer;
	class Sound;
}

namespace vb01{
	class Model;
}

namespace fsim{
	class Aircraft;

	class SAM : public Building{
		public:
			SAM(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~SAM();
			void update();
		private:
			void turnHorizontally(float);
			void turnVertically(float);
			void fire();
			inline bool canFire(){return vb01::getTime()-lastFire>rateOfFire;}

			sf::SoundBuffer *sfxBuffer;
			sf::Sound *sfx;
			vb01::Model *parts[3];
			vb01::s64 rateOfFire=5000,lastFire=0;
			vb01::Vector3 horDir,vertDir;
			float horAngle=0,vertAngle=0;
			int minVertAngle=-60,maxVertAngle=66;
			Aircraft *target=nullptr;
	};
}

#endif
