#include"aircraft.h"
#include"aircraftData.h"
#include"upgradeData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"defConfigs.h"
#include"projectileData.h"
#include"missile.h"
#include"map.h"
#include"gpsBomb.h"
#include"aiPilot.h"
#include<iostream>
#include<camera.h>
#include<model.h>
#include<quad.h>
#include<root.h>
#include<ray.h>
#include<box.h>
#include<particleEmitter.h>
#include<material.h>
#include<light.h>
#include<SFML/Audio.hpp>

using namespace vb01;
using namespace std;
using namespace fsim::structureData;

namespace fsim{
	Aircraft::Aircraft(GameManager *gm, int id, int faction, Vector3 pos, Quaternion rot, int *upgrades, bool ai) : Unit(gm,id,faction,pos,rot){
		if(!upgrades){
			this->upgrades=new int[numUpgrades];
			for(int i=0;i<numUpgrades;i++)
				this->upgrades[i]=0;
		}
		else
			this->upgrades=upgrades;
		if(ai)
			aiPilot=new AIPilot(gm,this);

		cam=gm->getRoot()->getCamera();
		this->rollSpeed=aircraftData::rollSpeed[id];
		this->yawSpeed=aircraftData::yawSpeed[id];
		this->pitchSpeed=aircraftData::pitchSpeed[id];
		this->weight=.1;
		this->rateOfPrimaryFire=aircraftData::rateOfPrimaryFire[id];
		this->rateOfSecondaryFire=aircraftData::rateOfSecondaryFire[id];
		this->rateOfChaff=aircraftData::rateOfChaff[id];
		this->primaryAmmo=aircraftData::primaryAmmo[id];
		this->secondaryAmmo=aircraftData::secondaryAmmo[id];
		this->chaff=aircraftData::chaff[id];
		this->fuel=aircraftData::fuelCapacity[id];

		muzzleFlash=new ParticleEmitter(50);
		Node *node=new Node(Vector3(0,0,2));
		Material *mat=new Material(Material::MATERIAL_PARTICLE);
		mat->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
		node->attachParticleEmitter(muzzleFlash);
		muzzleFlash->setMaterial(mat);
		node->setVisible(false);
		model->attachChild(node);
		muzzleFlash->setStartSize(Vector2(.5,.5));
		muzzleFlash->setEndSize(Vector2(.9,.9));
		muzzleFlash->setStartColor(Vector4(1,1,1,1));
		muzzleFlash->setEndColor(Vector4(1,1,0,1));
		/*
		*/

		engineSmoke=new ParticleEmitter(50);
		Node *engineSmokeNode=new Node(Vector3(0,0,2));
		Material *m=new Material(Material::MATERIAL_PARTICLE);
		m->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
		engineSmokeNode->attachParticleEmitter(engineSmoke);
		engineSmoke->setMaterial(m);
		engineSmokeNode->setVisible(false);
		model->attachChild(engineSmokeNode);
		engineSmoke->setStartSize(Vector2(.5,.5));
		engineSmoke->setEndSize(Vector2(.9,.9));
		engineSmoke->setStartColor(Vector4(1,1,1,1));
		engineSmoke->setEndColor(Vector4(1,1,0,1));

		sf::SoundBuffer *primaryFireSfxBuffer=new sf::SoundBuffer();
		primaryFireSfxBuffer->loadFromFile(PATH+"Sounds/shot.ogg");
		primaryFireSfx=new sf::Sound();
		primaryFireSfx->setBuffer(*primaryFireSfxBuffer);
		primaryFireSfx->setMinDistance(5);
		primaryFireSfx->setAttenuation(5);
		if(getType()!=FIGHTER_BOMBER){
			sf::SoundBuffer *secondaryFireSfxBuffer=new sf::SoundBuffer();
			secondaryFireSfxBuffer->loadFromFile(PATH+"Sounds/missile.ogg");
			secondaryFireSfx=new sf::Sound();
			secondaryFireSfx->setBuffer(*secondaryFireSfxBuffer);
			secondaryFireSfx->setMinDistance(5);
			secondaryFireSfx->setAttenuation(5);
		}

		sf::SoundBuffer *chaffSfxBuffer=new sf::SoundBuffer();
		chaffSfxBuffer->loadFromFile(PATH+"Sounds/chaff.ogg");
		chaffSfx=new sf::Sound();
		chaffSfx->setBuffer(*chaffSfxBuffer);
		chaffSfx->setMinDistance(5);
		chaffSfx->setAttenuation(5);

		/*
		muzzleLight=new Light(Light::POINT);
		muzzleLight->setColor(Vector3(1,1,0));
		muzzleLight->setAttenuationValues(.0007,.007,1);
		muzzleLight->setPosition(pos+dir*2);
		rootNode->addLight(muzzleLight);

		Quad *quad=new Quad(Vector3(100,100,.5),false,true);
		Material *mat=new Material(Material::MATERIAL_GUI);
		mat->addDiffuseMap(PATH+"Icons/Hitmarker/hitmarker.png");
		mat->setDiffuseColorEnabled(true);
		mat->setDiffuseColor(Vector4(0,0,1,1));
		quad->setMaterial(mat);
		model->attachMesh(quad);
		*/
	}

