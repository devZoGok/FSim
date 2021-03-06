#ifndef FX_H
#define FX_H

#include"util.h"
#include<vector.h>

namespace vb01{
	class ParticleEmitter;
	class Light;
}

namespace sf{
	class Sound;
}

namespace fsim{
	struct Fx{
		vb01::ParticleEmitter **emitters=nullptr;
		vb01::Light **lights=nullptr;
		vb01::Vector3 pos;
		bool allive=true;
		int numEmitters;
		sf::Sound *sfx=nullptr;
		s64 initTime,timeToLive;
	};
}

#endif
