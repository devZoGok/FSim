#include"playButton.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"textbox.h"
#include"defConfigs.h"
#include"inGameAppState.h"
#include"activeGameAppState.h"
#include"aircraft.h"
#include<root.h>
#include<node.h>

using namespace vb01;
using namespace std;

namespace fsim{
	PlayButton::PlayButton(GameManager *gm,Vector2 pos, Vector2 size):Button(gm,pos,size,"Play",true){}

	void PlayButton::onClick(){
		class FactionButton : public Button{
			public:
				FactionButton(GameManager *gm, Vector2 pos, Vector2 size, int faction,int &factionId):Button(gm,pos,size,to_string(faction),true){
					this->faction=faction;	
				}
				void onClick(){factionId=faction;}
			private:
				int factionId,faction;
		};

		class OkButton : public Button{
			public:
				OkButton(GameManager *gm,Vector2 pos, Vector2 size,Textbox *textbox,int &faction) : Button(gm,pos,size,"Ok",true){
					this->textbox=textbox;
					this->faction=faction;
				}
				void onClick(){
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
							StartButton(GameManager *gm, Vector2 pos, Vector2 size, AircraftTabButton *tabs[3], int faction) : Button(gm,pos,size,"Start"){
								this->faction=faction;
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
											inGameState->setPlayerId(playerId);
											inGameState->addStructure(new Aircraft(gm,aircraftId,Vector3(0,20,0),Quaternion(1,0,0,0)));
											ActiveGameAppState *activeGameState=new ActiveGameAppState(playerId);	

											stateManager->attachState(activeGameState);
											guiState->removeAllButtons(nullptr);
										}
									private:
										int aircraftId;
								};

								StateManager *stateManager=gm->getStateManager();
								GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
								Node *guiNode=gm->getRoot()->getGuiNode();

								int width=60;
								AircraftSelectionButton *selectionButtons[3];
								string aircraft[]={"Fighter","Fighter-bomber","Helicopter"};
								for(int i=0;i<3;i++){
									selectionButtons[i]=new AircraftSelectionButton(gm,Vector2(100+(width+10)*i,100),Vector2(width,100),aircraft[i],i);
									guiState->addButton(selectionButtons[i]);
								}

								stateManager->attachState(new InGameAppState(gm,faction));	
								for(int i=0;i<5;i++){
									guiNode->dettachChild(tabs[faction]->getTextNode(i));
									delete tabs[faction]->getTextNode(i);
								}

								Button *exceptions[4];
								exceptions[0]=this;
								for(int i=1;i<4;i++)
									exceptions[i]=selectionButtons[i-1];
								guiState->removeAllButtons(exceptions,4);
								guiState->removeButton(this);
							}
						private:
							int faction;
							AircraftTabButton *tabs[3];
					};

					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					string playerInfo=textbox->getText()+":";
					for(int i=0;i<4*15;i++){
						playerInfo+="0,";
					}
					playerInfo+="0";
					std::vector<string> lines;
					readFile(PATH+"../playerList.cfg",lines);
					lines.push_back(playerInfo);
					writeFile(PATH+"../playerList.cfg",lines);

					string tabNames[]={"Fighter","Fighter-bomber","Helicopter"};
					string upgrades[][5]={
						{"Airframe","Fuel tank","Machinegun","AAMs","Countermeasures"},
						{"Airframe","Fuel tank","Machinegun","Bombs","Countermeasures"},
						{"Airframe","Fuel tank","Machinegun","ASMs","Countermeasures"}
					};
					int width=60;
					AircraftTabButton *tabs[3];
					for(int i=0;i<3;i++){
						AircraftTabButton *tab=new AircraftTabButton(gm,Vector2(100+i*(width+10),300),Vector2(width,20),tabNames[i],upgrades[i]);
						guiState->addButton(tab);
						tabs[i]=tab;
					}
					guiState->getButton("Fighter")->onClick();
					guiState->addButton(new StartButton(gm,Vector2(500,500),Vector2(100,50),tabs,faction));

					for(int i=0;i<3;i++)
						guiState->removeButton(to_string(i));
					guiState->removeTextbox(textbox);
					guiState->removeButton(this);
				}
			private:
				Textbox *textbox;
				int faction;
		};

		GuiAppState *guiState=((GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE));
		int width=200,factionId=0;
		for(int i=0;i<3;i++)
			guiState->addButton(new FactionButton(gm,Vector2(50+i*(width+10),10),Vector2(width,400),i,factionId));
		Textbox *t=new Textbox(gm,Vector2(300,550),Vector2(150,40));
		guiState->addTextbox(t);
		guiState->addButton(new OkButton(gm,Vector2(480,550),Vector2(100,40),t,factionId));

		guiState->removeButton("Exit");
		guiState->removeButton("Load");
		guiState->removeButton("Options");
		guiState->removeButton(this);
	}
}
