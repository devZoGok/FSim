#include"abstractHelipad.h"
#include"gameManager.h"
#include"stateManager.h"
#include"guiAppState.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helipad.h"
#include"aircraft.h"
#include"structureData.h"
#include"helicopterAppState.h"
#include"helicopterAppState.h"
#include"aircraftSelectionButton.h"
#include<string>

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	AbstractHelipad::AbstractHelipad(GameManager *gm,int id,int faction){
		this->gm=gm;
		this->id=id;
		this->faction=faction;
	}

	void AbstractHelipad::update(){
		StateManager *stateManager=gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		HelicopterAppState *helicopterState=(HelicopterAppState*)stateManager->getState(AbstractAppState::HELICOPTER_STATE);

		if(helicopterState){
			ActiveGameAppState *activeState=(ActiveGameAppState*)helicopterState;
			Aircraft *aircraft=activeState->getAircraft();
			bool landed=false;
			int width=60;
			if(aircraft->getFaction()==faction){
				float minHeliDistance=12;
				if(!landed&&(aircraft->getPos()-pos).getLength()<=minHeliDistance){
					string aircraftTypes[]={"Fighter","Fighter-bomber","Helicopter"};
					Runway *runway=nullptr;
					float maxDistance=10;
					landed=true;
					inGameState->setSelectingAircraft(true);
					stateManager->dettachState(AbstractAppState::HELICOPTER_STATE);
					inGameState->setActiveState(nullptr);
					for(Structure *s : inGameState->getStructures())
						if(s->getId()==RUNWAY&&s->getPos().getDistanceFrom(pos)<maxDistance)
							runway=(Runway*)s;

					Vector3 aircraftPos[3]{
						runway?runway->getPos():Vector3(0,0,0),
						runway?runway->getPos():Vector3(0,0,0),
						pos+Vector3(0,50,0)
					};
					Quaternion aircraftRot[3]{
						runway?runway->getRot():Quaternion(1,0,0,0),
						runway?runway->getRot():Quaternion(1,0,0,0),
						rot
					};
					for(int i=runway?0:2;i<3;i++)
						guiState->addButton(new AircraftSelectionButton(gm,Vector2(100+(width+10)*i,100),Vector2(width,100),
									aircraftTypes[i],faction*3+i,faction,Mapping::Bind(Mapping::FIGHTER+i),
								aircraftPos[i],aircraftRot[i]));
					delete helicopterState;
				}
			}
		}
	}
}
