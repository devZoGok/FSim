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
				switch(m->type){
					case Mapping::KEYBOARD:
						break;
					case Mapping::MOUSE:
						if(m->action){
							bool pressed=glfwGetMouseButton(window,m->trigger);
							if((pressed&&!m->pressed)||(!pressed&&m->pressed)){
								m->pressed=pressed;	
								a->onAction(m->bind,pressed);
							}
						}
						break;
					case Mapping::JOYSTICK:
						break;
				}	
			}	
		}	
	}
}
