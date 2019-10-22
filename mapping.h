#ifndef MAPPING_H
#define MAPPING_H

namespace fsim{
	struct Mapping{
		enum Bind{
			MOUSE_CLICK,
			SHIFT,
			BACKSPACE,
			RISE,
			LAND,
			ACCELERATE,
			DECELERATE
		};
		enum Type{KEYBOARD,MOUSE,JOYSTICK};

		Bind bind;
		Type type;
		int trigger;
		bool action,pressed=false;
	};
}

#endif
