#include"playButton.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"textbox.h"
#include"defConfigs.h"
#include"inGameAppState.h"
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
								class UpgradeButton : public Button{
									public:
										UpgradeButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"",true,PATH+"Icons/upgrade.jpg"){}
										void onClick(){
								
										}
									private:
								};

								GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
								Node *guiNode=gm->getRoot()->getGuiNode();
								for(int i=0;i<5;i++){
									Text *upgradeText=new Text(PATH+"Fonts/batang.ttf",upgrades[i]);
									upgradeText->setScale(.2);
									Node *textNode=new Node(Vector3(50,300+i*50,.5));
									textNode->addText(upgradeText);
									guiNode->attachChild(textNode);

									for(int j=0;j<4;j++)
										guiState->addButton(new UpgradeButton(gm,Vector2(50+j*50,300+i*50),Vector2(40,40)));
								}
							}
						private:
							string upgrades[5];
					};
					class StartButton : public Button{
						public:
							StartButton(GameManager *gm,Vector2 pos, Vector2 size,int faction) : Button(gm,pos,size,"Start"){this->faction=faction;}
							void onClick(){
								GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);

								InGameAppState *inGameState=new InGameAppState(faction);	

								Button *exceptions[]={this};
								guiState->removeAllButtons(exceptions);
								guiState->removeButton(this);
							}
						private:
							int faction;
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
					for(int i=0;i<3;i++)
						guiState->addButton(new AircraftTabButton(gm,Vector2(100+i*(width+10),300),Vector2(width,20),tabNames[i],upgrades[i]));
					guiState->getButton("Fighter")->onClick();
					guiState->addButton(new StartButton(gm,Vector2(500,500),Vector2(100,50),faction));

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
