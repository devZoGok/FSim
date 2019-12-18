#ifndef MAPPING_H
#define MAPPING_H

#include"defConfigs.h"

namespace fsim{
	struct Mapping{
		enum Bind{
			MOUSE_CLICK,
			SHIFT,
			BACKSPACE,
			MAIN_MENU,
			PITCH_UP,
			PITCH_DOWN,
			YAW_LEFT,
			YAW_RIGHT,
			ROLL_LEFT,
			ROLL_RIGHT,
			PRIMARY_FIRE,
			SECONDARY_FIRE,
			PITCH_HELICOPTER_UP,
			PITCH_HELICOPTER_DOWN,
			ROLL_HELICOPTER_LEFT,
			ROLL_HELICOPTER_RIGHT,
			YAW_HELICOPTER_LEFT,
			YAW_HELICOPTER_RIGHT,
			PRIMARY_HELICOPTER_FIRE,
			SECONDARY_HELICOPTER_FIRE,
			RISE,
			LAND,
			NONE,
			PLAY,
			SELECT_CHINA,
			SELECT_JAPAN,
			SELECT_KOREA,
			LOAD,
			OPTIONS,
			CONTROLS,
			VIDEO,
			FIGHTER,
			FIGHTER_BOMBER,
			HELICOPTER
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
