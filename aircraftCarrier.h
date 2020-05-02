#ifndef AIRCRAFT_CARRIER_H
#define AIRCRAFT_CARRIER_H

#include"unit.h"

namespace fsim{
	class AbstractRunway;
	class AbstractHelipad;

	class AircraftCarrier : public Unit{
		public:
			AircraftCarrier(GameManager*,int,int,vb01::Vector3,vb01::Quaternion);
			~AircraftCarrier();
			void update();
			inline AbstractRunway* getAbstractRunway(){return runway;}
			inline AbstractHelipad* getAbstractHelipad(){return helipad;}
		private:
			AbstractRunway *runway;
			AbstractHelipad *helipad;
	};
}

#endif
