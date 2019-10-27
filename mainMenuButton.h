#ifndef MAIN_MENU_BUTTON_H
#define MAIN_MENU_BUTTON_H

#include"button.h"

namespace fsim{
	class MainMenuButton : public Button{
		public:
			MainMenuButton(GameManager*,vb01::Vector2,vb01::Vector2);
			void onClick();
		private:
	};
}

#endif
