#include"structure.h"
#include"structureData.h"
#include"defConfigs.h"
#include"gameManager.h"
#include<root.h>
#include<node.h>
#include<model.h>
#include<material.h>

using namespace vb01;

namespace fsim{
	Structure::Structure(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot){
		this->gm=gm;
		this->id=id;
		this->pos=pos;
		this->rot=rot;	
		this->faction=faction;
		rootNode=gm->getRoot()->getRootNode();
		guiNode=gm->getRoot()->getGuiNode();

		dir=rot*dir;
		left=rot*left;
		up=rot*up;

		model=new Model(PATH+path[id]+".obj");
		mat=new Material();
		mat->addDiffuseMap(defaultTexture);
		model->setMaterial(mat);
		model->setPosition(pos);
		model->setOrientation(rot);
		rootNode->attachChild(model);
	}

	Structure::~Structure(){}

	void Structure::update(){
	}
}
