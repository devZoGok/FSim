#include"stateManager.h"

namespace fsim{
	StateManager::StateManager(){}

	StateManager::~StateManager(){}

	void StateManager::update(){
		for(AbstractAppState *a : appStates)
			a->update();
	}

	void StateManager::attachState(AbstractAppState *a){
		appStates.push_back(a);
		a->onAttached();
	}

	void StateManager::dettachState(AbstractAppState *a){
		int id=-1;
		for(int i=0;i<appStates.size()&&id==-1;i++)
			if(appStates[i]==a)
				id=i;
		if(id!=-1)
			appStates.erase(appStates.begin()+id);
	}

	void StateManager::dettachState(AbstractAppState::Type type){
		int id=-1;
		for(int i=0;i<appStates.size()&&id==-1;i++)
			if(appStates[i]->getType()==type)
				id=i;
		if(id!=-1)
			appStates.erase(appStates.begin()+id);
	}

	AbstractAppState* StateManager::getState(AbstractAppState::Type type){
		AbstractAppState *a=nullptr;	
		for(int i=0;i<appStates.size()&&!a;i++)
			if(appStates[i]->getType()==type)
				a=appStates[i];
		return a;
	}
}
