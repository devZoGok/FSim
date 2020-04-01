#ifndef HELIPAD_H
#define HELIPAD_H

#include"building.h"
#include<vector.h>

namespace fsim{
	class Helipad : public Building{
		public:
			Helipad(GameManager*,int,vb01::Vector3);
			~Helipad();
			void update();
		private:
	};
}

#endif
