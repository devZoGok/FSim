#ifndef MAPPING_H
#define MAPPING_H

#include"defConfigs.h"

namespace fsim{
	struct Mapping{
		enum Bind{
			MOUSE_CLICK,
			SHIFT,
			BACKSPACE,
			RISE,
			LAND,
			ACCELERATE,
			DECELERATE,
			PITCH_UP,
			PITCH_DOWN,
			YAW_LEFT,
			YAW_RIGHT
		};
		enum BindType{KEYBOARD,MOUSE_KEY,MOUSE_AXIS,JOYSTICK_KEY,JOYSTICK_AXIS};
		enum AuxTriggers{MOUSE_AXIS_LEFT=310,MOUSE_AXIS_RIGHT=311,MOUSE_AXIS_UP=312,MOUSE_AXIS_DOWN=313};

		Bind bind;
		BindType type;
		int trigger;
		bool action,pressed=false;
	};
}

#endif
