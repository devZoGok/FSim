#include"helicopterAppState.h"
#include"helicopter.h"

using namespace vb01;

namespace fsim{
	HelicopterAppState::HelicopterAppState(GameManager *gm, int playerId) : ActiveGameAppState(gm,playerId){
		type=AbstractAppState::HELICOPTER_STATE;	
		helicopter=(Helicopter*)structures[playerId];
	}

	HelicopterAppState::~HelicopterAppState(){}

	void HelicopterAppState::onAttached(){
		ActiveGameAppState::onAttached();
	}

	void HelicopterAppState::onDettached(){
		ActiveGameAppState::onDettached();
	}

	void HelicopterAppState::update(){
		ActiveGameAppState::update();
		if(forwPitch)
			helicopter->setPitch(-helicopter->getPitchSpeed());
		if(backPitch)
			helicopter->setPitch(helicopter->getPitchSpeed());
		if(leftRoll)
			helicopter->setRoll(-helicopter->getRollSpeed());
		if(rightRoll)
			helicopter->setRoll(helicopter->getRollSpeed());
		if(leftYaw)
			helicopter->setYaw(helicopter->getYawSpeed());
		if(rightYaw)
			helicopter->setYaw(-helicopter->getYawSpeed());
		if(rising)
			helicopter->setRising(helicopter->getRising()+riseIncrement*(rising?1:-1));
		if(landing)
			helicopter->setLanding(helicopter->getLanding()+landIncrement*(landing?1:-1));
		/*
		*/
	}

	void HelicopterAppState::onAction(Mapping::Bind bind, bool isPressed){
		switch(bind){
			case Mapping::PITCH_HELICOPTER_UP:
				forwPitch=isPressed;
				break;
			case Mapping::PITCH_HELICOPTER_DOWN:
				backPitch=isPressed;
				break;
			case Mapping::ROLL_HELICOPTER_LEFT:
				leftRoll=isPressed;
				break;
			case Mapping::ROLL_HELICOPTER_RIGHT:
				rightRoll=isPressed;
				break;
			case Mapping::RISE:
				rising=isPressed;
				break;
			case Mapping::LAND:
				landing=isPressed;
				break;
			case Mapping::YAW_HELICOPTER_LEFT:
				leftYaw=isPressed;
				break;
			case Mapping::YAW_HELICOPTER_RIGHT:
				rightYaw=isPressed;
				break;
		}
	}

	void HelicopterAppState::onAnalog(Mapping::Bind bind, float str){
		switch(bind){
			case Mapping::PITCH_HELICOPTER_UP:
			case Mapping::PITCH_HELICOPTER_DOWN:
				helicopter->setPitch(helicopter->getPitchSpeed()*str);
				break;
			case Mapping::ROLL_HELICOPTER_LEFT:
			case Mapping::ROLL_HELICOPTER_RIGHT:
				helicopter->setRoll(helicopter->getRollSpeed()*str);
				break;
			case Mapping::RISE:
				helicopter->setRising(helicopter->getRisingSpeed()*(-str));
				break;
			case Mapping::LAND:
				helicopter->setLanding(helicopter->getLandingSpeed()*(str));
				break;
			case Mapping::YAW_HELICOPTER_LEFT:
			case Mapping::YAW_HELICOPTER_RIGHT:
				helicopter->setYaw(helicopter->getYawSpeed()*str);
				break;
		}
	}
}