#ifndef MAP_H
#define MAP_H

#include<string>
#include<vector>

namespace vb01{
	class Model;
}

namespace fsim{
	class Map{
		public:
			Map(std::string);
			~Map();
			void update();
		private:
			std::vector<vb01::Model*> models;
	};
}

#endif
