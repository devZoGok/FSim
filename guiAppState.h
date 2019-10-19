#ifndef GUI_APP_STATE_H
#define GUI_APP_STATE_H

#include"abstractAppState.h"
#include"util.h"
#include<vector>
#include<string>

namespace fsim{
	class Button;
	class Listbox;
	class Textbox;
	class GameManager;

	class GuiAppState : public AbstractAppState{
		public:
			GuiAppState(GameManager*);
			~GuiAppState();
			void update();
			void onDettached();
			void onAttached();
			void addButton(Button*);
			Button* getButton(std::string);
			void removeButton(Button*);
			void removeButton(std::string);
			void removeAllButtons(Button**);
			void addListbox(Listbox*);
			void removeListbox(Listbox*);
			void addTextbox(Textbox*);
			void removeTextbox(Textbox*);
			void onAction(Mapping::Bind,bool);
			void onRawKeyButton(char);
		private:
			bool canType(){return getTime()-lastTypeTime>100;}

			s64 lastTypeTime=0;
			bool shiftPressed=false;
			std::vector<Button*> buttons;
			std::vector<Listbox*> listboxes;
			std::vector<Textbox*> textboxes;
			GameManager *gm;
	};
}

#endif
