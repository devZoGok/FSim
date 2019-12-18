#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include"unit.h"
#include"util.h"

namespace vb01{
	class Camera;
}

namespace fsim{
	class Aircraft : public Unit{
		public:
			Aircraft(GameManager*,int,int,vb01::Vector3,vb01::Quaternion,int*);
			virtual ~Aircraft();
			virtual void update();
			virtual void yaw(float);
			virtual void roll(float);
			virtual void pitch(float);
			inline float getRollSpeed(){return rollSpeed;}
			inline float getYawSpeed(){return yawSpeed;}
			inline float getPitchSpeed(){return pitchSpeed;}
			inline float getPitch(){return pitchVal;}
			inline float getRoll(){return rollVal;}
			inline float getYaw(){return yawVal;}
			inline bool canPrimaryFire(){return primaryAmmo>0&&getTime()-lastPrimaryFire>rateOfPrimaryFire;}
			inline bool canSecondaryFire(){return secondaryAmmo>0&&getTime()-lastSecondaryFire>rateOfSecondaryFire;}
			void setPitch(float);
			void setRoll(float);
			void setYaw(float);
		private:
			vb01::Camera *cam=nullptr;
		protected:
			void primaryFire();
			void secondaryFire();

			s64 rateOfPrimaryFire,rateOfSecondaryFire,lastPrimaryFire=0,lastSecondaryFire=0;
			int *upgrades,primaryAmmo,secondaryAmmo;
			float rollSpeed,yawSpeed,pitchSpeed,pitchVal=0,rollVal=0,yawVal=0;
	};
}

#endif
