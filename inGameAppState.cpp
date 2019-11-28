#define MYSQLPP_MYSQL_HEADERS_BURIED

#include<mysql++.h>
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helicopterAppState.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"map.h"
#include"defConfigs.h"
#include"structure.h"
#include"unit.h"
#include"exitButton.h"
#include"optionsButton.h"
#include"mainMenuButton.h"
#include"textbox.h"
#include<root.h>
#include<quad.h>
#include<node.h>
#include<material.h>

using namespace std;
using namespace mysqlpp;
using namespace vb01;

namespace fsim{
	InGameAppState::InGameAppState(GameManager *gm,int pilotId,int faction,bool loadFromSave,int level,int objective) : AbstractAppState(gm){
		type=AbstractAppState::IN_GAME_STATE;
		this->faction=(Faction)faction;
		this->pilotId=pilotId;

		string factionSuffix;
		switch(this->faction){
			case CHINA:
				factionSuffix="Ch/Level01/";
				break;
			case JAPAN:
				factionSuffix="Jp/Level01/";
				break;
			case KOREA:
				factionSuffix="Kr/Level01/";
				break;
		}
		map=new Map(gm,PATH+"Models/Levels/"+factionSuffix,this,loadFromSave,level,objective,pilotId);
		playerId=structures.size();
		guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		
		//gm->getStateManager()->dettachState(guiState);
		
		Quad *quad=new Quad(Vector3(gm->getWidth(),gm->getHeight(),0),false);
		pauseOverlay=new Node(Vector3(0,0,.3));
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->setTexturingEnabled(false);
		mat->setDiffuseColor(Vector4(0,0,0,.5));
		quad->setMaterial(mat);
		pauseOverlay->attachMesh(quad);
	}

	InGameAppState::~InGameAppState(){}

	void InGameAppState::onAttached(){
		AbstractAppState::onAttached();
	}

	void InGameAppState::onDettached(){
		AbstractAppState::onDettached();
	}

	void InGameAppState::update(){
		map->update();
		if(!paused)
			for(Structure *s : structures)
				s->update();
	}

	void InGameAppState::togglePause(){
		class ResumeButton : public Button{
			public:
				ResumeButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"Resume"){}
				void onClick(){
					InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
					inGameState->onAction(Mapping::MAIN_MENU,true);
				}
			private:
		};
		class SaveButton : public Button{
			public:
				SaveButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"Save"){}
				void onClick(){
					class BackButton : public Button{
						public:
							BackButton(GameManager *gm) : Button(gm,Vector2(100,300),Vector2(100,50),"Back"){}
							void onClick(){
								GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
								Button *e[]{this};
								guiState->removeAllGUIElements(false);
								guiState->removeAllButtons(e,1);
								guiState->addButton(new ResumeButton(gm,Vector2(100,100),Vector2(100,50)));
								guiState->addButton(new SaveButton(gm,Vector2(100,160),Vector2(100,50)));
								guiState->addButton(new OptionsButton(gm,Vector2(100,220),Vector2(100,50)));
								guiState->addButton(new MainMenuButton(gm,Vector2(100,280),Vector2(100,50)));
								guiState->addButton(new ExitButton(gm,Vector2(100,340),Vector2(100,50)));
								guiState->removeButton(this);
							}
						private:
					};
					class OkButton : public Button{
						public:
							OkButton(GameManager *gm,Textbox *textbox) : Button(gm,Vector2(100,500),Vector2(100,50),"Ok"){
								this->textbox=textbox;
							}
							void onClick(){
								InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
								Map *map=inGameState->getMap();
								string name=textbox->getText();
								Connection conn(false);
								conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
								int numSaves=(int)conn.query("select count(*) from saves;").store()[0][0];
								int pilotId=inGameState->getPilotId(),playerId=inGameState->getPlayerId(),level=map->getLevel(),objectiveId=map->getObjective();
								conn.query("insert into saves values("+to_string(numSaves)+","+to_string(pilotId)+","+to_string(playerId)+","+to_string(level)+","+to_string(objectiveId)+",'"+name+"');").store();
								for(int i=0;i<inGameState->getNumStructures();i++){
									Structure *s=inGameState->getStructures()[i];
									int id=s->getId(),faction=s->getFaction();
									Vector3 pos=s->getPos();
									Quaternion rot=s->getRot();
									string values=to_string(numSaves)+","+to_string(id)+","+to_string(faction)+","+to_string(pos.x)+","+to_string(pos.y)+","+to_string(pos.z)+","+to_string(rot.w)+","+to_string(rot.x)+","+to_string(rot.y)+","+to_string(rot.z);
									conn.query("insert into save_units values("+values+");").store();
								}
							}
						private:
							Textbox *textbox;
					};

					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					Button *e[]{this};
					guiState->removeAllButtons(e,1);
					Textbox *t=new Textbox(gm,Vector2(100,100),Vector2(100,20));
					guiState->addTextbox(t);
					guiState->addButton(new BackButton(gm));
					guiState->addButton(new OkButton(gm,t));
					guiState->removeButton(this);
				}
			private:
		};
		StateManager *stateManager=gm->getStateManager();
		Node *guiNode=gm->getRoot()->getGuiNode();
		if(!activeState){
			JetAppState *jetState=(JetAppState*)stateManager->getState(AbstractAppState::JET_STATE);
			HelicopterAppState *helicopterState=(HelicopterAppState*)stateManager->getState(AbstractAppState::HELICOPTER_STATE);
			activeState=(jetState?(ActiveGameAppState*)jetState:(ActiveGameAppState*)helicopterState);
		}
		if(!paused){
			guiNode->attachChild(pauseOverlay);
			stateManager->dettachState(activeState);
			guiState->addButton(new ResumeButton(gm,Vector2(100,100),Vector2(100,50)));
			guiState->addButton(new SaveButton(gm,Vector2(100,160),Vector2(100,50)));
			guiState->addButton(new OptionsButton(gm,Vector2(100,220),Vector2(100,50)));
			guiState->addButton(new MainMenuButton(gm,Vector2(100,280),Vector2(100,50)));
			guiState->addButton(new ExitButton(gm,Vector2(100,340),Vector2(100,50)));
		}
		else{
			guiNode->dettachChild(pauseOverlay);
			guiState->removeAllGUIElements();
			stateManager->attachState(activeState);
		}
		paused=!paused;
	}

	void InGameAppState::onAction(Mapping::Bind bind, bool isPressed){
		switch(bind){
			case Mapping::MAIN_MENU:
				if(isPressed)togglePause();
				break;	
		}
	}

	void InGameAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
		
		}
	}
}
