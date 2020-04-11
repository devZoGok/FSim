#ifndef HELICOPTER_APP_STATE_H
#define HELICOPTER_APP_STATE_H

#include"activeGameAppState.h"

namespace fsim{
	class Helicopter;

	class HelicopterAppState : public ActiveGameAppState{
		public:
			HelicopterAppState(GameManager*,int);
			~HelicopterAppState();
			void onAttached();
			void onDettached();
			void update();
			void onAction(Mapping::Bind,bool);
			void onAnalog(Mapping::Bind,float);
		private:
			Helicopter *helicopter;
			bool leftYaw=0,rightYaw=0,rising=0,landing=0;
			float riseIncrement=.01;
	};
}

#endif
