#ifndef JET_APP_STATE_H 
#define JET_APP_STATE_H

#include"activeGameAppState.h"

namespace fsim{
	class Jet;

	class JetAppState : public ActiveGameAppState{
		public:
			JetAppState(GameManager*,int);
			~JetAppState();
			void onAttached();
			void onDettached();
			void update();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
		private:
			Jet *playerJet;
	};
}

#endif
