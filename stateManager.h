#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include<vector>
#include"abstractAppState.h"

namespace fsim{
	class StateManager{
		public:
			StateManager();
			~StateManager();
			void update();
			void attachState(AbstractAppState *a);
			void dettachState(AbstractAppState*);
			void dettachState(AbstractAppState::Type);
			inline void dettachState(int id){appStates.erase(appStates.begin()+id);}
			inline AbstractAppState* getState(int i){return appStates[i];}
			AbstractAppState* getState(AbstractAppState::Type);
			inline std::vector<AbstractAppState*> getAppStates(){return appStates;}
			inline int getNumAppStates(){return appStates.size();}
		private:
			std::vector<AbstractAppState*> appStates;
	};
}

#endif
