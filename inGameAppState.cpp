#define MYSQLPP_MYSQL_HEADERS_BURIED

#include<mysql++.h>
#include"aircraftSelectionButton.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helicopterAppState.h"
#include"helicopter.h"
#include"jet.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"map.h"
#include"aircraft.h"
#include"defConfigs.h"
#include"unit.h"
#include"okButton.h"
#include"exitButton.h"
#include"optionsButton.h"
#include"mainMenuButton.h"
#include"textbox.h"
#include"projectile.h"
#include"runway.h"
#include"helipad.h"
#include"aircraftCarrier.h"
#include"abstractRunway.h"
#include"abstractHelipad.h"
#include"structureData.h"
#include<text.h>
#include<root.h>
#include<quad.h>
#include<node.h>
#include<material.h>
#include<particleEmitter.h>
#include<algorithm>
#include<SFML/Audio.hpp>

using namespace std;
using namespace mysqlpp;
using namespace vb01;

namespace fsim{
	InGameAppState::RestartButton::RestartButton(GameManager *gm,Vector2 pos,Vector2 size):Button(gm,pos,size,"Restart",Mapping::RESTART){}

	void InGameAppState::RestartButton::onClick(){
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		inGameState->restart();
	}

	InGameAppState::InGameAppState(GameManager *gm,int pilotId,int playerId,int faction,int saveId,int level,int objective) : AbstractAppState(gm){
		type=AbstractAppState::IN_GAME_STATE;
		Node *guiNode=gm->getRoot()->getGuiNode();
		this->faction=(Faction)faction;
		this->pilotId=pilotId;

		string levelSuffix,levelNumSuffix=(level<10?"0"+to_string(level):to_string(level));
		switch(this->faction){
			case CHINA:
				 levelSuffix="Ch/Level"+levelNumSuffix+"/";
				break;
			case JAPAN:
				levelSuffix="Jp/Level"+levelNumSuffix+"/";
				break;
			case KOREA:
				levelSuffix="Kr/Level"+levelNumSuffix+"/";
				break;
		}
		map=new Map(gm,PATH+"Models/Levels/"+levelSuffix,this,saveId,level,objective,pilotId);
		guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		this->playerId=(playerId==-1?structures.size():playerId);
		
		//gm->getStateManager()->dettachState(guiState);
		
		Quad *quad=new Quad(Vector3(gm->getWidth(),gm->getHeight(),0),false);
		pauseOverlay=new Node(Vector3(0,0,.3));
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->setTexturingEnabled(false);
		mat->setDiffuseColor(Vector4(0,0,0,.5));
		quad->setMaterial(mat);
		pauseOverlay->attachMesh(quad);
		pauseOverlay->setVisible(false);
		guiNode->attachChild(pauseOverlay);

		Text *levelEnding=new Text(PATH+"Fonts/batang.ttf","");
		levelEnding->setScale(.3);
		levelEndingNode=new Node(Vector3(300,300,-.95));
		levelEndingNode->addText(levelEnding);
		guiNode->attachChild(levelEndingNode);
	}

	InGameAppState::~InGameAppState(){
		Node *guiNode=gm->getRoot()->getGuiNode();
		clearMap();
		guiNode->dettachChild(levelEndingNode);
		guiNode->dettachChild(pauseOverlay);
		delete levelEndingNode;
		delete pauseOverlay;
	}

	void InGameAppState::onAttached(){
		AbstractAppState::onAttached();
	}

	void InGameAppState::onDettached(){
		AbstractAppState::onDettached();
	}

