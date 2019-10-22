#ifndef JET_H
#define JET_H

#include"aircraft.h"

namespace fsim{
	class Jet : public Aircraft{
		public:
			Jet(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~Jet();
			void update();
		private:
	};
}

#endif
