#ifndef DEFCONFIGS_H
#define DEFCONFIGS_H

#include<string>
#include<glfw3.h>

namespace fsim{
	const static std::string PATH="../Assets/",defaultTexture=PATH+"Textures/defaultTexture.jpg";

	const static int numStates=4,numBinds[numStates]{0,1,8,10};

	const static int buttonTriggers[8]{
		GLFW_KEY_P,
		GLFW_KEY_L,
		GLFW_KEY_O,
		GLFW_KEY_C,
		GLFW_KEY_V,
		GLFW_KEY_1,
		GLFW_KEY_2,
		GLFW_KEY_3,
	};
}

#endif
