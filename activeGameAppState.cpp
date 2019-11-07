#include"activeGameAppState.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"jet.h"
#include"helicopter.h"
#include"structureData.h"
#include<glfw3.h>

namespace fsim{
	ActiveGameAppState::ActiveGameAppState(GameManager *gm,int playerId) : AbstractAppState(gm){
		type=AbstractAppState::ACTIVE_GAME_STATE;
		structures=((InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE))->getStructures();
		this->playerId=playerId;
		aircraft=(Aircraft*)structures[playerId];
	}

	void ActiveGameAppState::onAttached(){
		AbstractAppState::onAttached();
		const int numMappings=4;
		Mapping::Bind binds[numMappings]{
			Mapping::PITCH_UP,
			Mapping::PITCH_DOWN,
			Mapping::YAW_LEFT,
			Mapping::YAW_RIGHT
		};
		int triggers[numMappings]{
			Mapping::MOUSE_AXIS_UP,
			Mapping::MOUSE_AXIS_DOWN,
			Mapping::MOUSE_AXIS_LEFT,
			Mapping::MOUSE_AXIS_RIGHT
		};
		for(int i=0;i<numMappings;i++){
			Mapping *m=new Mapping;
			m->bind=binds[i];
			m->trigger=triggers[i];
			m->action=0;
			m->type=Mapping::MOUSE_AXIS;
			mappings.push_back(m);
		}
	}

	void ActiveGameAppState::onDettached(){
		AbstractAppState::onDettached();
	}

	void ActiveGameAppState::update(){}

	void ActiveGameAppState::onAction(Mapping::Bind bind, bool isPressed){
		Jet *jet=nullptr;
		Helicopter *helicopter=nullptr;
		switch(aircraft->getId()){
			case FIGHTER:
			case FIGHTER_BOMBER:
				jet=(Jet*)aircraft;
				break;
			case HELICOPTER:
				helicopter=(Helicopter*)aircraft;
				break;
		}
		switch(bind){
			case Mapping::ACCELERATE:
				break;		
		}
	}

	void ActiveGameAppState::onAnalog(Mapping::Bind bind, float str){
		Jet *jet=nullptr;
		Helicopter *helicopter=nullptr;
		switch(aircraft->getId()){
			case FIGHTER:
			case FIGHTER_BOMBER:
				jet=(Jet*)aircraft;
				break;
			case HELICOPTER:
				helicopter=(Helicopter*)aircraft;
				break;
		}
		switch(bind){
		
		}
	}
}
