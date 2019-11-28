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
	class InGameAppState;

	class Map{
		public:
			Map(GameManager*,std::string,InGameAppState*,bool loadFromSave=false,int=0,int=0,int=-1);
			~Map();
			void update();
			inline int getObjective(){return objective;}
			inline int getLevel(){return level;}
		private:
			void getCoords(std::string,std::string&,float*,const int);

			int objective,level;
			GameManager *gm;
			vb01::Model* mapModel;
			vb01::Node *rootNode;
			std::vector<vb01::Light*> lights;
	};
}

#endif
