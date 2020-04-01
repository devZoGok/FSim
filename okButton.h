#ifndef OK_BUTTON_H
#define OK_BUTTON_H

#include"button.h"

namespace fsim{
	class Textbox;

	class OkButton : public Button{
		public:
			OkButton(GameManager*,vb01::Vector2,vb01::Vector2,Textbox*,int=-1,int=-1);
			void onClick();
			inline void setFaction(int f){this->faction=f;}
		private:
			Textbox *textbox;
			int faction,pilotId;
	};
}

#endif
