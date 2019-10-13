#ifndef GUI_APP_STATE_H
#define GUI_APP_STATE_H

#include"abstractAppState.h"
#include<vector>

namespace fsim{
	class Button;
	class GameManager;

	class GuiAppState : public AbstractAppState{
		public:
			GuiAppState(GameManager*);
			~GuiAppState();
			void update();
			void onDettached();
			void onAttached();
			void addButton(Button*);
			void removeButton(Button*);
			void onAction(Mapping::Bind,bool);
		private:
			std::vector<Button*> buttons;
			GameManager *gm;
	};
}

#endif
