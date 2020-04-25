#include"aircraftSelectionButton.h"
#include"jetAppState.h"
#include"jet.h"
#include"helicopterAppState.h"
#include"helicopter.h"
#include"guiAppState.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"structureData.h"

using namespace fsim::structureData;
using namespace vb01;
using namespace std;

namespace fsim{
	AircraftSelectionButton::AircraftSelectionButton(GameManager *gm,Vector2 pos,Vector2 size,string name,
			int aircraftId,int faction,Mapping::Bind bind,Vector3 aircraftPos,Quaternion aircraftRot) : Button(gm,pos,size,name,bind){
		this->aircraftId=aircraftId;
		this->faction=faction;
		this->aircraftPos=aircraftPos;
		this->aircraftRot=aircraftRot;
	}

	void AircraftSelectionButton::onClick(){
		StateManager *stateManager=gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		int playerId=inGameState->getNumStructures();
		//Vector3 pos=Vector3(0,20,-20);
		bool helicopter=(aircraftId==CHINESE_HELICOPTER||aircraftId==JAPANESE_HELICOPTER||aircraftId==KOREAN_HELICOPTER);
		Aircraft *aircraft=helicopter?(Aircraft*)new Helicopter(gm,aircraftId,faction,aircraftPos,aircraftRot,false):
		(Aircraft*)new Jet(gm,aircraftId,faction,aircraftPos,aircraftRot,false);
		inGameState->addStructure(aircraft);
		inGameState->setSelectingAircraft(false);
		ActiveGameAppState *activeState=helicopter?(ActiveGameAppState*)new HelicopterAppState(gm,playerId):(ActiveGameAppState*)new JetAppState(gm,playerId);
		stateManager->attachState((AbstractAppState*)activeState);
		inGameState->setActiveState(activeState);
		guiState->removeAllButtons(nullptr);
	}
}
