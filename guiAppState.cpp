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
	double *x,*y;

	GuiAppState::GuiAppState(GameManager *gm) : AbstractAppState(gm){
		this->gm=gm;
		type=Type::GUI_STATE;
		x=new double,y=new double;
	}

	GuiAppState::~GuiAppState(){}

	void GuiAppState::update(){
		for(Button *b : buttons){
			Vector2 pos=b->getPos(),size=b->getSize();
			b->update();
			glfwGetCursorPos(gm->getRoot()->getWindow(),x,y);
			if((pos.x<=*x&&*x<=pos.x+size.x)&&(pos.y<=*y&&*y<=pos.y+size.y))
				b->onMouseOver();
			else
				b->onMouseOff();
		}
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
		Mapping::BindType types[numMappings]{
			Mapping::MOUSE_KEY,
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
		mappings.push_back(b->getMapping());
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
			removeButton(listboxes[id]->getScrollingButton());
			removeButton(listboxes[id]->getListboxButton());
			delete listboxes[id];
			listboxes.erase(listboxes.begin()+id);
		}
	}

	void GuiAppState::removeAllListboxes(){
		while(!listboxes.empty()){
			int id=listboxes.size()-1;		
			removeButton(listboxes[id]->getScrollingButton());
			removeButton(listboxes[id]->getListboxButton());
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
			removeMapping(buttons[id]->getMapping()->bind);
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
			removeMapping(buttons[id]->getMapping()->bind);
			delete buttons[id];
			buttons.erase(buttons.begin()+id);
		}
	}

	void GuiAppState::removeAllButtons(Button **exceptions,int numExceptions){
		int targetId=0;
		while(targetId!=buttons.size()){
			if(!isException(exceptions,numExceptions,buttons[targetId])){
				delete buttons[targetId];	
				buttons.erase(buttons.begin()+targetId);
			}
			else
				targetId++;
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

	void GuiAppState::removeAllTextboxes(){
		while(!textboxes.empty()){
			int id=textboxes.size()-1;		
			removeButton(textboxes[id]->getTextboxButton());
			delete textboxes[id];
			textboxes.erase(textboxes.begin()+id);
		}
	}

	void GuiAppState::removeAllGUIElements(bool removeButtons,bool removeListboxes,bool removeTextboxes){
		if(removeListboxes)
			removeAllListboxes();
		if(removeTextboxes)
			removeAllTextboxes();
		if(removeButtons)
			removeAllButtons(nullptr);
	}

	void GuiAppState::onAction(Mapping::Bind bind, bool isPressed){
		glfwGetCursorPos(gm->getRoot()->getWindow(),x,y);
		switch(bind){
			case Mapping::MOUSE_CLICK:
				{
				bool clicked=false;
				if(isPressed){
					for(Button *b : buttons){
						if(!clicked){
							Vector2 pos=b->getPos(),size=b->getSize();
							if(b->isActive()&&(pos.x<=*x&&*x<=pos.x+size.x)&&(pos.y<=*y&&*y<=pos.y+size.y)){
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
			default:
				if(isPressed&&bind!=Mapping::NONE)
					activateBoundButton(bind);
		}
	}

	void GuiAppState::onRawKeyButton(short ch){
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
			if(ch!=GLFW_KEY_BACKSPACE)
				t->type(ch,shiftPressed);
			else 
				t->deleteCharacter();
			lastTypeTime=getTime();
		}
		updateControlsListbox(Mapping::KEYBOARD,true,ch);
	}

	void GuiAppState::onRawMouseButton(short s){
		updateControlsListbox(Mapping::MOUSE_KEY,1,s);	
	}

	void GuiAppState::onRawJoystickButton(short s){
		updateControlsListbox(Mapping::JOYSTICK_KEY,1,s);	
	}

	void GuiAppState::onRawJoystickAxis(short axis, float str){
		updateControlsListbox(Mapping::JOYSTICK_AXIS,str>0,axis);	
	}

	void GuiAppState::updateControlsListbox(Mapping::BindType bt, bool action, short trigger){
		Listbox *listbox=nullptr;
		for(Listbox *l : listboxes)
			if(l->getType()==Listbox::CONTROLS){
				listbox=l;
				break;
			}
		if(listbox){
			int i[]{(int)bt,action,trigger},selectedOption=listbox->getSelectedOption();
			string line=listbox->getContents()[selectedOption];
			int colonId;
			for(int i=0;i<line.length();i++)
				if(line.c_str()[i]==':')
					colonId=i;
			string key=fromIntToString(i),bind=line.substr(0,colonId+1);
			listbox->changeLine(selectedOption,bind+fromIntToString(i));
		}
	}

	void GuiAppState::activateBoundButton(Mapping::Bind bind){
		for(int i=0;i<buttons.size();i++)	
			if(buttons[i]->getMapping()->bind==bind)
				buttons[i]->onClick();
	}

	bool GuiAppState::isException(Button **e, int num, Button *b){
		bool exception=false;
		for(int i=0;i<num&&!exception;i++)
			if(b==e[i])
				exception=true;
		return exception;
	}
}