	Aircraft::~Aircraft(){
		if(aiPilot)
			delete aiPilot;
		Node *muzzleFlashNode=muzzleFlash->getNode();
		Node *engineSmokeNode=engineSmoke->getNode();
		model->dettachChild(muzzleFlashNode);
		model->dettachChild(engineSmokeNode);
		delete muzzleFlashNode;
		delete engineSmokeNode;
		delete primaryFireSfx->getBuffer();
		delete primaryFireSfx;
		if(getType()!=FIGHTER_BOMBER){
			delete secondaryFireSfx->getBuffer();
			delete secondaryFireSfx;
		}
		delete chaffSfx->getBuffer();
		delete chaffSfx;
	}

	void Aircraft::update(){
		Unit::update();
		if(aiPilot)
			aiPilot->update();
		if(hp<50)
			engineSmoke->getNode()->setVisible(true);
		if(cam){
			cam->setPosition(pos-dir*6);
			cam->lookAt(dir,up);
		}
		if(primaryFiring)
			primaryFire();
		if(getTime()-lastConsumption>rateOfFuelConsumption){
			fuel--;
			lastConsumption=getTime();
		}
		if(getTime()-lastPrimaryFire>30){
			muzzleFlash->getNode()->setVisible(false);
		}
		lockedOnto=!getHomingMissiles().empty();
		//muzzleFlash->setDirection(dir*-.1);
		/*
		for(int i=0;i<fx.size();i++){
			if(getTime()-fx[i].initTime>fx[i].timeToLive){
			}
			else{
				float g=9.8,speed=1,t=(float)(getTime()-fx[i].initTime)/1000;
				//x=x0+t*cos(v),y=y0+t*sin(v)-g*t^2/2;
				for(int j=0;j<fx[i].numEmitters;j++){
					Vector3 baseDir=fx[i].emitters[j]->getDirection();
					//fx[i].emitters[j]->setPosition(baseDir*t*cos(speed)+Vector3(0,t*sin(speed)-g*t*t/2,0));
				}
			}
		}
		*/
	}

