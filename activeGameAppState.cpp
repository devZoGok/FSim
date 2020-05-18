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

		minimapPos=Vector3(0,gm->getHeight()-minimapRadius*2,-.8);
		Quad *quad=new Quad(Vector3(minimapRadius,minimapRadius,0)*2,false);
		minimapNode=new Node(minimapPos);
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->addDiffuseMap(PATH+"Icons/Minimap/minimap.png");
		mat->setLightingEnabled(false);
		quad->setMaterial(mat);
		minimapNode->attachMesh(quad);

		Quad *dangerQuad=new Quad(Vector3(50,50,0),false);
		dangerNode=new Node(Vector3(700,400,0));
		Material *dangerMat=new Material(Material::MATERIAL_GUI);
		dangerMat->addDiffuseMap(PATH+"Icons/HUD/danger.png");
		dangerMat->setLightingEnabled(false);
		dangerQuad->setMaterial(dangerMat);
		dangerNode->attachMesh(dangerQuad);

		string icons[]{
			"hp.png",
			"bullet.png",
			aircraft->getType()==AircraftType::FIGHTER_BOMBER?"bomb.png":"missile.png",
			"fuel.png",
			"chaff.png"
		};
		hudText=new Node*[numHudElements];
		hudIcons=new Node*[numHudElements];
		Vector3 startPos=Vector3(550,500,.0);
		float offset=50,iconSize=50;
		for(int i=0;i<numHudElements;i++){
			Quad *iconQuad=new Quad(Vector3(iconSize,iconSize,0),false);
			Node *iconNode=new Node(startPos+Vector3(offset*i,0,0));
			Material *mat=new Material(Material::MATERIAL_GUI);
			mat->addDiffuseMap(PATH+"Icons/HUD/"+icons[i],true);
			mat->setLightingEnabled(false);
			mat->setDiffuseColorEnabled(true);
			mat->setDiffuseColor(Vector4(0,0,0,1));
			iconQuad->setMaterial(mat);
			iconNode->attachMesh(iconQuad);
			hudIcons[i]=iconNode;

			Text *text=new Text(PATH+"Fonts/batang.ttf","100");
			text->setScale(.3);
			text->setColor(Vector4(0,0,0,1));
			Node *textNode=new Node(startPos+Vector3(offset*i,-20,0));
			textNode->addText(text);
			hudText[i]=textNode;
			/*
			*/
		}

		alarmSfxBuffer=new sf::SoundBuffer();
		alarmSfxBuffer->loadFromFile(PATH+"Sounds/alarm.ogg");
		alarmSfx=new sf::Sound();
		alarmSfx->setBuffer(*alarmSfxBuffer);
		alarmSfx->setRelativeToListener(true);
		alarmSfx->setVolume(gm->getOptions().sfxVolume);

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
		for(int i=0;i<numHudElements;i++){
			delete hudText[i];
			delete hudIcons[i];
		}
		delete[] hudText;
		delete[] hudIcons;
		for(Node *s : structureIconNodes)
			delete s;
		delete minimapNode;
		delete dangerNode;
	}

	void ActiveGameAppState::onAttached(){
		AbstractAppState::onAttached();
		guiNode->attachChild(minimapNode);
		guiNode->attachChild(dangerNode);
		for(int i=0;i<numHudElements;i++){
			guiNode->attachChild(hudIcons[i]);
			guiNode->attachChild(hudText[i]);
		}
		for(Node *i : structureIconNodes)
			if(!i->getParent())
				guiNode->attachChild(i);
	}

	void ActiveGameAppState::onDettached(){
		AbstractAppState::onDettached();
		guiNode->dettachChild(minimapNode);
		guiNode->dettachChild(dangerNode);
		for(int i=0;i<numHudElements;i++){
			guiNode->dettachChild(hudIcons[i]);
			guiNode->dettachChild(hudText[i]);
		}
		for(Node *i : structureIconNodes)
			guiNode->dettachChild(i);
	}

	void ActiveGameAppState::update(){
		if(!inGameState)
			inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		hudText[0]->getText(0)->setText(to_string(aircraft->getHp()));
		hudText[1]->getText(0)->setText(to_string(aircraft->getPrimaryAmmo()));
		hudText[2]->getText(0)->setText(to_string(aircraft->getSecondaryAmmo()));
		hudText[3]->getText(0)->setText(to_string(aircraft->getFuel()));
		hudText[4]->getText(0)->setText(to_string(aircraft->getChaff()));

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
				structureIconNodes[i]->setPosition(
							Vector3(cos(angle+PI/2),-sin(angle+PI/2),0)*(dist/lineOfSight*minimapRadius)+Vector3(minimapPos.x+minimapRadius-iconSize/2,minimapPos.y+minimapRadius-iconSize/2,-(.8+i*.01))
						);
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

