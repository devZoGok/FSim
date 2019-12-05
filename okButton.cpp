#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"playButton.h"
#include"upgradeData.h"
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
	OkButton::OkButton(GameManager *gm,Vector2 pos, Vector2 size,Textbox *textbox,int saveId,int pilotId) : Button(gm,pos,size,"Ok",true){
		this->textbox=textbox;
		this->pilotId=pilotId;
		this->saveId=saveId;
	}

	void OkButton::onClick(){
		class AircraftTabButton : public Button{
			public:
				class UpgradeButton : public Button{
					public:
						UpgradeButton(GameManager *gm, Vector2 pos, Vector2 size,AircraftTabButton *tab,int type, int level) : Button(gm,pos,size,"",true,PATH+"Icons/upgrade.jpg"){
							this->tab=tab;
							this->type=type;
							this->level=level;
						}
						void onClick(){
							int *upgradeLevels=tab->getUpgradeLevels();
							if(upgradeLevels[type]==level){
								upgradeLevels[type]++;
								string column,value="";
								switch(tab->getAircraftTypeId()){
									case 0:
										column="fighter_upgrades";
										break;
									case 1:
										column="fighter_bomber_upgrades";
										break;
									case 2:
										column="helicopter_upgrades";
										break;
								}
								for(int i=0;i<numUpgrades;i++){
									value+=to_string(upgradeLevels[i]);
								}
								Connection conn(false);					
								conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
								conn.query("update pilots set "+column+"='"+value+"' where pid="+to_string(tab->getPilotId())+";").store();
								int *score=tab->getScore();
								*score-=price;
								tab->getScoreNode()->getText(0)->setText(to_string(*score));
							}
						}
					private:
						int type,level;
						AircraftTabButton *tab;
				};

				AircraftTabButton(GameManager *gm, Vector2 pos, Vector2 size,Node *scoreNode,int pilotId, int aircraftTypeId,int *upgradeLevels,int *score) : Button(gm,pos,size,tabNames[aircraftTypeId],true){
					this->upgradeLevels=upgradeLevels;
					this->aircraftTypeId=aircraftTypeId;
					this->pilotId=pilotId;
					this->score=score;
					this->scoreNode=scoreNode;
					for(int i=0;i<numUpgrades*numLevels;i++)
						upgradeButtons[i]=nullptr;
					for(int i=0;i<numUpgrades;i++)
						textNodes[i]=nullptr;
				}
				~AircraftTabButton(){
					if(!active){
						Node *guiNode=gm->getRoot()->getGuiNode();
						guiNode->dettachChild(scoreNode);
						delete scoreNode;
						delete score;
					}
				}
				void onClick(){
					active=false;
					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					Node *guiNode=gm->getRoot()->getGuiNode();

					for(int i=0;i<3;i++){
						if(i!=aircraftTypeId){
							AircraftTabButton *b=(AircraftTabButton*)guiState->getButton(tabNames[i]);
							b->setActive(true);
							for(int j=0;j<numUpgrades;j++){
								Node *textNode=b->getTextNode(j);
								if(textNode){
									b->setTextNode(nullptr,j);
									guiNode->dettachChild(textNode);
									delete textNode;
								}
								for(int z=0;z<numLevels;z++){
									int id=numLevels*j+z;
									UpgradeButton *u=b->getUpgradeButton(id);
									if(u){
										b->setUpgradeButton(nullptr,id);
										delete u;
									}
								}
							}
						}
					}

					for(int i=0;i<numUpgrades;i++){
						Text *upgradeText=new Text(PATH+"Fonts/batang.ttf",upgrades[aircraftTypeId][i]);
						upgradeText->setScale(.2);
						Node *textNode=new Node(Vector3(50,300+i*50,.5));
						textNode->addText(upgradeText);
						guiNode->attachChild(textNode);
						textNodes[i]=textNode;

						for(int j=0;j<numLevels;j++){
							UpgradeButton *u=new UpgradeButton(gm,Vector2(50+j*50,300+i*50),Vector2(40,40),this,i,j);
							upgradeButtons[numLevels*i+j]=u;
							guiState->addButton(u);
						}
					}
				}
				UpgradeButton* getUpgradeButton(int i){return upgradeButtons[i];}
				void setUpgradeButton(UpgradeButton *u,int i){this->upgradeButtons[i]=u;}
				Node* getTextNode(int i){return textNodes[i];}
				Node* getScoreNode(){return scoreNode;}
				void setTextNode(Node *node,int i){this->textNodes[i]=node;}
				int* getUpgradeLevels(){return upgradeLevels;}
				int getAircraftTypeId(){return aircraftTypeId;}
				int getPilotId(){return pilotId;}
				int* getScore(){return score;}
			private:
				Node *scoreNode;
				int *upgradeLevels,aircraftTypeId,*score,pilotId;
				Node *textNodes[numUpgrades];
				UpgradeButton *upgradeButtons[numUpgrades*numLevels];
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
								int playerId=inGameState->getNumStructures(),faction=0;
								Vector3 pos=Vector3(0,20,-20);
								Aircraft *aircraft=aircraftId==2?(Aircraft*)new Helicopter(gm,aircraftId,faction,pos,Quaternion(1,0,0,0)):
									(Aircraft*)new Jet(gm,aircraftId,faction,pos,Quaternion(1,0,0,0));
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
					int playerUnitId=0,level=1,objective=0;
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

					stateManager->attachState(new InGameAppState(gm,pilotId,-1,faction,-1,level,objective));	
					guiState->removeAllButtons(exceptions,numExceptions);
					guiState->removeButton(this);
				}
			private:
				bool loadFromSave;
				int faction,pilotId;
				AircraftTabButton *tabs[3];
		};

		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		Node *guiNode=gm->getRoot()->getGuiNode();

		Connection conn(false);					
		conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
		int pilotId=(int)conn.query("select count(*) from pilots;").store()[0][0];
		string playerName=textbox->getText(),queryText="insert into pilots values("+to_string(pilotId)+","+to_string(faction)+",'"+playerName+"'";
		for(int i=0;i<3;i++){
			queryText+=",'";
		for(int j=0;j<numUpgrades;j++)
			queryText+="0";
			queryText+="'";
		}
		queryText+=");";
		conn.query(queryText).store();

		conn.query("insert into stats values("+to_string(pilotId)+",0,0,0,0,0,0);").store();
		StoreQueryResult res=conn.query("select fighter_upgrades,fighter_bomber_upgrades,helicopter_upgrades from pilots where pid="+to_string(pilotId)+";").store();
		string s[3]={
			(string)res[0][0],
			(string)res[0][1],
			(string)res[0][2]
		};

		int *score=new int;
		*score=(int)conn.query("select score from stats where pid="+to_string(pilotId)+";").store()[0][0];
		int **upgradeLevels=new int*[3];
		for(int i=0;i<3;i++)
			upgradeLevels[i]=new int[numUpgrades];
		Text *scoreText=new Text(PATH+"Fonts/batang.ttf",to_string(*score));
		Node *textNode=new Node(Vector3(200,100,.5));
		textNode->addText(scoreText);
		guiNode->attachChild(textNode);

		for(int i=0;i<3;i++){
			for(int j=0;j<numUpgrades;j++)
				upgradeLevels[i][j]=atoi(s[i].substr(j,1).c_str());
		}
		

		Button *e[]{this};
		guiState->removeAllGUIElements(false);
		guiState->removeAllButtons(e,1);

		AircraftTabButton *tabs[3];
		int width=60;
		for(int i=0;i<3;i++){
			AircraftTabButton *tab=new AircraftTabButton(gm,Vector2(100+i*(width+60),250),Vector2(width,20),textNode,pilotId,i,upgradeLevels[i],score);
			guiState->addButton(tab);
			tabs[i]=tab;
		}
		guiState->getButton("Fighter")->onClick();
		guiState->addButton(new StartButton(gm,Vector2(500,500),Vector2(100,50),tabs,pilotId,faction,textbox==nullptr));
		
		guiState->removeButton(this);
	}
}
