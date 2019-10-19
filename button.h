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
			Button(GameManager*,vb01::Vector2,vb01::Vector2,std::string,bool=true,std::string="");
			~Button();
			void update();
			virtual void onMouseOver(){}
			virtual void onClick(){}
			inline vb01::Vector2 getPos(){return pos;}
			void setPos(vb01::Vector2);
			inline vb01::Vector2 getSize(){return size;}
			void setSize(vb01::Vector2);
			inline std::string getName(){return name;}
			inline bool isSeparate(){return separate;}
			inline vb01::Vector4 getColor(){return color;}
			void setColor(vb01::Vector4);
			void setZOrder(float);
		private:
			bool separate;
			vb01::Quad *rect;
			vb01::Node *rectNode,*textNode=nullptr,*guiNode;
			vb01::Vector2 pos,size;
			vb01::Text *text=nullptr;
			std::string name;
			vb01::Vector4 color=vb01::Vector4(.6,.6,.6,1);
		protected:
			GameManager *gm;
	};
}

#endif
