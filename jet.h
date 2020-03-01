#ifndef JET_H
#define JET_H

#include"aircraft.h"

namespace vb01{
	class ParticleEmitter;
}

namespace fsim{
	class Jet : public Aircraft{
		public:
			Jet(GameManager*,int,int,vb01::Vector3,vb01::Quaternion,int* = nullptr);
			~Jet();
			void update();
		private:
			vb01::ParticleEmitter *gas=nullptr,*smoke=nullptr;
	};
}

#endif
