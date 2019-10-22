#ifndef MAP_H
#define MAP_H

#include<string>
#include<vector>

namespace vb01{
	class Model;
	class Node;
	class Light;
}

namespace fsim{
	class GameManager;

	class Map{
		public:
			Map(GameManager*,std::string);
			~Map();
			void update();
		private:
			GameManager *gm;
			vb01::Model* mapModel;
			vb01::Node *rootNode;
			std::vector<vb01::Light*> lights;
	};
}

#endif
