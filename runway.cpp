#include"runway.h"
#include"abstractRunway.h"
#include"structureData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"guiAppState.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helipad.h"
#include"aircraft.h"
#include"aircraftSelectionButton.h"

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	Runway::Runway(GameManager *gm, int faction, Vector3 pos, Quaternion rot) : Building(gm,RUNWAY,faction,pos,rot){
		runway=new AbstractRunway(gm,RUNWAY,faction);
	}

	Runway::~Runway(){
	}

	void Runway::update(){
		runway->setStartPos(pos);
		runway->setStartRot(rot);

		float maxDistance=10;
		StateManager *stateManager=Structure::gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		JetAppState *jetState=(JetAppState*)stateManager->getState(AbstractAppState::JET_STATE);

		for(Structure *s : inGameState->getStructures())
			if(s->getId()==HELIPAD&&s->getPos().getDistanceFrom(Structure::pos)<maxDistance)
				runway->setAbstractHelipad(((Helipad*)s)->getAbstractHelipad());

		runway->update();
	}
}
