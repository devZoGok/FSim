#ifndef DEFCONFIGS_H
#define DEFCONFIGS_H

#include<string>
#include<glfw3.h>
#include"mapping.h"

namespace fsim{
	const static std::string PATH="../Assets/",defaultTexture=PATH+"Textures/defaultTexture.jpg";

	const static int numStates=4,numBinds[numStates]{0,1,9,11},maxNumBinds=11,maxNumLevels=1;

	const static int buttonTriggers[12]{
		GLFW_KEY_P,
		GLFW_KEY_1,
		GLFW_KEY_2,
		GLFW_KEY_3,
		GLFW_KEY_L,
		GLFW_KEY_O,
		GLFW_KEY_C,
		GLFW_KEY_V,
		GLFW_KEY_R,
		GLFW_KEY_1,
		GLFW_KEY_2,
		GLFW_KEY_3
	};

	const static Mapping::BindType defaultBindTypes[numStates][maxNumBinds]{
		{},
		{
			Mapping::KEYBOARD
		},
		{
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::MOUSE_KEY,
			Mapping::MOUSE_KEY,
			Mapping::KEYBOARD
		},
		{
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::MOUSE_KEY,
			Mapping::MOUSE_KEY,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD,
			Mapping::KEYBOARD
		}
	};

	const static int defaultTriggers[numStates][maxNumBinds]{
		{},
		{
			GLFW_KEY_ESCAPE
		},
		{
			GLFW_KEY_S,
			GLFW_KEY_W,
			GLFW_KEY_A,
			GLFW_KEY_D,
			GLFW_KEY_Q,
			GLFW_KEY_E,
			GLFW_MOUSE_BUTTON_LEFT,
			GLFW_MOUSE_BUTTON_RIGHT,
			GLFW_KEY_SPACE
		},
		{
			GLFW_KEY_S,
			GLFW_KEY_W,
			GLFW_KEY_A,
			GLFW_KEY_D,
			GLFW_KEY_Q,
			GLFW_KEY_E,
			GLFW_MOUSE_BUTTON_LEFT,
			GLFW_MOUSE_BUTTON_RIGHT,
			GLFW_KEY_SPACE,
			GLFW_KEY_LEFT_SHIFT,
			GLFW_KEY_LEFT_CONTROL
		}
	};

	const static bool action[numStates][maxNumBinds]{
		{},
		{1},
		{1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1},
	};

}

#endif
