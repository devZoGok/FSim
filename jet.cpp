#include"jet.h"
#include"missile.h"
#include"bomb.h"
#include"projectileData.h"
#include"inGameAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"structureData.h"
#include<model.h>
#include<material.h>
#include<particleEmitter.h>
#include<vector>

using namespace vb01;
using namespace std;

namespace fsim{
	Jet::Jet(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot,int *upgrades) : Aircraft(gm,id,faction,pos,rot,upgrades){
		gas=new ParticleEmitter(100);
		Node *gasNode=new Node(Vector3(0,0,-1));
		Material *gasMat=new Material(Material::MATERIAL_PARTICLE);
		gasMat->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
		gas->setMaterial(gasMat);
		gasNode->attachParticleEmitter(gas);
		model->attachChild(gasNode);

		gas->setStartColor(Vector4(1,1,0,1));
		gas->setEndColor(Vector4(.8,.8,0,1));
		gas->setStartSize(Vector2(.1,.1));
		gas->setEndSize(Vector2(1,1));
		gas->setLowLife(.04);
		gas->setHighLife(.1);
		gas->setSpread(1);
		gas->setDirection(Vector3(0,0,-.4));

		smoke=new ParticleEmitter(100);
		Node *smokeNode=new Node(Vector3(0,0,-1));
		Material *smokeMat=new Material(Material::MATERIAL_PARTICLE);
		smokeMat->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
		smoke->setMaterial(smokeMat);
		smokeNode->attachParticleEmitter(smoke);
		//model->attachChild(smokeNode);

		smoke->setStartColor(Vector4(.7,.7,.7,1));
		smoke->setEndColor(Vector4(.7,.7,.7,.4));
		smoke->setStartSize(Vector2(.1,.1));
		smoke->setEndSize(Vector2(3,3));
		smoke->setLowLife(.1);
		smoke->setHighLife(1);
		smoke->setSpread(20);
		smoke->setDirection(Vector3(0,0,-.1));
	}

	Jet::~Jet(){}

	void Jet::update(){
		Aircraft::update();
		pos=pos+dir*.1;
		gas->setDirection(dir*-.4);
		smoke->setDirection(dir*-.1);
	}

}
