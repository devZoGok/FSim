#ifndef BUILDING_H
#define BUILDING_H

#include"structure.h"

namespace fsim{
	class Building : public Structure{
		public:
			Building(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			virtual ~Building();
			virtual void update();
		private:
	};
}

#endif
