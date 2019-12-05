#ifndef PROJECTILE_DATA_H
#define PROJECTILE_DATA_H

#include<string>

namespace fsim{
	namespace projectileData{
		enum Type{AAM,ASM,SAM,BOMB};

		static const int numProjectiles=4;	

		static const std::string meshPath[numProjectiles]{
			"aam.obj",	
			"aam.obj",	
			"sam.obj",	
			"bomb.obj",	
		};

		static const float speed[numProjectiles]{.2,.1,.1,.1};
		static const float turnSpeed[numProjectiles]{.5,.1,.1,0};
	}
}

#endif
