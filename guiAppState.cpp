#include"guiAppState.h"
#include"gameManager.h"
#include"listbox.h"
#include"textbox.h"
#include<root.h>
#include<glfw3.h>
#include<algorithm>

using namespace vb01;
using namespace std;

namespace fsim{
	GuiAppState::GuiAppState(GameManager *gm){
		this->gm=gm;
		type=Type::GUI_STATE;
	}

	GuiAppState::~GuiAppState(){}

	void GuiAppState::update(){
		for(Button *b : buttons)
			b->update();
		for(Listbox *l : listboxes)
			l->update();
		for(Textbox *t : textboxes)
			t->update();
	}

	void GuiAppState::onDettached(){
		AbstractAppState::onDettached();
	}
	
	void GuiAppState::onAttached(){
		AbstractAppState::onAttached();
		const int numMappings=2;
		Mapping::Bind binds[numMappings]{
			Mapping::MOUSE_CLICK,
			Mapping::SHIFT
		};
		int triggers[numMappings]{
			GLFW_MOUSE_BUTTON_1,
			GLFW_KEY_LEFT_SHIFT
		};
		Mapping::Type types[numMappings]{
			Mapping::MOUSE,
			Mapping::KEYBOARD
		};
		for(int i=0;i<numMappings;i++){
			Mapping *m=new Mapping;
			m->bind=binds[i];
			m->trigger=triggers[i];
			m->action=1;
			m->type=types[i];
			mappings.push_back(m);
		}
	}

	void GuiAppState::addButton(Button *b){
		buttons.push_back(b);
	}

	Button* GuiAppState::getButton(string name){
		Button *b=nullptr;
		for(int i=0;i<buttons.size()&&!b;i++)
			if(buttons[i]->getName()==name)
				b=buttons[i];
		return b;
	}

	void GuiAppState::addListbox(Listbox *l){
		listboxes.push_back(l);
		buttons.push_back(l->getListboxButton());
		buttons.push_back(l->getScrollingButton());
	}

	void GuiAppState::removeListbox(Listbox *l){
		int id=-1;
		for(int i=0;i<listboxes.size()&&id==-1;i++)
			if(listboxes[i]==l)
				id=i;
		if(id!=-1){
			delete listboxes[id];
			listboxes.erase(listboxes.begin()+id);
		}
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

	void GuiAppState::removeButton(string name){
		int id=-1;
		for(int i=0;i<buttons.size()&&id==-1;i++)
			if(buttons[i]->getName()==name)
				id=i;
		if(id!=-1){
			delete buttons[id];
			buttons.erase(buttons.begin()+id);
		}
	}

	void GuiAppState::removeAllButtons(Button **exceptions){
		int numExceptions=sizeof(exceptions)/sizeof(Button**);
		for(int i=0;i<buttons.size();i++){
			for(int j=0;j<numExceptions;j++)
				if(buttons[i]==exceptions[j])
					swap(buttons[i],buttons[j]);
		}
		while(buttons.size()>2){
			delete buttons[buttons.size()-1];
			buttons.pop_back();
		}
	}

	void GuiAppState::addTextbox(Textbox *t){
		textboxes.push_back(t);
		addButton(t->getTextboxButton());
	}

	void GuiAppState::removeTextbox(Textbox *t){
		int id=-1;
		for(int i=0;i<textboxes.size()&&id==-1;i++)
			if(textboxes[i]==t)
				id=i;
		if(id!=-1){
			removeButton(textboxes[id]->getTextboxButton());
			delete textboxes[id];
			textboxes.erase(textboxes.begin()+id);
		}
	}

	void GuiAppState::onAction(Mapping::Bind bind, bool isPressed){
		double *x=new double,*y=new double;
		glfwGetCursorPos(gm->getRoot()->getWindow(),x,y);
		switch(bind){
			case Mapping::MOUSE_CLICK:
				{
				bool clicked=false;
				if(isPressed){
					for(Button *b : buttons){
						if(!clicked){
							Vector2 pos=b->getPos(),size=b->getSize();
							if((pos.x<=*x&&*x<=pos.x+size.x)&&(pos.y<=*y&&*y<=pos.y+size.y)){
								clicked=true;
								b->onClick();
							}
						}
					}
				}
				break;
				}
			case Mapping::SHIFT:
				shiftPressed=!shiftPressed;
				break;
		}
		delete x,y;
	}

	void GuiAppState::onRawKeyButton(char ch){
		Textbox *t=nullptr;
		for(int i=0;i<textboxes.size()&&!t;i++)
			if(textboxes[i]->isEnabled())
				t=textboxes[i];
		if(t&&canType()){
			switch(ch){
				case '/':
					if(shiftPressed)ch='?';
					break;
				case '.':
					if(shiftPressed)ch='>';
					break;
				case ',':
					if(shiftPressed)ch='<';
					break;
				case ';':
					if(shiftPressed)ch=':';
					break;
				case 39:
					if(shiftPressed)ch='"';
					break;
				case '-':
					if(shiftPressed)ch='_';
					break;
				case '=':
					if(shiftPressed)ch='+';
					break;
				case 92:
					if(shiftPressed)ch='|';
					break;
				case '`':
					if(shiftPressed)ch='~';
					break;
				case '[':
					if(shiftPressed)ch='{';
					break;
				case ']':
					if(shiftPressed)ch='}';
					break;
				case '1':
					if(shiftPressed)ch='!';
					break;
				case '2':
					if(shiftPressed)ch='@';
					break;
				case '3':
					if(shiftPressed)ch='#';
					break;
				case '4':
					if(shiftPressed)ch='$';
					break;
				case '5':
					if(shiftPressed)ch='%';
					break;
				case '6':
					if(shiftPressed)ch='^';
					break;
				case '7':
					if(shiftPressed)ch='&';
					break;
				case '8':
					if(shiftPressed)ch='*';
					break;
				case '9':
					if(shiftPressed)ch='(';
					break;
				case '0':
					if(shiftPressed)ch=')';
					break;
			}
			if(ch!=3)
				t->type(ch,shiftPressed);
			else 
				t->deleteCharacter();
			lastTypeTime=getTime();
		}
	}
}