	void Aircraft::primaryFire(){
		if(canPrimaryFire()){
			InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);

			vector<CollisionResult> results;
			Model *m=inGameState->getMap()->getMapModel();
			for(Structure *s : inGameState->getStructures()){
				Model *hitbox=s->getHitbox();
				if(s!=this&&hitbox)
					retrieveCollisions(pos,dir,s->getHitbox(),results);
			}
			sortResults(results);
			for(CollisionResult r : results){
				for(Structure *s : inGameState->getStructures()){
					Model *hitbox=s->getHitbox();
					if(hitbox&&hitbox->getChild(0)->getMesh(0)==r.mesh){
						int hp=s->getHp();
						hp-=10;
						s->setHp(hp);
					}
				}
			}

			primaryFireSfx->play();
			cout<<results.size()<<"\n";
			//primaryAmmo--;
			muzzleFlash->getNode()->setVisible(true);
			lastPrimaryFire=getTime();
		}
	}

	void Aircraft::secondaryFire(){
		if(canSecondaryFire()){
			InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
			Projectile *projectile;
			AircraftType type=getType();
			if(type!=FIGHTER_BOMBER){
				float maxAngle=.3;
				vector<Aircraft*> hostileTargets;
				for(Structure *s : inGameState->getStructures())
					if(s->getFaction()!=faction&&dir.getAngleBetween((s->getPos()-pos).norm())<=maxAngle){
						if((type==FIGHTER&&s->getId()<=Type::KOREAN_HELICOPTER)||(type==HELICOPTER&&s->getId()>=KOREAN_HELICOPTER))
							hostileTargets.push_back((Aircraft*)s);
					}
				Structure *target=hostileTargets.size()>0?hostileTargets[0]:nullptr;
				projectile=new Missile(gm,aircraftData::projectileId[id],this,pos-up*1,rot,target);
			}
			else{
				if(id==Type::KOREAN_FIGHTER_BOMBER){
					vector<Structure*> targets;
					Vector3 targetPos=Vector3::VEC_ZERO;
					for(Structure *s : inGameState->getStructures())
						if(s->getFaction()!=faction&&s->getId()>Type::KOREAN_HELICOPTER)
							targets.push_back(s);
					if(!targets.empty())
						targetPos=targets[0]->getPos();
					projectile=new GPSBomb(gm,this,targetPos,pos-up*.25,rot,.1);
				}
				else
					projectile=new Bomb(gm,aircraftData::projectileId[id],this,pos-up*.25,rot,.1);
			}
			if(secondaryFireSfx)
				secondaryFireSfx->play();
			inGameState->addProjectile(projectile);
			secondaryAmmo--;
			lastSecondaryFire=getTime();
		}
	}

	void Aircraft::deployChaff(){
		if(canDeployChaff()){
			InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
			const int numChaffSources=1;
			ParticleEmitter **chaffSources=new ParticleEmitter*[numChaffSources];
			Node *node=new Node(pos);
			rootNode->attachChild(node);

			vector<Missile*> homingMissiles=getHomingMissiles();
			for(Missile *m : homingMissiles)
				m->setTarget(nullptr);

			chaffSfx->play();

			chaffSources[0]=new ParticleEmitter(100);
			Material *mat=new Material(Material::MATERIAL_PARTICLE);
			mat->addDiffuseMap(PATH+"Textures/Smoke/smoke00.png");
			chaffSources[0]->setMaterial(mat);
			node->attachParticleEmitter(chaffSources[0]);
			chaffSources[0]->setLowLife(1);
			chaffSources[0]->setHighLife(2);
			chaffSources[0]->setStartSize(Vector2(.1,.1));
			chaffSources[0]->setEndSize(Vector2(.1,.1));
			chaffSources[0]->setStartColor(Vector4(.8,.8,.8,1));
			chaffSources[0]->setEndColor(Vector4(.8,.8,.8,0));
			chaffSources[0]->setDirection(dir*.1);
			chaffSources[0]->setSpread(360);
			node->attachParticleEmitter(chaffSources[0]);

			Fx fx;
			fx.emitters=chaffSources;
			fx.pos=pos;
			fx.timeToLive=2000;
			fx.initTime=getTime();
			inGameState->addFx(fx);
			chaff--;
		}
	}

	void Aircraft::roll(float angle){
		Quaternion rotQuat=Quaternion(angle,dir.norm());
		left=rotQuat*left,up=rotQuat*up;	
		rot=rotQuat*rot;
	}

	void Aircraft::yaw(float angle){
		Quaternion rotQuat=Quaternion(angle,up.norm());
		dir=rotQuat*dir,left=rotQuat*left;	
		rot=rotQuat*rot;
	}

	void Aircraft::pitch(float angle){
		Quaternion rotQuat=Quaternion(angle,left.norm());
		dir=rotQuat*dir,up=rotQuat*up;	
		rot=rotQuat*rot;
	}

	void Aircraft::setPitch(float p){
		this->pitchVal=p;
		if(abs(pitchVal)>pitchSpeed)
			pitchVal=pitchVal/pitchVal*pitchSpeed;
		pitch(p);
	}

	void Aircraft::setRoll(float r){
		this->rollVal=r;
		if(abs(rollVal)>rollSpeed)
			rollVal=rollVal/rollVal*rollSpeed;
		roll(r);
	}

	void Aircraft::setYaw(float y){
		this->yawVal=y;
		if(abs(yawVal)>yawSpeed)
			yawVal=yawVal/yawVal*yawSpeed;
		yaw(y);
	}

	vector<Missile*> Aircraft::getHomingMissiles(){
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		vector<Missile*> homingMissiles;
		for(Projectile *p : inGameState->getProjectiles()){
			int id=p->getId();
			if(id==projectileData::SAM||id<=projectileData::KOREAN_FIGHTER_AAM){
				Missile *m=(Missile*)p;
				if(m->getTarget()==this&&m->getPos().getDistanceFrom(pos)<=10)
					homingMissiles.push_back(m);
			}
		}
		return homingMissiles;
	}

	AircraftType Aircraft::getType(){
		AircraftType t;
		switch(id){
			case CHINESE_FIGHTER:
			case JAPANESE_FIGHTER:
			case KOREAN_FIGHTER:
				t=FIGHTER;
				break;
			case CHINESE_FIGHTER_BOMBER:
			case JAPANESE_FIGHTER_BOMBER:
			case KOREAN_FIGHTER_BOMBER:
				t=FIGHTER_BOMBER;
				break;
			case CHINESE_HELICOPTER:
			case JAPANESE_HELICOPTER:
			case KOREAN_HELICOPTER:
				t=HELICOPTER;;
				break;
		}
		return t;
	}
}
