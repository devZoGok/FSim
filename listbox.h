#ifndef LISTBOX_H
#define LISTBOX_H

#include"button.h"
#include<vector.h>
#include<vector>

namespace vb01{
	class Node;
	class Text;
}

namespace fsim{
	class Listbox{
		public: 
			Listbox(GameManager*,vb01::Vector2, vb01::Vector2, std::vector<std::string>&, int);
			~Listbox();
			void update();
			void openUp();
			void close();
			void scrollUp();
			void scrollDown();
			void addLine(std::string);
			void changeLine(int, std::string);
			inline bool isOpen(){return open;}
			void appendLines(std::vector<std::string>&);
			inline int getSelectedOption(){return selectedOption;}
			inline int getMaxDisplay(){return maxDisplay;}
			inline int getNumLines(){return lines.size();}
			std::vector<std::string> getContents();
			inline vb01::Vector2 getPos(){return pos;} 
			inline vb01::Vector2 getSize(){return size;} 
			inline double* getMousePosX(){return mousePosX;}
			inline double* getMousePosY(){return mousePosY;}
		private: 
			class ListboxButton : public Button{
				public:
					ListboxButton(Listbox*, GameManager*, vb01::Vector2, vb01::Vector2, std::string, bool);
					void onClick();
				private:
					Listbox *listbox = nullptr;
			};
			class ScrollingButton : public Button{
				public:
					ScrollingButton(GameManager*,vb01::Vector2,vb01::Vector2,std::string,bool);
					void onClick();
				private:
			};
			GameManager *gm;
			double *mousePosX,*mousePosY;
			int maxDisplay, scrollOffset = 0, selectedOption = 0;
			bool open = false;
			std::vector<vb01::Text*> lines;
			vb01::Vector2 pos, size;
			ListboxButton *listboxButton;
			ScrollingButton *scrollingButton;
			vb01::Node *selRectNode,*guiNode;
		public: 
			inline ListboxButton* getListboxButton(){return listboxButton;}
			inline ScrollingButton* getScrollingButton(){return scrollingButton;}
	};
}

#endif
