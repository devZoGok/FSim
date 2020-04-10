#include"activeGameAppState.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"jet.h"
#include"helicopter.h"
#include"structureData.h"
#include<root.h>
#include<quad.h>
#include<node.h>
#include<text.h>
#include<material.h>
#include<cmath>
#include<SFML/Audio.hpp>

using namespace std;
using namespace vb01;

namespace fsim{
	ActiveGameAppState::ActiveGameAppState(GameManager *gm,int playerId) : AbstractAppState(gm){
		this->playerId=playerId;
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		vector<Structure*> &structures=inGameState->getStructures();
		aircraft=(Aircraft*)inGameState->getStructure(playerId);
		guiNode=gm->getRoot()->getGuiNode();
		for(Structure *s : structures)
			addStructureIcon(s->getId());

		Quad *quad=new Quad(Vector3(minimapRadius,minimapRadius,0),false);
		minimapNode=new Node(minimapPos);
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->addDiffuseMap(PATH+"Icons/Minimap/minimap.png");
		mat->setLightingEnabled(false);
		quad->setMaterial(mat);
		minimapNode->attachMesh(quad);

		Quad *dangerQuad=new Quad(Vector3(50,50,0),false);
		dangerNode=new Node(Vector3(700,400,0));
		Material *dangerMat=new Material(Material::MATERIAL_GUI);
		dangerMat->addDiffuseMap(PATH+"Icons/Danger/danger.png");
		dangerMat->setLightingEnabled(false);
		dangerQuad->setMaterial(dangerMat);
		dangerNode->attachMesh(dangerQuad);

		Text *ammoText=new Text(PATH+"Fonts/batang.ttf","");
		ammoText->setScale(.3);
		ammoTextNode=new Node(Vector3(550,500,-.95));
		ammoTextNode->addText(ammoText);

		alarmSfxBuffer=new sf::SoundBuffer();
		alarmSfxBuffer->loadFromFile(PATH+"Sounds/alarm.ogg");
		alarmSfx=new sf::Sound();
		alarmSfx->setBuffer(*alarmSfxBuffer);

		/*
		for(Structure *s : inGameState->getStructures())
			if(s->getId()<=structureData::KOREAN_HELICOPTER)
				targets.push_back(s);
		for(int i=0;i<targets.size();i++){
			Quad *quad=new Quad(Vector3(50,50,0),false);
			Node *node=new Node(Vector3(0,0,0));
			Material *mat=new Material(Material::MATERIAL_GUI);
			mat->addDiffuseMap(PATH+"Icons/Hitmarker/hitmarker.png");
			mat->setLightingEnabled(false);
			quad->setMaterial(mat);
			node->attachMesh(quad);
			guiNode->attachChild(node);
			hitmarkerNodes.push_back(node);
		}
		*/
	}

	ActiveGameAppState::~ActiveGameAppState(){
		vector<Structure*> &structures=inGameState->getStructures();
		if(structures.size()>playerId){
			delete aircraft;
			structures.erase(structures.begin()+playerId);	
		}
		inGameState->setPlayerId(structures.size());
		for(Node *s : structureIconNodes)
			delete s;
		delete minimapNode;
		delete dangerNode;
	}

	void ActiveGameAppState::onAttached(){
		AbstractAppState::onAttached();
		guiNode->attachChild(minimapNode);
		guiNode->attachChild(ammoTextNode);
		guiNode->attachChild(dangerNode);
		for(Node *i : structureIconNodes)
			if(!i->getParent())
				guiNode->attachChild(i);
	}

	void ActiveGameAppState::onDettached(){
		AbstractAppState::onDettached();
		guiNode->dettachChild(minimapNode);
		guiNode->dettachChild(ammoTextNode);
		guiNode->dettachChild(dangerNode);
		for(Node *i : structureIconNodes)
			guiNode->dettachChild(i);
	}

	void ActiveGameAppState::update(){
		if(!inGameState)
			inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		ammoTextNode->getText(0)->setText(to_string(aircraft->getFuel())+"|"+to_string(aircraft->getPrimaryAmmo())+"|"+to_string(aircraft->getSecondaryAmmo())+"|"+to_string(aircraft->getChaff()));

		/*
		Camera *cam=gm->getRoot()->getCamera();
		Vector3 camPos=cam->getPosition(),dir=cam->getDirection(),up=cam->getUp();
		int width=gm->getHeight(),height=gm->getHeight();

		for(int i=0;i<targets.size();i++){
			Vector3 sPos=targets[i]->getPos();
			//vec4 p=vec4(sPos.x,sPos.y,sPos.z,1);
			mat4 model=translate(mat4(1.f),vec3(sPos.x,sPos.y,sPos.z));
			mat4 view=lookAt(vec3(camPos.x,camPos.y,camPos.z),vec3(camPos.x+dir.x,camPos.y+dir.y,camPos.z+dir.z),vec3(up.x,up.y,up.z));
			mat4 proj=ortho(-1000.f,1000.f,-1000.f,1000.f,.1f,100.f);
			//mat4 proj=ortho(float(-width)/2,(float)width/2,float(-height)/2,(float)height/2,.1f,100.f);
			//mat4 proj=perspective(radians(45.f),(float)width/height,.1f,100.f);
			vec4 p=proj*view*model*vec4(1.f);
			hitmarkerNodes[i]->setPosition(Vector3(width*p.x,height*p.y,0));
		}
		*/

		if(aircraft->isLockedOnto()&&getTime()-lastAlarmTime>rateOfAlarm){
			alarmSfx->play();
			dangerNode->setVisible(true);
			lastAlarmTime=getTime();
		}
		else
			dangerNode->setVisible(false);
		vector<Structure*> &structures=inGameState->getStructures();
		for(int i=0;i<structures.size();i++){
			if(i!=playerId){
				Vector3 otherStructPos=structures[i]->getPos(),playerPos=aircraft->getPos(),playerDir=aircraft->getDir(),playerLeft=aircraft->getLeft(),dir=(otherStructPos-playerPos).norm();
				float angle=Vector3(playerDir.x,0,playerDir.z).norm().getAngleBetween(Vector3(dir.x,0,dir.z).norm());
				float dist=Vector3(playerPos.x-otherStructPos.x,0,playerPos.z-otherStructPos.z).getLength();
				bool left=Vector3(playerLeft.x,0,playerLeft.z).norm().getAngleBetween(Vector3(dir.x,0,dir.z).norm())<PI/2;
				bool friendly=structures[i]->getFaction()==aircraft->getFaction();
				angle*=(left?-1:1);
				structureIconNodes[i]->setVisible(i!=playerId&&dist<lineOfSight);
				structureIconNodes[i]->setPosition(Vector3(cos(angle+PI/2),-sin(angle+PI/2),0)*(dist/lineOfSight*minimapRadius)+Vector3(minimapPos.x+minimapRadius/2-iconSize/2,minimapPos.y+minimapRadius/2-iconSize/2,-.8-i*.01));
				structureIconNodes[i]->getMesh(0)->getMaterial()->setDiffuseColor(Vector4(!friendly,friendly,0,1));
			}
		}
	}

	void ActiveGameAppState::addStructureIcon(int id){
		Quad *quad=new Quad(Vector3(iconSize,iconSize,0),false);
		Node *iconNode=new Node(Vector3(0,0,.0));
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->addDiffuseMap(PATH+"Icons/Minimap/"+structureData::iconPath[id]);
		mat->setDiffuseColorEnabled(true);
		quad->setMaterial(mat);
		iconNode->attachMesh(quad);
		guiNode->attachChild(iconNode);
		structureIconNodes.push_back(iconNode);
	}
}
