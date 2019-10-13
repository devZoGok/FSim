#ifndef BUTTON_H
#define BUTTON_H

#include<vector.h>
#include<string>

namespace vb01{
	class Quad;
	class Node;
	class Text;
}

namespace fsim{
	class GameManager;

	class Button{
		public:
			Button(GameManager*,vb01::Vector2,vb01::Vector2,std::string,std::string="");
			~Button();
			void update();
			virtual void onMouseOver(){}
			virtual void onClick(){}
			inline vb01::Vector2 getPos(){return pos;}
			inline vb01::Vector2 getSize(){return size;}
		private:
			GameManager *gm;
			vb01::Quad *rect;
			vb01::Node *rectNode,*textNode,*guiNode;
			vb01::Vector2 pos,size;
			vb01::Text *text;
			std::string name;
			vb01::Vector4 color=vb01::Vector4(.6,.6,.6,1);
	};
}

#endif
