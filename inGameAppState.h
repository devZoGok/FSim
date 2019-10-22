#ifndef IN_GAME_APP_STATE_H
#define IN_GAME_APP_STATE_H

#include"abstractAppState.h"
#include"playerData.h"
#include<vector>

namespace fsim{
	class Structure;
	class Unit;
	class Map;
	class GameManager;

	class InGameAppState : public AbstractAppState{
		public:
			InGameAppState(GameManager*,int);
			~InGameAppState();
			void onAttached();
			void onDettached();
			void update();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
			inline int getNumStructures(){return structures.size();}
			inline void addStructure(Structure *s){structures.push_back(s);}
			inline void setPlayerId(int id){this->playerId=id;}
		private:
			int playerId;
			GameManager *gm;
			Map *map;
			Faction faction;
			std::vector<Structure*> structures;
	};
}

#endif
