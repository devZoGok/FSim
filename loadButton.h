#ifndef LOAD_BUTTON_H
#define LOAD_BUTTON_H

#include"button.h"

namespace fsim{
	class LoadButton : public Button{
		public:
			LoadButton(GameManager*,vb01::Vector2,vb01::Vector2);
			void onClick();
		private:
	};
}

#endif
