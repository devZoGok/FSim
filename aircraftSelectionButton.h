#ifndef AIRCRAFT_SELECTION_BUTTON_H
#define AIRCRAFT_SELECTION_BUTTON_H

#include"button.h"
#include<quaternion.h>

namespace fsim{
	class GameManager;

	class AircraftSelectionButton : public Button{
		public:
			AircraftSelectionButton(GameManager*,vb01::Vector2,vb01::Vector2,std::string,int,int,Mapping::Bind,vb01::Vector3,vb01::Quaternion);
			void onClick();
		private:
			int aircraftId,faction;
			vb01::Vector3 aircraftPos;
			vb01::Quaternion aircraftRot;
	};
}

#endif
