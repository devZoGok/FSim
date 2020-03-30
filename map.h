#ifndef MAP_H
#define MAP_H

#include<string>
#include<vector>
#include<vector.h>
#include<quaternion.h>
#include"util.h"

namespace vb01{
	class Model;
	class Node;
	class Light;
}

namespace fsim{
	class GameManager;
	class InGameAppState;
	class Structure;

	class Map{
		public:
			Map(GameManager*,std::string,InGameAppState*,int=-1,int=0,int=0,int=-1);
			~Map();
			void update();
			inline int getObjective(){return objective;}
			inline int getLevel(){return level;}
			inline vb01::Model* getMapModel(){return mapModel;}
		private:
			struct Objective{
				enum Status{SUCCESS,FAILURE,PENDING};
				enum Type{DESTROY,MOVE_TO,WAIT};
				struct Condition{
					Type type;
					std::vector<Structure*> targets;
					s64 initTime,time;
					vb01::Vector3 pos;
				};
				Condition *success=nullptr,*failure=nullptr;
				Status status=PENDING;
				std::string text;
			};
			template<typename T>
			void getLineData(std::string,T*,int,int=0);
			void createStructure(InGameAppState*,int,int,vb01::Vector3,vb01::Quaternion,int*,bool);
			void createObjective(InGameAppState*,Objective::Condition*,std::string,int);

			int objective,level;
			GameManager *gm;
			vb01::Model* mapModel;
			vb01::Node *rootNode;
			std::vector<vb01::Light*> lights;
			std::vector<Objective> objectives;

			void foo(int);
	};
}

#endif
