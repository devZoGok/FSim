#include"jetAppState.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"map.h"
#include"jet.h"
#include<glfw3.h>
#include<ray.h>
#include<vector>
#include<root.h>
#include<iostream>

using namespace vb01;
using namespace std;

namespace fsim{
	JetAppState::JetAppState(GameManager *gm, int playerId) : ActiveGameAppState(gm,playerId){
		type=AbstractAppState::JET_STATE;
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		playerJet=(Jet*)getAircraft();
	}
	
	JetAppState::~JetAppState(){}

	void JetAppState::onAttached(){
		ActiveGameAppState::onAttached();
		const int numMappings=4;
		Mapping::Bind binds[numMappings]{
			Mapping::PITCH_UP,
			Mapping::PITCH_DOWN,
			Mapping::YAW_LEFT,
			Mapping::YAW_RIGHT
		};
		int triggers[numMappings]{
			GLFW_KEY_W,
			GLFW_KEY_S,
			GLFW_KEY_A,
			GLFW_KEY_D
		};
		/*
		for(int i=0;i<numMappings;i++){
			Mapping *m=new Mapping;
			m->bind=binds[i];
			m->trigger=triggers[i];
			m->action=1;
			m->type=Mapping::KEYBOARD;
			mappings.push_back(m);
		}
		*/
	}

	void JetAppState::onDettached(){
		ActiveGameAppState::onDettached();
	}

	void JetAppState::update(){
		ActiveGameAppState::update();
		if(forwPitch)
			playerJet->pitch(playerJet->getPitchSpeed());
		if(backPitch)
			playerJet->pitch(-playerJet->getPitchSpeed());
		if(leftRoll)
			playerJet->yaw(playerJet->getYawSpeed());
		if(rightRoll)
			playerJet->yaw(-playerJet->getYawSpeed());
	}

	void JetAppState::onAction(Mapping::Bind bind, bool isPressed){
		switch(bind){
			case Mapping::PITCH_UP:
				forwPitch=isPressed;
				break;
			case Mapping::PITCH_DOWN:
				backPitch=isPressed;
				break;
			case Mapping::YAW_LEFT:
				leftRoll=isPressed;
				break;
			case Mapping::YAW_RIGHT:
				rightRoll=isPressed;
				break;
			case Mapping::PRIMARY_FIRE:
				playerJet->setPrimaryFiring(isPressed);
				break;
			case Mapping::SECONDARY_FIRE:
				if(isPressed)playerJet->secondaryFire();
				break;
			case Mapping::DEPLOY_CHAFF:
				if(isPressed)playerJet->deployChaff();
				break;
		}
	}

	void JetAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
			case Mapping::PITCH_UP:
			case Mapping::PITCH_DOWN:
				playerJet->pitch(-playerJet->getPitchSpeed()*str);
				break;
			case Mapping::YAW_LEFT:
			case Mapping::YAW_RIGHT:
				playerJet->setYaw(-playerJet->getYawSpeed()*str);
				break;
			case Mapping::ROLL_LEFT:
			case Mapping::ROLL_RIGHT:
				playerJet->setRoll(-playerJet->getRollSpeed()*str);
				break;
		}
		/*
		vector<CollisionResult> results;
		Node *rootNode=gm->getRoot()->getRootNode();
		Model *m=((InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE))->getMap()->getMapModel();
		retrieveCollisions(Vector3(0,40,0),Vector3(0,-1,0),(Node*)playerJet->getModel(),results,100);
		cout<<results.size()<<"\n";		
		*/
	}
}
