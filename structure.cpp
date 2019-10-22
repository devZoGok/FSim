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
	Structure::Structure(GameManager *gm,int id, Vector3 pos, Quaternion rot){
		this->gm=gm;
		this->id=id;
		this->pos=pos;
		this->rot=rot;	
		rootNode=gm->getRoot()->getRootNode();
		guiNode=gm->getRoot()->getGuiNode();

		model=new Model(PATH+path[id]+".obj");
		mat=new Material();
		mat->addDiffuseMap(defaultTexture);
		model->setMaterial(mat);
		model->setPosition(pos);
		rootNode->attachChild(model);
	}

	Structure::~Structure(){}

	void Structure::update(){}
}
