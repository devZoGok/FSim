#ifndef STRUCTURE_H
#define STRUCTURE_H

#include<vector.h>
#include<quaternion.h>

namespace vb01{
	class Model;
	class Node;
	class Material;
}

namespace fsim{
	class GameManager;

	class Structure{
		public:
			Structure(GameManager*,int,vb01::Vector3,vb01::Quaternion);
			~Structure();
			virtual void update();
			inline int getId(){return id;}
			inline int getFaction(){return faction;}
			inline void setPos(vb01::Vector3 p){this->pos=p;}
			inline vb01::Quaternion getRot(){return rot;}
			inline vb01::Vector3 getPos(){return pos;}
		protected:
			GameManager *gm;
			int id,faction;
			vb01::Vector3 pos,dir=vb01::Vector3(0,0,-1),left=vb01::Vector3(1,0,0),up=vb01::Vector3(0,1,0);
			vb01::Quaternion rot;
			vb01::Model *model;
			vb01::Node *rootNode,*guiNode;
			vb01::Material *mat;
	};
}

#endif
