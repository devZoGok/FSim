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
			virtual void yaw(float);
			virtual void roll(float);
			virtual void pitch(float);
			inline float getRollSpeed(){return rollSpeed;}
			inline float getYawSpeed(){return yawSpeed;}
			inline float getPitchSpeed(){return pitchSpeed;}
			inline float getPitch(){return pitchVal;}
			inline float getRoll(){return rollVal;}
			inline float getYaw(){return yawVal;}
			void setPitch(float);
			void setRoll(float);
			void setYaw(float);
		private:
			vb01::Camera *cam=nullptr;
		protected:
			float rollSpeed,yawSpeed,pitchSpeed,pitchVal=0,rollVal=0,yawVal=0;
	};
}

#endif
