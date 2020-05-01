#ifndef ABSRTACT_RUNWAY_H
#define ABSTRACT_RUNWAY_H

#include<vector.h>
#include<quaternion.h>
#include"gameManager.h"

namespace fsim{
	class AbstractRunway{
		protected:
			AbstractRunway(GameManager*,int,int);
			void update();

			vb01::Vector3 pos;
			vb01::Quaternion rot;
		private:
			GameManager *gm;
			int faction,id;
	};
}

#endif