	void InGameAppState::update(){
		if(!activeState){
			StateManager *stateManager=gm->getStateManager();
			JetAppState *jetState=(JetAppState*)stateManager->getState(AbstractAppState::JET_STATE);
			HelicopterAppState *helicopterState=(HelicopterAppState*)stateManager->getState(AbstractAppState::HELICOPTER_STATE);
			activeState=(jetState?(ActiveGameAppState*)jetState:(ActiveGameAppState*)helicopterState);
		}
		if(!paused){
			if(levelStatus==ONGOING)
				map->update();
			for(int i=0;i<structures.size();i++){
				if(structures[i]->getHp()>0)
					structures[i]->update();
				else{
					const int numFrames=14;
					string paths[numFrames];
					for(int j=0;j<numFrames;j++){
						string numString=j<10?"0"+to_string(j):to_string(j);
						paths[j]=PATH+"Textures/Explosion/explosion"+numString+".png";
					}

					ParticleEmitter *explosion=new ParticleEmitter(1);
					Node *explosionNode=new Node(structures[i]->getPos());
					explosionNode->attachParticleEmitter(explosion);
					Material *mat=new Material(Material::MATERIAL_PARTICLE);
					mat->addDiffuseMap(new Texture(paths,numFrames,30));
					explosion->setStartSize(Vector2(20,20));
					explosion->setEndSize(Vector2(20,20));
					explosion->setStartColor(Vector4(1,1,1,1));
					explosion->setEndColor(Vector4(1,.5,0,1));
					explosion->setDirection(Vector3(0.,.0,0.));
					explosion->setMaterial(mat);
					explosion->setSpread(1);
					explosion->setLowLife(.5);
					explosion->setHighLife(.5);
					gm->getRoot()->getRootNode()->attachChild(explosionNode);

					int explosionSfx=rand()%4;
					sf::SoundBuffer *sfxBuffer=new sf::SoundBuffer();
					sfxBuffer->loadFromFile(PATH+"Sounds/Explosions/explosion0"+to_string(explosionSfx)+".ogg");
					sf::Sound *sfx=new sf::Sound();
					sfx->setBuffer(*sfxBuffer);
					sfx->setMinDistance(5);
					sfx->setAttenuation(5);
					sfx->play();

					Fx fx;
					fx.emitters=new ParticleEmitter*;
					fx.emitters[0]=explosion;
					fx.pos=structures[i]->getPos();
					fx.timeToLive=1500;
					fx.initTime=getTime();
					fx.sfx=sfx;
					addFx(fx);

					delete structures[i];
					structures.erase(structures.begin()+i);
					if(i<playerId){
						playerId--;
						activeState->setPlayerId(playerId);
					}
					else if(i==playerId){
						activeState->setAircraft(nullptr);
						gm->getStateManager()->dettachState(activeState);
						endLevel(false);
					}
				}
			}
			for(int i=0;i<projectiles.size();i++){
				if(!projectiles[i]->isExploded())
					projectiles[i]->update();
				else{
					delete projectiles[i];
					projectiles.erase(projectiles.begin()+i);
				}
			}
			for(int i=0;i<fx.size();i++)
				if(getTime()-fx[i].initTime>fx[i].timeToLive&&fx[i].allive){
					Node *node=fx[i].emitters[0]->getNode();
					Root::getSingleton()->getRootNode()->dettachChild(node);
					if(fx[i].sfx){
						fx[i].sfx->stop();
						delete fx[i].sfx->getBuffer();
						delete fx[i].sfx;
					}
					//delete[] fx[lastId].emitters;
					fx[i].allive=false;
					//delete node;
					fx.erase(fx.begin()+i);
				}
			if(levelStatus!=ONGOING&&getTime()-levelEndTime>4000){
				guiState->addButton(new RestartButton(gm,Vector2(100,280),Vector2(100,50)));
				if(levelStatus==VICTORY){
					class ContinueButton : public OkButton{
						public:
							ContinueButton(GameManager *gm, Vector2 pos,Vector2 size,int pilotId) : OkButton(gm,pos,size,nullptr,-1,pilotId){}
							void onClick(){
								StateManager *stateManager=gm->getStateManager();
								InGameAppState *inGameState=(InGameAppState*)stateManager->getState(AbstractAppState::IN_GAME_STATE);
								stateManager->dettachState(inGameState);
								delete inGameState;
								OkButton::onClick();
							}
						private:
					};
					Connection conn(false);
					conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
					int level=map->getLevel()+1;
					if(level<maxNumLevels)
						conn.query("update pilots set level="+to_string(level)+" where pid="+to_string(pilotId)+";").store();

					ContinueButton *continueButton=new ContinueButton(gm,Vector2(100,340),Vector2(100,50),pilotId);
					continueButton->setFaction(structures[playerId]->getFaction());
					guiState->addButton(continueButton);
				}
				pauseOverlay->setVisible(true);
				paused=true;
			}
		}
	}

	void InGameAppState::endLevel(bool win){
		levelStatus=win?VICTORY:DEFEAT;
		((Text*)levelEndingNode->getText(0))->setText(win?"Mission accomplished!":"Mission failed");
		levelEndingNode->setVisible(true);
		levelEndTime=getTime();
	}

