#ifndef IN_GAME_APP_STATE_H
#define IN_GAME_APP_STATE_H

#include"abstractAppState.h"
#include"playerData.h"

namespace fsim{
	class Map;

	class InGameAppState{
		public:
			InGameAppState(int);
			~InGameAppState();
			void onAttached();
			void onDettached();
			void update();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
		private:
			Map *map;
			Faction faction;
	};
}

#endif
