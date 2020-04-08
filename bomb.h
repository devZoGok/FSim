#ifndef BOMB_H
#define BOMB_H

#include"projectile.h"
#include"util.h"

namespace fsim{
	class Bomb : public Projectile{
		public:
			Bomb(GameManager*,int,Structure*,vb01::Vector3,vb01::Quaternion,float);
			~Bomb();
			void update();
		private:
			s64 initTime;
			vb01::Vector3 initDir;
			vb01::Quaternion initRot;
			float initAngle=0;
	};
}

#endif
