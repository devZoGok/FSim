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

		if(singleModel[id]){
			model=new Model(PATH+path[id]+".obj");
			mat=new Material();
			mat->addDiffuseMap(defaultTexture);
			mat->setLightingEnabled(true);
			model->setMaterial(mat);
			model->setPosition(pos);
			model->setOrientation(rot);
			model->setCastShadow(true);
			rootNode->attachChild(model);

			hitbox=new Model(PATH+hitboxPath[id]+".obj");
			hitbox->setWireframe(true);
			Material *mat=new Material();
			mat->setTexturingEnabled(false);
			mat->setDiffuseColor(Vector4(0,0,1,1));
			hitbox->setMaterial(mat);
			hitbox->setVisible(false);
			model->attachChild(hitbox);
		}
	}

	Structure::~Structure(){
		if(singleModel[id]){
			rootNode->dettachChild(model);
			delete model;
		}
	}

	void Structure::update(){
		/*
		hitbox->setPosition(pos);
		hitbox->setOrientation(rot);
		*/
	}
}
