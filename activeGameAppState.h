#ifndef ACTIVE_GAME_APP_STATE_H
#define ACTIVE_GAME_APP_STATE_H

#include"abstractAppState.h"
#include<vector.h>
#include<util.h>

namespace vb01{
	class Node;
}

namespace sf{
	class SoundBuffer;
	class Sound;
}

namespace fsim{
	class Structure;
	class GameManager;
	class Aircraft;
	class InGameAppState;

	class ActiveGameAppState : public AbstractAppState{
		public:
			ActiveGameAppState(GameManager*,int);
			virtual ~ActiveGameAppState();
			virtual void onAttached();
			virtual void onDettached();
			virtual void update();
			virtual void onAction(Mapping::Bind,bool){}
			inline Aircraft* getAircraft(){return aircraft;}
			inline void setAircraft(Aircraft *aircraft){this->aircraft=aircraft;}
			inline void setPlayerId(int playerId){this->playerId=playerId;}
			void addStructureIcon(int);
		private:
			Aircraft *aircraft;
			InGameAppState *inGameState=nullptr;
			sf::SoundBuffer *alarmSfxBuffer;
			sf::Sound *alarmSfx;
			vb01::s64 lastAlarmTime=0,rateOfAlarm=100;
			vb01::Node *dangerNode;
		protected:
			int playerId;
			bool forwPitch=false,backPitch=false,leftRoll=false,rightRoll=false,leftYaw=false,rightYaw=false;
			float accIncrement=.01,deccIncrement=.01,yawIncrement=.01,rollIncrement=.01,pitchIncrement=.001,minimapRadius=200,lineOfSight=100,iconSize=20;
			vb01::Vector3 minimapPos=vb01::Vector3(100,400,-.8);
			vb01::Node *guiNode,*minimapNode,*ammoTextNode;
			std::vector<Structure*> targets;
			std::vector<vb01::Node*> structureIconNodes,hitmarkerNodes;
	};
}

#endif
