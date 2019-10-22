#ifndef HELICOPTER_H
#define HELICOPTER_H

#include"aircraft.h"

namespace fsim{
	class Helicopter : public Aircraft{
		public:
			Helicopter(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~Helicopter();
			void update();
		private:
	};
}

#endif
