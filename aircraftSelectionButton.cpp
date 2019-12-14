#include"aircraftSelectionButton.h"
#include"jetAppState.h"
#include"jet.h"
#include"helicopterAppState.h"
#include"helicopter.h"
#include"guiAppState.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"

using namespace vb01;
using namespace std;

namespace fsim{
	AircraftSelectionButton::AircraftSelectionButton(GameManager *gm, Vector2 pos, Vector2 size, string name,int aircraftId) : Button(gm,pos,size,name){
		this->aircraftId=aircraftId;
	}

	void AircraftSelectionButton::onClick(){
		StateManager *stateManager=gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		int playerId=inGameState->getNumStructures(),faction=0;
		Vector3 pos=Vector3(0,20,-20);
		Aircraft *aircraft=aircraftId==2?(Aircraft*)new Helicopter(gm,aircraftId,faction,pos,Quaternion(1,0,0,0)):
		(Aircraft*)new Jet(gm,aircraftId,faction,pos,Quaternion(1,0,0,0));
		inGameState->addStructure(aircraft);

		stateManager->attachState(aircraftId==2?(AbstractAppState*)new HelicopterAppState(gm,playerId):(AbstractAppState*)new JetAppState(gm,playerId));
		guiState->removeAllButtons(nullptr);
	}
}
