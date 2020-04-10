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
			inline vb01::Vector3 getPos(){return pos;}
			inline vb01::Quaternion getRot(){return rot;}
			inline int getId(){return id;}
			inline bool isExploded(){return exploded;}
			inline Structure* getOwner(){return owner;}
		protected:
			virtual void checkForCollision();
			virtual void explode();

			GameManager *gm;
			int id;
			bool exploded=false;
			Structure *owner;
			vb01::Vector3 pos,dir,left,up;
			vb01::Quaternion rot;
			vb01::Model *mesh;
			float speed,turnSpeed,length;
	};
}

#endif
