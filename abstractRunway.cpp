#include"abstractRunway.h"
#include"abstractHelipad.h"
#include"gameManager.h"
#include"stateManager.h"
#include"guiAppState.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helipad.h"
#include"aircraft.h"
#include"structureData.h"
#include"aircraftSelectionButton.h"
#include<string>

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	AbstractRunway::AbstractRunway(GameManager *gm,int id, int faction){
		this->gm=gm;
		this->id=id;
		this->faction=faction;
	}

	void AbstractRunway::update(){
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
				Vector3 runwayDir=startRot*Vector3(0,0,1);
				Vector3 jetPos=aircraft->getPos(),jetDir=aircraft->getDir();
				Vector3 rp=startPos;
				if(fabs(jetPos.y-rp.y)<=minJetHeight&&
					(rp-jetPos).getLength()<=minJetDist&&
					(-runwayDir).getAngleBetween(Vector3(jetDir.x,0,jetDir.z).norm())<=minDirAngle&&
					runwayDir.getAngleBetween((jetPos-rp).norm())<=minPosAngle){
					string aircraftTypes[]={"Fighter","Fighter-bomber","Helicopter"};
					landed=true;

					inGameState->setSelectingAircraft(true);
					inGameState->setActiveState(nullptr);
					stateManager->dettachState(AbstractAppState::JET_STATE);

					Vector3 aircraftPos[3]{
						startPos,
						startPos,
						helipad?helipad->getStartPos()+Vector3(0,50,0):Vector3(0,0,0)
					};
					Quaternion aircraftRot[3]{
						startRot,
						startRot,
						helipad?helipad->getStartRot():Quaternion(1,0,0,0)
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
