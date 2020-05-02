#ifndef ABSTRACT_HELIPAD_H
#define ABSTRACT_HELIPAD_H

#include<vector.h>
#include<quaternion.h>
#include"gameManager.h"

namespace fsim{
	class AbstractRunway;

	class AbstractHelipad{
		public:
			AbstractHelipad(GameManager*,int,int);
			void update();
			inline int getId(){return id;}
			inline vb01::Vector3 getStartPos(){return startPos;}
			inline vb01::Quaternion getStartRot(){return startRot;}
			inline void setStartPos(vb01::Vector3 sp){this->startPos=sp;}
			inline void setStartRot(vb01::Quaternion sr){this->startRot=sr;}
			inline void setAbstractRunway(AbstractRunway *ar){this->runway=ar;}
		private:
			GameManager *gm;
			int id,faction;
			vb01::Vector3 startPos;
			vb01::Quaternion startRot;
			AbstractRunway *runway=nullptr;
	};
}

#endif
