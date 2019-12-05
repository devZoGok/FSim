#ifndef PROJECTILE_H
#define PROJECTILE_H

#include<vector.h>
#include<quaternion.h>

namespace vb01{
	class Model;
}

namespace fsim{
	class GameManager;
	class Structure;

	class Projectile{
		public:
			Projectile(GameManager*,int,Structure*,vb01::Vector3,vb01::Quaternion);
			virtual ~Projectile();
			virtual void update();
		protected:
			virtual void checkForCollision();

			GameManager *gm;
			int id;
			Structure *structure;
			vb01::Vector3 pos,dir,left,up;
			vb01::Quaternion rot;
			vb01::Model *mesh;
			float speed,turnSpeed;
	};
}

#endif
