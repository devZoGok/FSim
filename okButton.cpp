#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"playButton.h"
#include"okButton.h"
#include"mainMenuButton.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"textbox.h"
#include"listbox.h"
#include"defConfigs.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helicopterAppState.h"
#include"jet.h"
#include"helicopter.h"
#include"structureData.h"
#include<root.h>
#include<node.h>
#include<mysql++.h>
#include<cstdlib>

using namespace vb01;
using namespace std;
using namespace mysqlpp;

namespace fsim{
	OkButton::OkButton(GameManager *gm,Vector2 pos, Vector2 size,Textbox *textbox,int pilotId) : Button(gm,pos,size,"Ok",true){
		this->textbox=textbox;
		this->pilotId=pilotId;
	}

	void OkButton::onClick(){
		class AircraftTabButton : public Button{
			public:
				AircraftTabButton(GameManager *gm, Vector2 pos, Vector2 size, string name, string upgrades[5]) : Button(gm,pos,size,name,true){
					for(int i=0;i<5;i++)
						this->upgrades[i]=upgrades[i];
				}
				void onClick(){
					active=false;
					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					Node *guiNode=gm->getRoot()->getGuiNode();
					for(int i=0;i<5;i++){
						Text *upgradeText=new Text(PATH+"Fonts/batang.ttf",upgrades[i]);
						upgradeText->setScale(.2);
						Node *textNode=new Node(Vector3(50,300+i*50,.5));
						textNode->addText(upgradeText);
						guiNode->attachChild(textNode);
						textNodes[i]=textNode;

						for(int j=0;j<4;j++){
							upgradeButtons[4*i+j]=new UpgradeButton(gm,Vector2(50+j*50,300+i*50),Vector2(40,40));
							guiState->addButton(upgradeButtons[4*i+j]);
						}
					}
				}
				Node* getTextNode(int i){return textNodes[i];}
			private:
				class UpgradeButton : public Button{
					public:
						UpgradeButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"",true,PATH+"Icons/upgrade.jpg"){}
						void onClick(){
					
						}
					private:
				};
				string upgrades[5];
				Node *textNodes[5]{0,0,0,0,0};
				UpgradeButton *upgradeButtons[20]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		};
		class StartButton : public Button{
			public:
				StartButton(GameManager *gm, Vector2 pos, Vector2 size, AircraftTabButton *tabs[3], int pilotId, int faction,bool loadFromSave) : Button(gm,pos,size,"Start"){
					this->faction=faction;
					this->pilotId=pilotId;
					this->loadFromSave=loadFromSave;
					for(int i=0;i<3;i++)
						this->tabs[i]=tabs[i];
				}
				void onClick(){
					class AircraftSelectionButton : public Button{
						public:
							AircraftSelectionButton(GameManager *gm, Vector2 pos, Vector2 size, string name,int aircraftId) : Button(gm,pos,size,name){this->aircraftId=aircraftId;}
							void onClick(){
								StateManager *stateManager=gm->getStateManager();
								GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
								InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
								int playerId=inGameState->getNumStructures();
								Vector3 pos=Vector3(0,20,-20);
								Aircraft *aircraft=aircraftId==2?(Aircraft*)new Helicopter(gm,aircraftId,pos,Quaternion(1,0,0,0)):
									(Aircraft*)new Jet(gm,aircraftId,pos,Quaternion(1,0,0,0));
								inGameState->addStructure(aircraft);

								stateManager->attachState(aircraftId==2?(AbstractAppState*)new HelicopterAppState(gm,playerId):(AbstractAppState*)new JetAppState(gm,playerId));
								guiState->removeAllButtons(nullptr);
							}
						private:
							int aircraftId;
					};

					StateManager *stateManager=gm->getStateManager();
					GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
					Node *guiNode=gm->getRoot()->getGuiNode();

					for(int i=0;i<5;i++){
						guiNode->dettachChild(tabs[faction]->getTextNode(i));
						delete tabs[faction]->getTextNode(i);
					}

					const int numExceptions=!loadFromSave?4:2;
					int playerUnitId=0,level=0,objective=0;
					Button *exceptions[numExceptions];
					exceptions[0]=this;

					int width=60;
					AircraftSelectionButton *selectionButtons[3];
					string aircraft[]={"Fighter","Fighter-bomber","Helicopter"};
					for(int i=0;i<3;i++){
						selectionButtons[i]=new AircraftSelectionButton(gm,Vector2(100+(width+10)*i,100),Vector2(width,100),aircraft[i],i);
						guiState->addButton(selectionButtons[i]);
					}

					for(int i=1;i<numExceptions;i++)
						exceptions[i]=selectionButtons[i-1];

					stateManager->attachState(new InGameAppState(gm,pilotId,faction,false,level,objective));	
					guiState->removeAllButtons(exceptions,numExceptions);
					guiState->removeButton(this);
				}
			private:
				bool loadFromSave;
				int faction,pilotId;
				AircraftTabButton *tabs[3];
		};

		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);

		const int numUpgrades=5;
		string tabNames[]={"Fighter","Fighter_bomber","Helicopter"};
		string upgrades[][numUpgrades]={
			{"Airframe","Fuel_tank","Machinegun","AAMs","Countermeasures"},
			{"Airframe","Fuel_tank","Machinegun","Bombs","Countermeasures"},
			{"Airframe","Fuel_tank","Machinegun","ASMs","Countermeasures"}
		};

		Connection conn(false);					
		conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
		if(textbox){
			int numPilots=(int)conn.query("select count(*) from pilots;").store()[0][0];
			pilotId=numPilots;
			string playerName=textbox->getText(),queryText="insert into pilots values("+to_string(numPilots)+","+to_string(faction)+",'"+playerName+"'";
			for(int i=0;i<3;i++){
				queryText+=",'";
			for(int j=0;j<numUpgrades;j++)
				queryText+="0";
				queryText+="'";
			}
			queryText+=");";
			conn.query(queryText).store();
		}
		else{
			faction=(int)conn.query("select faction from pilots where pid="+to_string(pilotId)+";").store()[0][0];
		}

		Button *e[]{this};
		guiState->removeAllGUIElements(false);
		guiState->removeAllButtons(e,1);

		if(textbox){
			AircraftTabButton *tabs[3];
			int width=60;
			for(int i=0;i<3;i++){
				AircraftTabButton *tab=new AircraftTabButton(gm,Vector2(100+i*(width+10),300),Vector2(width,20),tabNames[i],upgrades[i]);
				guiState->addButton(tab);
				tabs[i]=tab;
			}
			guiState->getButton("Fighter")->onClick();
			guiState->addButton(new StartButton(gm,Vector2(500,500),Vector2(100,50),tabs,pilotId,faction,textbox==nullptr));
		}
		else{
			StateManager *stateManager=gm->getStateManager();
			Connection conn(false);					
			conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
			StoreQueryResult res=conn.query("select uid,lid,oid,puid from pilots p inner join saves s inner join save_units su on p.pid=s.pid and s.sid=su.sid where p.pid="+to_string(pilotId)+";").store();
			int unitId=(int)res[1][0],level=(int)res[0][1],objective=(int)res[0][2],playerId=(int)res[0][3];
			stateManager->attachState(new InGameAppState(gm,pilotId,faction,true,level,objective));
			switch(unitId){
				case Type::HELICOPTER:
					stateManager->attachState(new HelicopterAppState(gm,playerId));
					break;
				default:
					stateManager->attachState(new JetAppState(gm,playerId));
					break;
			}
		}
		guiState->removeButton(this);
	}
}
