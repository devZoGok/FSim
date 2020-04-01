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
	AircraftSelectionButton::AircraftSelectionButton(GameManager *gm, Vector2 pos, Vector2 size, string name,int aircraftId,int faction,Mapping::Bind bind) : Button(gm,pos,size,name,bind){
		this->aircraftId=aircraftId;
		this->faction=faction;
	}

	void AircraftSelectionButton::onClick(){
		StateManager *stateManager=gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		int playerId=inGameState->getNumStructures();
		Vector3 pos=Vector3(0,20,-20);
		bool helicopter=(aircraftId==CHINESE_HELICOPTER||aircraftId==JAPANESE_HELICOPTER||aircraftId==KOREAN_HELICOPTER);
		Aircraft *aircraft=helicopter?(Aircraft*)new Helicopter(gm,aircraftId,faction,pos,Quaternion(1,0,0,0),false):
		(Aircraft*)new Jet(gm,aircraftId,faction,pos,Quaternion(1,0,0,0),false);
		inGameState->addStructure(aircraft);
		inGameState->setSelectingAircraft(false);

		stateManager->attachState(helicopter?(AbstractAppState*)new HelicopterAppState(gm,playerId):(AbstractAppState*)new JetAppState(gm,playerId));
		guiState->removeAllButtons(nullptr);
	}
}
