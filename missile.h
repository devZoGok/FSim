#ifndef MISSILE_H
#define MISSILE_H

#include"projectile.h"

namespace vb01{
	class ParticleEmitter;
}

namespace fsim{
	class Missile : public Projectile{
		public:
			Missile(GameManager*,int,Structure*,vb01::Vector3,vb01::Quaternion,Structure*);
			~Missile();
			void update();
			inline Structure* getTarget(){return target;}
			inline void setTarget(Structure *t){this->target=t;}
		private:
			vb01::ParticleEmitter *gas=nullptr,*smoke=nullptr;
			Structure *target=nullptr;
	};
}

#endif
