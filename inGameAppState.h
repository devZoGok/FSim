#ifndef IN_GAME_APP_STATE_H
#define IN_GAME_APP_STATE_H

#include"abstractAppState.h"
#include"playerData.h"
#include"button.h"
#include"fx.h"
#include<util.h>
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
			void endLevel(bool);
			void restart();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
			inline int getNumStructures(){return structures.size();}
			inline int getPilotId(){return pilotId;}
			inline int getPlayerId(){return playerId;}
			void addStructure(Structure*);
			inline void addProjectile(Projectile *p){projectiles.push_back(p);}
			inline void addFx(Fx f){this->fx.push_back(f);}
			inline void setPlayerId(int id){this->playerId=id;}
			inline void setSelectingAircraft(bool selecting){this->selectingAircraft=selecting;}
			inline std::vector<Structure*>& getStructures(){return structures;}
			inline std::vector<Projectile*>& getProjectiles(){return projectiles;}
			inline std::vector<Fx>& getFxs(){return fx;}
			inline Structure* getStructure(int i){return structures[i];}
			inline Map* getMap(){return map;}
		private:
			enum LevelStatus{ONGOING,VICTORY,DEFEAT};
			class RestartButton : public Button{
				public:
					RestartButton(GameManager*,vb01::Vector2,vb01::Vector2);
					void onClick();
			};
			void togglePause();
			void clearMap();
			
			vb01::Node *pauseOverlay=nullptr,*levelEndingNode;
			ActiveGameAppState *activeState=nullptr;
			int pilotId,playerId=-1;
			vb01::s64 levelEndTime;
			bool paused=false,selectingAircraft=false;
			LevelStatus levelStatus=ONGOING;
			Map *map;
			Faction faction;
			GuiAppState *guiState;
			std::vector<Structure*> structures;
			std::vector<Projectile*> projectiles;
			std::vector<Fx> fx;
	};
}

#endif
