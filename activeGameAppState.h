#ifndef ACTIVE_GAME_APP_STATE_H
#define ACTIVE_GAME_APP_STATE_H

#include"abstractAppState.h"

namespace fsim{
	class ActiveGameAppState : public AbstractAppState{
		public:
			ActiveGameAppState(int);
			void onAttached();
			void onDettached();
			void update();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
		private:
			int playerId;
	};
}

#endif
