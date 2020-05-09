#ifndef PROJECTILE_DATA_H
#define PROJECTILE_DATA_H

#include<string>

namespace fsim{
	namespace projectileData{
		enum Type{
			CHINESE_FIGHTER_AAM,
			JAPANESE_FIGHTER_AAM,
			KOREAN_FIGHTER_AAM,
			CHINESE_HELICOPTER_ASM,
			JAPANESE_HELICOPTER_ASM,
			KOREAN_HELICOPTER_ASM,
			SAM,
			BOMB,
			GPS_BOMB
		};

		static const int numProjectiles=9;

		static const float g=.1;

		static const std::string meshPath[numProjectiles]{
			"AAMs/aamJet00.obj",	
			"AAMs/aamJet01.obj",	
			"AAMs/aam.obj",	
			"AAMs/aam.obj",	
			"ASMs/asmHelicopter01.obj",	
			"ASMs/asmHelicopter02.obj",	
			"SAMs/sam.obj",	
			"Bombs/bomb.obj",	
			"Bombs/gpsBomb.obj"
		};

		static const float speed[numProjectiles]{.2,.2,.2,.2,.2,.2,.2,.2,.2};
		static const float length[numProjectiles]{.7,.7,.7,.7,.7,.7,.6,.28,.35};
		static const float turnSpeed[numProjectiles]{.5,.5,.5,.5,.5,.5,.1,0,.0};
	}
}

#endif
