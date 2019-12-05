#ifndef MISSILE_H
#define MISSILE_H

#include"projectile.h"

namespace fsim{
	class Missile : public Projectile{
		public:
			Missile(GameManager*,int,Structure*,vb01::Vector3,vb01::Quaternion,Structure*);
			~Missile();
			void update();
		private:
			Structure *target=nullptr;
	};
}

#endif
