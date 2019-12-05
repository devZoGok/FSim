#ifndef IN_GAME_APP_STATE_H
#define IN_GAME_APP_STATE_H

#include"abstractAppState.h"
#include"playerData.h"
#include<vector>

namespace vb01{
	class Quad;
	class Node;
}

namespace fsim{
	class Structure;
	class Projectile;
	class Map;
	class GuiAppState;
	class ActiveGameAppState;
	class GameManager;

	class InGameAppState : public AbstractAppState{
		public:
			InGameAppState(GameManager*,int,int,int,int,int,int);
			~InGameAppState();
			void onAttached();
			void onDettached();
			void update();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
			inline int getNumStructures(){return structures.size();}
			inline int getPilotId(){return pilotId;}
			inline int getPlayerId(){return playerId;}
			inline void addStructure(Structure *s){structures.push_back(s);}
			inline void addProjectile(Projectile *p){projectiles.push_back(p);}
			inline void setPlayerId(int id){this->playerId=id;}
			inline Map* getMap(){return map;}
			std::vector<Structure*>& getStructures(){return structures;}
		private:
			void togglePause();
			
			vb01::Node *pauseOverlay=nullptr;
			ActiveGameAppState *activeState=nullptr;
			int pilotId,playerId;
			bool paused=false;
			Map *map;
			Faction faction;
			GuiAppState *guiState;
			std::vector<Structure*> structures;
			std::vector<Projectile*> projectiles;
	};
}

#endif
