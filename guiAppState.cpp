#include"guiAppState.h"
#include"gameManager.h"
#include"button.h"
#include<root.h>
#include<glfw3.h>

using namespace vb01;

namespace fsim{
	GuiAppState::GuiAppState(GameManager *gm){
		this->gm=gm;
		type=Type::GUI_STATE;
	}

	GuiAppState::~GuiAppState(){}

	void GuiAppState::update(){
		for(Button *b : buttons)
			b->update();
	}

	void GuiAppState::onDettached(){
		AbstractAppState::onDettached();
	}
	
	void GuiAppState::onAttached(){
		AbstractAppState::onAttached();
		Mapping *m=new Mapping;
		m->bind=Mapping::MOUSE_CLICK;
		m->trigger=GLFW_MOUSE_BUTTON_1;
		m->action=1;
		m->type=Mapping::MOUSE;
		mappings.push_back(m);
	}

	void GuiAppState::addButton(Button *b){
		buttons.push_back(b);
	}

	void GuiAppState::removeButton(Button *b){
		int id=-1;
		for(int i=0;i<buttons.size()&&id==-1;i++)
			if(buttons[i]==b)
				id=i;
		if(id!=-1){
			delete buttons[id];
			buttons.erase(buttons.begin()+id);
		}
	}

	void GuiAppState::onAction(Mapping::Bind bind, bool isPressed){
		double *x=new double,*y=new double;
		glfwGetCursorPos(gm->getRoot()->getWindow(),x,y);
		switch(bind){
			case Mapping::MOUSE_CLICK:
				if(isPressed)
					for(Button *b : buttons){
						Vector2 pos=b->getPos(),size=b->getSize();
						if((pos.x<=*x&&*x<=pos.x+size.x)&&(pos.y<=*y&&*y<=pos.y+size.y))
							b->onClick();
					}
				break;
		}
		delete x;
		delete y;
	}
}
