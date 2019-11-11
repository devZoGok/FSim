#include"inputManager.h"
#include"gameManager.h"
#include"stateManager.h"
#include"util.h"
#include<glfw3.h>
#include<cmath>
#include<iostream>

namespace fsim{
	double *posX,*posY,strX,strY;
	int width,height;
	
	void foo(GLFWwindow *window,double newPosX,double newPosY){
		strX=(*posX-newPosX)/width,strY=(newPosY-*posY)/height;
	}

	InputManager::InputManager(GameManager *gm,GLFWwindow *window){
		this->gm=gm;
		this->stateManager=gm->getStateManager();
		this->window=window;
		posX=new double,posY=new double;
	}

	InputManager::~InputManager(){}

	void InputManager::update(){
		width=gm->getWidth(),height=gm->getHeight();

		glfwGetCursorPos(window,posX,posY);
		int joystick;
		int numAxis=3,numButtons=6;
		const u8 *buttons;
		const float *axis;
		if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
			joystick=GLFW_JOYSTICK_1;
			axis=glfwGetJoystickAxes(joystick,&numAxis);
			buttons=glfwGetJoystickButtons(joystick,&numButtons);
		}
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
							if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
								pressed=buttons[m->trigger];
							}
							break;
					}
					if((pressed&&!m->pressed)||(!pressed&&m->pressed)){
						m->pressed=pressed;	
						a->onAction(m->bind,pressed);
					}
				}
				else{
					switch(m->type){
						case Mapping::MOUSE_AXIS:
							{
								float str;
								switch(m->trigger){
									case Mapping::MOUSE_AXIS_LEFT:
									case Mapping::MOUSE_AXIS_RIGHT:
										str=strX;
										break;
									case Mapping::MOUSE_AXIS_UP:
									case Mapping::MOUSE_AXIS_DOWN:
										str=strY;
										break;
								}
								a->onAnalog(m->bind,str);
								break;
							}
						case Mapping::JOYSTICK_AXIS:
							{
								int axisId=(m->trigger-(m->trigger%2>0))/2;
								float str=fabs(axis[axisId])>=.1?axis[axisId]:0;
								if(fabs(str)>0)
									a->onAnalog(m->bind,str);
								break;
							}
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
