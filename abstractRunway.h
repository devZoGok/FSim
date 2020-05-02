#ifndef ABSRTACT_RUNWAY_H
#define ABSTRACT_RUNWAY_H

#include<vector.h>
#include<quaternion.h>
#include"gameManager.h"

namespace fsim{
	class AbstractHelipad;

	class AbstractRunway{
		public:
			AbstractRunway(GameManager*,int,int);
			void update();
			inline int getId(){return id;}
			inline vb01::Vector3 getStartPos(){return startPos;}
			inline vb01::Quaternion getStartRot(){return startRot;}
			inline void setStartPos(vb01::Vector3 sp){this->startPos=sp;}
			inline void setStartRot(vb01::Quaternion sr){this->startRot=sr;}
			inline void setAbstractHelipad(AbstractHelipad *ah){this->helipad=ah;}
		private:
			GameManager *gm;
			int faction,id;
			vb01::Vector3 startPos;
			vb01::Quaternion startRot;
			AbstractHelipad *helipad=nullptr;
	};
}

#endif
