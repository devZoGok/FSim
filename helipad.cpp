#include"helipad.h"
#include"abstractHelipad.h"
#include"structureData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"guiAppState.h"
#include"helicopterAppState.h"
#include"runway.h"
#include"aircraft.h"
#include"aircraftSelectionButton.h"

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	Helipad::Helipad(GameManager *gm, int faction, Vector3 pos) : Building(gm,HELIPAD,faction,pos,Quaternion(1,0,0,0)){
		helipad=new AbstractHelipad(gm,HELIPAD,faction);
	}

	Helipad::~Helipad(){
	}

	void Helipad::update(){
		helipad->setStartPos(pos);
		helipad->setStartRot(rot);

		float maxDistance=10;
		StateManager *stateManager=Structure::gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		HelicopterAppState *helicopterState=(HelicopterAppState*)stateManager->getState(AbstractAppState::HELICOPTER_STATE);

		for(Structure *s : inGameState->getStructures())
			if(s->getId()==RUNWAY&&s->getPos().getDistanceFrom(Structure::pos)<maxDistance)
				helipad->setAbstractRunway(((Runway*)s)->getAbstractRunway());

		helipad->update();
	}
}
