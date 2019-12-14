#ifndef ACTIVE_GAME_APP_STATE_H
#define ACTIVE_GAME_APP_STATE_H

#include"abstractAppState.h"
#include<vector.h>

namespace vb01{
	class Node;
}

namespace fsim{
	class Structure;
	class GameManager;
	class Aircraft;

	class ActiveGameAppState : public AbstractAppState{
		public:
			ActiveGameAppState(GameManager*,int);
			virtual ~ActiveGameAppState();
			virtual void onAttached();
			virtual void onDettached();
			virtual void update();
			virtual void onAction(Mapping::Bind,bool){}
			inline Aircraft* getAircraft(){return aircraft;}
			void addStructureIcon(int);
		private:
			Aircraft *aircraft;
		protected:
			bool forwPitch=false,backPitch=false,leftRoll=false,rightRoll=false;
			float accIncrement=.01,deccIncrement=.01,yawIncrement=.01,rollIncrement=.01,pitchIncrement=.001,minimapRadius=200,lineOfSight=100,iconSize=20;
			int playerId;
			vb01::Vector3 minimapPos=vb01::Vector3(100,400,.5);
			vb01::Node *guiNode,*minimapNode;
			std::vector<vb01::Node*> structureIconNodes;
	};
}

#endif
