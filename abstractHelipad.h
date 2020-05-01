#ifndef ABSTRACT_HELIPAD_H
#define ABSTRACT_HELIPAD_H

#include<vector.h>
#include<quaternion.h>
#include"gameManager.h"

namespace fsim{
	class AbstractHelipad{
		protected:
			AbstractHelipad(GameManager*,int,int);
			void update();
			vb01::Vector3 pos;
			vb01::Quaternion rot;
		private:
			GameManager *gm;
			int id,faction;
	};
}

#endif
