#include"activeGameAppState.h"
#include<glfw3.h>

namespace fsim{
	ActiveGameAppState::ActiveGameAppState(int playerId){
		type=AbstractAppState::ACTIVE_GAME_STATE;
		this->playerId=playerId;
	}

	void ActiveGameAppState::onAttached(){
		AbstractAppState::onAttached();
	}

	void ActiveGameAppState::onDettached(){
		AbstractAppState::onDettached();
	}

	void ActiveGameAppState::update(){}

	void ActiveGameAppState::onAction(Mapping::Bind bind, bool isPressed){}

	void ActiveGameAppState::onAnalog(Mapping::Bind bind, float str){}
}
