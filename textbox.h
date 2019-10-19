#ifndef TEXTBOX_H
#define TEXTBOX_H

#include"button.h"
#include"util.h"
#include<text.h>

namespace vb01{
	class Node;
	class Quad;
}

namespace fsim{
	class Textbox{
		public: 
			Textbox(GameManager*,vb01::Vector2,vb01::Vector2,std::string="");
			~Textbox();
			void update();
			void enable();
			void disable();
			void type(char,bool=false);
			void moveCursor(bool,float);
			void deleteCharacter();
			void setEntry(std::string);
			inline bool isEnabled(){return enabled;}
			inline std::string getText(){return text->getText();}
			void remove();
		private:
			class TextboxButton : public Button{
				public:
					TextboxButton(Textbox*,GameManager*,vb01::Vector2,vb01::Vector2,std::string);
					void onClick();
				private:
					Textbox *textbox;
			};
			inline bool canChangeCursor(){return getTime()-lastBlinkTime>250;}
			const int cursorWidth=5;
			vb01::Vector2 pos, size;
			std::string entry="";
			bool enabled=false,canShowCursor=false,capitalLeters=false;
			TextboxButton *textboxButton;
			s64 lastBlinkTime, cursorPosOffset=0;
			vb01::Quad *cursorRect;
			vb01::Text *text;
			vb01::Node *guiNode,*textNode,*cursorNode;
		public:
			inline TextboxButton* getTextboxButton(){return textboxButton;}
	};
}

#endif
