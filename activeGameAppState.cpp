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
		structures=((InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE))->getStructures();
		this->playerId=playerId;
	}

	void ActiveGameAppState::onAttached(){
		AbstractAppState::onAttached();
	}

	void ActiveGameAppState::onDettached(){
		AbstractAppState::onDettached();
	}

	void ActiveGameAppState::update(){}
}
