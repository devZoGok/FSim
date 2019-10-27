#include"inputManager.h"
#include"stateManager.h"
#include"util.h"
#include<glfw3.h>
#include<cmath>
#include<iostream>

namespace fsim{
	double *x,*y;
	
	void foo(GLFWwindow *window,double X,double Y){}

	InputManager::InputManager(StateManager *stateManager,GLFWwindow *window){
		this->stateManager=stateManager;
		this->window=window;
		x=new double,y=new double;
	}

	InputManager::~InputManager(){}

	void InputManager::update(){
		glfwGetCursorPos(window,x,y);
				int numAxis=3,numButtons=6;
		for(AbstractAppState *a : stateManager->getAppStates()){
			for(Mapping *m : a->getMappings()){
				if(m->action){
					bool pressed;
					switch(m->type){
						case Mapping::KEYBOARD:
							pressed=glfwGetKey(window,m->trigger);
							break;
						case Mapping::MOUSE_KEY:
							pressed=glfwGetMouseButton(window,m->trigger);
							break;
						case Mapping::JOYSTICK_KEY:
							if(glfwJoystickPresent(GLFW_JOYSTICK_1)){}
							break;
					}
					if((pressed&&!m->pressed)||(!pressed&&m->pressed)){
						m->pressed=pressed;	
						a->onAction(m->bind,pressed);
					}
				}
			}
			for(int i=0;i<350;i++){
				if(glfwGetKey(window,i))
					a->onRawKeyButton(i);
				else if(glfwGetMouseButton(window,i))
					a->onRawMouseButton(i);
			}

			glfwSetCursorPosCallback(window,foo);

			if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
				int joystick=GLFW_JOYSTICK_1;
				const float *axis=glfwGetJoystickAxes(joystick,&numAxis);
				const u8 *buttons=glfwGetJoystickButtons(joystick,&numButtons);
				for(int i=0;i<numAxis;i++)
					if(abs(axis[i])==1)
						a->onRawJoystickAxis(i,axis[i]);
				for(int i=0;i<numButtons;i++)
					if(buttons[i])
						a->onRawJoystickButton(i);
			}
		}
	}
}
