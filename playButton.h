#ifndef PLAY_BUTTON_H
#define PLAY_BUTTON_H

#include"button.h"

namespace fsim{
	class PlayButton : public Button{
		public:
			PlayButton(GameManager*,vb01::Vector2,vb01::Vector2);
			void onClick();
		private:
	};
}

#endif
