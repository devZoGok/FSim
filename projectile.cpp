#include"defConfigs.h"
#include"projectile.h"
#include"projectileData.h"
#include"gameManager.h"
#include<iostream>
#include<root.h>
#include<model.h>
#include<material.h>

using namespace vb01;
using namespace std;

namespace fsim{
	Projectile::Projectile(GameManager *gm,int id,Structure *structure,Vector3 pos,Quaternion rot){
		this->gm=gm;
		this->id=id;
		this->structure=structure;
		this->pos=pos;
		this->rot=rot;
		this->speed=projectileData::speed[id];
		this->turnSpeed=projectileData::turnSpeed[id];
		this->length=projectileData::length[id];
		
		Node *rootNode=gm->getRoot()->getRootNode();
		mesh=new Model(PATH+"Models/Projectiles/"+projectileData::meshPath[id]);
		Material *mat=new Material();
		mat->addDiffuseMap(defaultTexture);
		mesh->setMaterial(mat);
		rootNode->attachChild(mesh);

		Vector3 initDir=Vector3(0,0,1);
		Vector3 initLeft=Vector3(1,0,0);
		Vector3 initUp=Vector3(0,1,0);
		this->dir=rot*initDir;
		this->left=rot*initLeft;
		this->up=rot*initUp;
	}
	
	Projectile::~Projectile(){
		mesh->getParent()->dettachChild(mesh);
		delete mesh;
	}

	void Projectile::update(){
		mesh->setPosition(pos);
		mesh->setOrientation(rot);
	}

	void Projectile::checkForCollision(){
	}

	void Projectile::explode(){
		exploded=true;
	}
}
