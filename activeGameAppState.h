#ifndef ACTIVE_GAME_APP_STATE_H
#define ACTIVE_GAME_APP_STATE_H

#include"abstractAppState.h"

namespace fsim{
	class Structure;
	class GameManager;
	class Aircraft;

	class ActiveGameAppState : public AbstractAppState{
		public:
			ActiveGameAppState(GameManager*,int);
			virtual void onAttached();
			virtual void onDettached();
			virtual void update();
			virtual void onAction(Mapping::Bind,bool){}
		private:
			Aircraft *aircraft;
		protected:
			bool forwPitch=false,backPitch=false,leftRoll=false,rightRoll=false;
			float accIncrement=.01,deccIncrement=.01,yawIncrement=.01,rollIncrement=.01,pitchIncrement=.001;
			int playerId;
			std::vector<Structure*> structures;
	};
}

#endif
