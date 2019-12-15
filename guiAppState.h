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
			void removeAllButtons(Button**,int=0);
			void addListbox(Listbox*);
			void removeListbox(Listbox*);
			void removeAllListboxes();
			void addTextbox(Textbox*);
			void removeTextbox(Textbox*);
			void removeAllTextboxes();
			void removeAllGUIElements(bool=true,bool=true,bool=true);
			void onAction(Mapping::Bind,bool);
			void onRawKeyButton(short);
			void onRawMouseButton(short);
			void onRawJoystickAxis(short,float);
			void onRawJoystickButton(short);
		private:
			bool canType(){return getTime()-lastTypeTime>100;}
			void updateControlsListbox(Mapping::BindType,bool,short);
			void activateBoundButton(Mapping::Bind);

			s64 lastTypeTime=0;
			bool shiftPressed=false;
			std::vector<Button*> buttons;
			std::vector<Listbox*> listboxes;
			std::vector<Textbox*> textboxes;
	};
}

#endif
