#include"jetAppState.h"
#include"jet.h"
#include<glfw3.h>

namespace fsim{
	JetAppState::JetAppState(GameManager *gm, int playerId) : ActiveGameAppState(gm,playerId){
		type=AbstractAppState::JET_STATE;
		playerJet=(Jet*)structures[playerId];
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
		}
		/*
		*/
	}

	void JetAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
			case Mapping::PITCH_UP:
			case Mapping::PITCH_DOWN:
				playerJet->pitch(playerJet->getPitchSpeed()*str);
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
	}
}
