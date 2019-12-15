#include"airfield.h"
#include"model.h"
#include"material.h"
#include"gameManager.h"
#include"stateManager.h"
#include"defConfigs.h"
#include"jetAppState.h"
#include"jet.h"
#include"helicopterAppState.h"
#include"helicopter.h"
#include"guiAppState.h"
#include"aircraftSelectionButton.h"
#include"structureData.h"

using namespace vb01;
using namespace std;

namespace fsim{
	Airfield::Airfield(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot, int numRunways, Vector3 *runwayPos, Vector3 *runwayDirs,int numHelipads, Vector3 *helipadPos) : Building(gm,id,faction,pos,rot) {
		this->numRunways=numRunways;
		this->numHelipads=numHelipads;
		this->runwayPos=runwayPos;
		this->runwayDirs=runwayDirs;
		this->helipadPos=helipadPos;
		model=(Model*)new Node(pos,rot);
		rootNode->attachChild(model);

		for(int i=0;i<numRunways;i++){
			Model *hangarModel=new Model(PATH+"Models/Buildings/Airfield/hangar.obj");	
			Material *mat=new Material();
			mat->addDiffuseMap(defaultTexture);
			hangarModel->setPosition(runwayPos[i]);
			hangarModel->setMaterial(mat);
			model->attachChild(hangarModel);
		}

		for(int i=0;i<numHelipads;i++){
			Model *helipadModel=new Model(PATH+"Models/Buildings/Airfield/helipad.obj");	
			Material *mat=new Material();
			mat->addDiffuseMap(defaultTexture);
			helipadModel->setPosition(helipadPos[i]);
			helipadModel->setMaterial(mat);
			model->attachChild(helipadModel);
		}
	}

	void Airfield::update(){
		StateManager *stateManager=gm->getStateManager();
		GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
		JetAppState *jetState=(JetAppState*)stateManager->getState(AbstractAppState::JET_STATE);
		HelicopterAppState *helicopterState=(HelicopterAppState*)stateManager->getState(AbstractAppState::HELICOPTER_STATE);
		if(jetState||helicopterState){
			bool landed=false;
			ActiveGameAppState *activeState=(jetState?(ActiveGameAppState*)jetState:(ActiveGameAppState*)helicopterState);
			Aircraft *aircraft=activeState->getAircraft();
			string aircraftTypes[]={"Fighter","Fighter-bomber","Helicopter"};
			int width=60;
			if(aircraft->getId()<Type::HELICOPTER){
				float minJetDist=4,minJetHeight=3,minDirAngle=.17,minPosAngle=.5;
				for(int i=0;i<numRunways&&!landed;i++){
					Vector3 jetPos=aircraft->getPos(),jetDir=aircraft->getDir();
					Vector3 rp=pos+runwayPos[i];
					if(fabs(jetPos.y-rp.y)<=minJetHeight&&
							(rp-jetPos).getLength()<=minJetDist&&
							(-runwayDirs[i]).getAngleBetween(Vector3(jetDir.x,0,jetDir.z).norm())<=minDirAngle&&
							runwayDirs[i].getAngleBetween((jetPos-rp).norm())<=minPosAngle){
						landed=true;
						stateManager->dettachState(AbstractAppState::JET_STATE);
						for(int j=0;j<3;j++)
							guiState->addButton(new AircraftSelectionButton(gm,Vector2(100+(width+10)*j,100),Vector2(width,100),aircraftTypes[j],j,Mapping::Bind(Mapping::FIGHTER+i)));
						delete jetState;
					}
				}
			}
			else{
				float minHeliDistance=12;
				for(int i=0;i<numHelipads&&!landed;i++){
					if((aircraft->getPos()-(pos+helipadPos[i])).getLength()<=minHeliDistance){
						landed=true;
						stateManager->dettachState(AbstractAppState::HELICOPTER_STATE);
						for(int j=0;j<3;j++)
							guiState->addButton(new AircraftSelectionButton(gm,Vector2(100+(width+10)*j,100),Vector2(width,100),aircraftTypes[j],j,Mapping::Bind(Mapping::FIGHTER+i)));
						delete helicopterState;
					}
				}
			}
		}
	}
}
