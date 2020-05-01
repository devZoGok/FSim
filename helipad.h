#ifndef HELIPAD_H
#define HELIPAD_H

#include"building.h"
#include"abstractHelipad.h"
#include<vector.h>

namespace fsim{
	class Helipad : public Building, public AbstractHelipad{
		public:
			Helipad(GameManager*,int,vb01::Vector3);
			~Helipad();
			void update();
		private:
	};
}

#endif
