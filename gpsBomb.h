#ifndef GPS_BOMB_H
#define GPS_BOMB_H

#include"bomb.h"

namespace fsim{
	class GPSBomb : public Bomb{
		public:
			GPSBomb(GameManager*,Structure*,vb01::Vector3,vb01::Vector3,vb01::Quaternion,float);
			~GPSBomb();
			void update();
		private:
			float maxDist;
	};
}

#endif
