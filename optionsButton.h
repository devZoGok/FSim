#ifndef OPTIONS_BUTTON_H
#define OPTIONS_BUTTON_H

#include"button.h"

namespace fsim{
	class GameManager;

	class OptionsButton : public Button{
		public:
			OptionsButton(GameManager*,vb01::Vector2,vb01::Vector2);
			void onClick();
		private:
	};
}

#endif
