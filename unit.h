#ifndef UNIT_H
#define UNIT_H

#include"structure.h"

namespace fsim{
	class Unit : public Structure{
		public:
			Unit(GameManager*,int,int,vb01::Vector3,vb01::Quaternion);
			virtual ~Unit();
			virtual void update();
		private:
	};
}

#endif
