#ifndef AIRCRAFT_SELECTION_BUTTON_H
#define AIRCRAFT_SELECTION_BUTTON_H

#include"button.h"

namespace fsim{
	class GameManager;

	class AircraftSelectionButton : public Button{
		public:
			AircraftSelectionButton(GameManager*,vb01::Vector2,vb01::Vector2,std::string,int,Mapping::Bind);
			void onClick();
		private:
			int aircraftId;
	};
}

#endif
