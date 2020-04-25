#include"runway.h"
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
	}

	Runway::~Runway(){
	}

	void Runway::update(){
		StateManager *stateManager=gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
		JetAppState *jetState=(JetAppState*)stateManager->getState(AbstractAppState::JET_STATE);

		if(jetState){
			ActiveGameAppState *activeState=(ActiveGameAppState*)jetState;
			Aircraft *aircraft=activeState->getAircraft();
			bool landed=false;
			int width=60;
			if(!landed&&aircraft->getFaction()==faction){
				float minJetDist=4,minJetHeight=3,minDirAngle=.17,minPosAngle=.5;
				Vector3 runwayDir=rot*Vector3(0,0,1);
				Vector3 jetPos=aircraft->getPos(),jetDir=aircraft->getDir();
				Vector3 rp=pos;
				if(fabs(jetPos.y-rp.y)<=minJetHeight&&
					(rp-jetPos).getLength()<=minJetDist&&
					(-runwayDir).getAngleBetween(Vector3(jetDir.x,0,jetDir.z).norm())<=minDirAngle&&
					runwayDir.getAngleBetween((jetPos-rp).norm())<=minPosAngle){
					string aircraftTypes[]={"Fighter","Fighter-bomber","Helicopter"};
					Helipad *helipad=nullptr;
					float maxDistance=10;
					landed=true;

					inGameState->setSelectingAircraft(true);
					inGameState->setActiveState(nullptr);
					stateManager->dettachState(AbstractAppState::JET_STATE);
					for(Structure *s : inGameState->getStructures())
						if(s->getId()==HELIPAD&&s->getPos().getDistanceFrom(pos)<maxDistance)
							helipad=(Helipad*)s;

					Vector3 aircraftPos[3]{
						pos,
						pos,
						helipad?helipad->getPos()+Vector3(0,50,0):Vector3(0,0,0)
					};
					Quaternion aircraftRot[3]{
						rot,
						rot,
						helipad?helipad->getRot():Quaternion(1,0,0,0)
					};
					for(int i=0;i<(helipad?3:2);i++)
						guiState->addButton(new AircraftSelectionButton(gm,Vector2(100+(width+10)*i,100),Vector2(width,100),
									aircraftTypes[i],faction*3+i,faction,Mapping::Bind(Mapping::FIGHTER+i),
								aircraftPos[i],aircraftRot[i]));
					delete jetState;
				}
			}
		}
	}
}
