#include"gameManager.h"
#include"inputManager.h"
#include"stateManager.h"
#include<root.h>

using namespace vb01;

namespace fsim{
	GameManager::GameManager(){
		root = Root::getSingleton();
		width=800,height=600;
		running=true;

		root->start(width,height);

		stateManager=new StateManager();
		inputManager=new InputManager(stateManager,root->getWindow());
	}

	GameManager::~GameManager(){}

	void GameManager::update(){
		root->update();	

		stateManager->update();
		inputManager->update();
	}
}
