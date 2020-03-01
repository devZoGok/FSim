#ifndef FX_H
#define FX_H

#include"util.h"
#include<vector.h>

namespace vb01{
	class ParticleEmitter;
	class Light;
}

namespace fsim{
	struct Fx{
		vb01::ParticleEmitter **emitters=nullptr;
		vb01::Light **lights=nullptr;
		vb01::Vector3 pos;
		int numEmitters;
		s64 initTime,timeToLive;
	};
}

#endif
