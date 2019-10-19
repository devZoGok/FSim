#ifndef EXIT_BUTTON_H
#define EXIT_BUTTON_H

#include"button.h"

namespace fsim{
	class GameManager;

	class ExitButton : public Button{
		public:
			ExitButton(GameManager*,vb01::Vector2,vb01::Vector2);
			void onClick();
		private:
			GameManager *gm;
	};
}

#endif