	void InGameAppState::restart(){
		Node *guiNode=gm->getRoot()->getGuiNode();
		string mapPath=map->getPath(),aircraftTypes[]={"Fighter","Fighter-bomber","Helicopter"};
		int level=map->getLevel(),objective=map->getObjective(),width=60;

		pauseOverlay->setVisible(false);
		paused=false;
		levelStatus=ONGOING;
		levelEndingNode->setVisible(false);

		clearMap();
		map=new Map(gm,mapPath,this,-1,level,objective,pilotId);
		playerId=structures.size();
		guiState->removeAllButtons(nullptr);

		StateManager *stateManager=gm->getStateManager();
		Runway *runway=map->getHomeRunway();
		Helipad *helipad=map->getHomeHelipad();
		AircraftCarrier *carrier=map->getHomeCarrier();
		AircraftSelectionButton *selectionButtons[3];
		string aircraft[]={"Fighter","Fighter-bomber","Helicopter"};
		if(carrier){
			Vector3 pos[]{
				carrier->getAbstractRunway()->getStartPos(),
				carrier->getAbstractRunway()->getStartPos(),
				carrier->getAbstractHelipad()->getStartPos()+Vector3(0,50,0)
			};
			Quaternion rot[]{
				carrier->getAbstractRunway()->getStartRot(),
				carrier->getAbstractRunway()->getStartRot(),
				carrier->getAbstractHelipad()->getStartRot()
			};
			for(int i=0;i<3;i++){
				selectionButtons[i]=new AircraftSelectionButton(gm,Vector2(100+(width+10)*i,100),Vector2(width,100),
						aircraft[i],faction*3+i,faction,Mapping::Bind(Mapping::FIGHTER+i),
						pos[i],rot[i]);
				guiState->addButton(selectionButtons[i]);
			}
		}
		else{
			if(runway){
				Vector3 runwayPos=runway->getPos();
				Quaternion runwayRot=runway->getRot();
				for(int i=0;i<2;i++){
					selectionButtons[i]=new AircraftSelectionButton(gm,Vector2(100+(width+10)*i,100),Vector2(width,100),
							aircraft[i],faction*3+i,faction,Mapping::Bind(Mapping::FIGHTER+i),
							runwayPos,runwayRot);
					guiState->addButton(selectionButtons[i]);
				}
			}
			if(helipad){
				Vector3 helipadPos=helipad->getPos()+Vector3(0,50,0);
				Quaternion helipadRot=helipad->getRot();
				selectionButtons[2]=new AircraftSelectionButton(gm,Vector2(100+(width+10)*2,100),Vector2(width,100),
						aircraft[2],faction*3+2,faction,Mapping::Bind(Mapping::FIGHTER+2),
						helipadPos,helipadRot);
				guiState->addButton(selectionButtons[2]);
			}
			if(!runway&&!helipad){
				int aircraftId=map->getAircraftId();
				Vector3 spawnPos=map->getSpawnPos();
				Quaternion spawnRot=map->getSpawnRot();
				bool helicopter=(aircraftId==structureData::CHINESE_HELICOPTER||aircraftId==structureData::JAPANESE_HELICOPTER||aircraftId==structureData::KOREAN_HELICOPTER);
				Aircraft *aircraft=helicopter?(Aircraft*)new Helicopter(gm,aircraftId,faction,spawnPos,spawnRot,false):
				(Aircraft*)new Jet(gm,aircraftId,faction,spawnPos,spawnRot,false);
				structures.push_back(aircraft);
				selectingAircraft=false;
				ActiveGameAppState *activeState=helicopter?(ActiveGameAppState*)new HelicopterAppState(gm,playerId):(ActiveGameAppState*)new JetAppState(gm,playerId);
				stateManager->attachState((AbstractAppState*)activeState);
				this->activeState=activeState;
			}
		}
	}

	void InGameAppState::save(string name){
		Connection conn(false);
		conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
		int level=map->getLevel(),objectiveId=map->getObjective();
		int numSaves=(int)conn.query("select count(*) from saves;").store()[0][0];
		conn.query("insert into saves values("+to_string(numSaves)+",'"+name+"',"+to_string(pilotId)+","+to_string(playerId)+","+to_string(level)+","+to_string(objectiveId)+");").store();
		for(int i=0;i<structures.size();i++){
			Structure *s=structures[i];
			int id=s->getId(),faction=s->getFaction();
			Vector3 pos=s->getPos();
			Quaternion rot=s->getRot();
			string values=to_string(numSaves)+","+to_string(id)+","+to_string(faction)+","+to_string(pos.x)+","+to_string(pos.y)+","+to_string(pos.z)+","+to_string(rot.w)+","+to_string(rot.x)+","+to_string(rot.y)+","+to_string(rot.z);
			conn.query("insert into save_units values("+values+");").store();
		}
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
								inGameState->save(textbox->getText());
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
		pauseOverlay->setVisible(!paused);
		if(!paused){
			stateManager->dettachState(activeState);
			guiState->addButton(new ResumeButton(gm,Vector2(100,100),Vector2(100,50)));
			guiState->addButton(new SaveButton(gm,Vector2(100,160),Vector2(100,50)));
			guiState->addButton(new OptionsButton(gm,Vector2(100,220),Vector2(100,50)));
			guiState->addButton(new RestartButton(gm,Vector2(100,280),Vector2(100,50)));
			guiState->addButton(new MainMenuButton(gm,Vector2(100,340),Vector2(100,50)));
			guiState->addButton(new ExitButton(gm,Vector2(100,400),Vector2(100,50)));
		}
		else{
			guiState->removeAllGUIElements();
			stateManager->attachState(activeState);
		}
		paused=!paused;
	}

	void InGameAppState::clearMap(){
		gm->getStateManager()->dettachState(activeState);
		delete activeState;
		activeState=nullptr;
		while(!structures.empty()){
			delete structures[0];
			structures.erase(structures.begin());
		}
		while(!projectiles.empty()){
			delete projectiles[0];
			projectiles.erase(projectiles.begin());
		}
		while(!fx.empty())
			fx.pop_back();
		delete map;
	}

	void InGameAppState::onAction(Mapping::Bind bind, bool isPressed){
		switch(bind){
			case Mapping::MAIN_MENU:
				if(isPressed&&!selectingAircraft)togglePause();
				break;	
		}
	}

	void InGameAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
		}
	}

	void InGameAppState::addStructure(Structure *s){
		if(activeState)
			activeState->addStructureIcon(s->getId());
		structures.push_back(s);
	}
}
