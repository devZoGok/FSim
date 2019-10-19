#include"inputManager.h"
#include"stateManager.h"
#include<glfw3.h>

namespace fsim{
	InputManager::InputManager(StateManager *stateManager,GLFWwindow *window){
		this->stateManager=stateManager;
		this->window=window;
	}

	InputManager::~InputManager(){}

	void InputManager::update(){
		for(AbstractAppState *a : stateManager->getAppStates()){
			for(Mapping *m : a->getMappings()){
				if(m->action){
					bool pressed;
					switch(m->type){
						case Mapping::KEYBOARD:
							pressed=glfwGetKey(window,m->trigger);
							break;
						case Mapping::MOUSE:
							pressed=glfwGetMouseButton(window,m->trigger);
							break;
						case Mapping::JOYSTICK:
							break;
					}
					if((pressed&&!m->pressed)||(!pressed&&m->pressed)){
						m->pressed=pressed;	
						a->onAction(m->bind,pressed);
					}
				}
			}
			for(int i=0;i<260;i++){
				if(glfwGetKey(window,i))
					a->onRawKeyButton(i);
				else if(glfwGetMouseButton(window,i))
					a->onRawMouseButton(i);
			}
		}
	}
}
