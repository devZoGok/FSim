#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"map.h"
#include"upgradeData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"guiAppState.h"
#include"aircraftSelectionButton.h"
#include"jet.h"
#include"helicopter.h"
#include"runway.h"
#include"helipad.h"
#include"sam.h"
#include"structureData.h"
#include"runway.h"
#include"helipad.h"
#include"aircraftCarrier.h"
#include<model.h>
#include<root.h>
#include<node.h>
#include<light.h>
#include<material.h>
#include<mysql++.h>
#include"util.h"

using namespace fsim::structureData;
using namespace std;
using namespace vb01;
using namespace mysqlpp;

namespace fsim{
	Map::Map(GameManager *gm,string path,InGameAppState *inGameState,int saveId,int level,int objective,int pilotId){
		this->gm=gm;
		this->level=level;
		this->objective=objective;
		this->path=path;
		rootNode=gm->getRoot()->getRootNode();

		Root *root=gm->getRoot();
		string t[]={
			path+"top.jpg",
			path+"bottom.jpg",
			path+"left.jpg",
			path+"right.jpg",
			path+"front.jpg",
			path+"back.jpg"
		};
		root->createSkybox(t);

		mapModel=new Model(path+"level.obj");
		Material *mat=new Material();
		mat->addDiffuseMap(defaultTexture);
		mat->setLightingEnabled(true);
		mapModel->setMaterial(mat);
		rootNode->attachChild(mapModel);

		vector<string> lines;
		readFile(path+"level.cfg",lines);
		int lightsLine=0,structuresLine=0,objectivesLine=0,spawnLine=0;
		for(int i=0;i<lines.size();i++)
			if(lines[i]=="Lighting:")
				lightsLine=i;
			else if(lines[i]=="Structures:")
				structuresLine=i;
			else if(lines[i]=="Objectives:")
				objectivesLine=i;
			else if(lines[i]=="Spawn:")
				spawnLine=i+1;

		for(int i=lightsLine+1;i<structuresLine;i++){
			string lightName;
			bool endOfLightName=false;
			for(int j=0;j<lines[i].length()&&!endOfLightName;j++){
				char ch=lines[i].c_str()[j];
				if(ch=='.'||ch==','){
					endOfLightName=true;
					lightName=lines[i].substr(0,j);
				}
			}

			Light::Type t=Light::DIRECTIONAL;
			if(lightName=="point"){
				t=Light::POINT;
			}
			else if(lightName=="spot"){
				t=Light::SPOT;
			}

			const int numCoords=10;
			float coords[numCoords];
			getLineData(lines[i],coords,numCoords,1);
			Light *light=new Light(t);
			Vector3 pos=Vector3(coords[0],coords[1],coords[2]).norm();
			Vector3 dir=Vector3(0,-1,0).norm();
			Vector3 color=Vector3(coords[7],coords[8],coords[9]);
			light->setDirection(dir);
			light->setPosition(pos);
			light->setAttenuationValues(.0001,.00001,1);
			light->setColor(color);
			lights.push_back(light);
			rootNode->addLight(light);
		}

		if(saveId==-1){
			for(int i=structuresLine+1;i<objectivesLine;i++){
				const int numCoords=8;
				float coords[numCoords];
				getLineData(lines[i],coords,numCoords,1);
				int structureId,faction=coords[0];
				Vector3 pos=Vector3(coords[1],coords[2],coords[3]);
				Quaternion rot=Quaternion(coords[4],coords[5],coords[6],coords[7]);

				string structureName;
				bool endOfStructureName=false;
				for(int j=0;j<lines[i].length()&&!endOfStructureName;j++){
					char ch=lines[i].c_str()[j];
					if(ch=='.'||ch==','){
						endOfStructureName=true;
						structureName=lines[i].substr(0,j);
					}
				}
				for(int j=0;j<numStructures;j++)
					if(structureName==structureData::name[j])
						structureId=j;
				createStructure(inGameState,structureId,faction,pos,rot,nullptr,true);
			}
			for(int i=objectivesLine+1;i<spawnLine-1;i++){
				int coords[2];
				getLineData(lines[i],coords,2);
				bool failureCondition=coords[0];

				Objective::Condition *success=new Objective::Condition,*failure=nullptr;
				success->type=(Objective::Type)coords[1];
				createObjective(inGameState,success,lines[i],1);
				if(failureCondition){
					int offset;
					failure=new Objective::Condition;
					switch(success->type){
						case Objective::DESTROY:
							int numTargets[1];
							getLineData(lines[i],numTargets,1,2);
							offset=3+numTargets[0];
							break;
						case Objective::MOVE_TO:
							offset=5;
							break;
						case Objective::WAIT:
							offset=3;
							break;
					}
					int t[1];
					getLineData(lines[i],t,1,offset);
					failure->type=(Objective::Type)t[0];
					createObjective(inGameState,failure,lines[i],offset);
				}
				Objective o;
				o.success=success;
				o.failure=failure;
				objectives.push_back(o);
			}
			int airbaseData[3];
			getLineData(lines[spawnLine],airbaseData,3);
			if(airbaseData[0]==-1&&airbaseData[1]==-1&&airbaseData[2]==-1){
				int aircraftId[1];
				getLineData(lines[spawnLine],aircraftId,1,3);
				this->aircraftId=aircraftId[0];
				float spawn[7];
				getLineData(lines[spawnLine],spawn,7,4);
				spawnPos=Vector3(spawn[0],spawn[1],spawn[2]);
				spawnRot=Quaternion(spawn[3],spawn[4],spawn[5],spawn[6]);
			}
			else{
				int faction=inGameState->getFaction();
				vector<Runway*> runways;
				vector<Helipad*> helipads;
				vector<AircraftCarrier*> carriers;

				for(Structure *s : inGameState->getStructures())
					if(s->getFaction()==faction&&s->getId()==structureData::RUNWAY)
						runways.push_back((Runway*)s);
				for(Structure *s : inGameState->getStructures())
					if(s->getFaction()==faction&&s->getId()==structureData::HELIPAD)
						helipads.push_back((Helipad*)s);
				for(Structure *s : inGameState->getStructures()){
					if(s->getFaction()==faction){
						int id=s->getId();
						if(id>=structureData::CHINESE_CARRIER&&id<=structureData::KOREAN_CARRIER)
							carriers.push_back((AircraftCarrier*)s);
					}
				}
				if(airbaseData[0]!=-1)
					homeRunway=runways[airbaseData[0]];
				if(airbaseData[1]!=-1)
					homeHelipad=helipads[airbaseData[1]];
				if(airbaseData[2]!=-1)
					homeCarrier=carriers[airbaseData[2]];
			}
		}
		else{
			Connection conn(false);
			conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
			int playerId=(int)conn.query("select puid from pilots p inner join saves s on p.pid=s.pid where sid="+to_string(saveId)+";").store()[0][0];
			StoreQueryResult res=conn.query("select uid,su.faction,pos_x,pos_y,pos_z,rot_w,rot_x,rot_y,rot_z from pilots p inner join saves s inner join save_units su on p.pid=s.pid and s.sid=su.sid where p.pid="+to_string(pilotId)+" and s.sid="+to_string(saveId)+";").store();

			for(int i=0;i<res.size();i++){
				int unitId=(int)res[i][0],faction=(int)res[i][1];
				Vector3 pos=Vector3((float)res[i][2],(float)res[i][3],(float)res[i][4]);
				Quaternion rot=Quaternion((float)res[i][5],(float)res[i][6],(float)res[i][7],(float)res[i][8]);
				int *upgrades=nullptr;
				if(i==playerId){
					upgrades=new int[numUpgrades];
					StoreQueryResult upgradesResults=conn.query("select fighter_upgrades,fighter_bomber_upgrades,helicopter_upgrades from pilots where pid="+to_string(pilotId)+";").store();
					string upgradesString=(string)upgradesResults[0][unitId%3];
					for(int j=0;j<upgradesString.length();j++){
						upgrades[j]=atoi(upgradesString.substr(j,1).c_str());
					}
				}
				createStructure(inGameState,unitId,faction,pos,rot,upgrades,i!=playerId);
			}
		}
	}

	Map::~Map(){
		gm->getRoot()->removeSkybox();
		rootNode->dettachChild(mapModel);
		delete mapModel;
		while(!lights.empty()){
			rootNode->removeLight(0);
			delete lights[0];
			lights.erase(lights.begin());
		}
	}

	void Map::foo(int i){
		InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
		int playerId=inGameState->getPlayerId();
		Aircraft *playerAircraft=nullptr;
		if(playerId!=-1)
			playerAircraft=(Aircraft*)inGameState->getStructure(playerId);
		Objective::Condition *c=(i==0?objectives[0].failure:objectives[0].success);
		switch(c->type){
			case Objective::DESTROY:
				{
					for(int j=0;j<c->targets.size();j++){
						bool destroyed=true;
						for(Structure *s : inGameState->getStructures()){
							if(s==c->targets[j])
								destroyed=false;
						}
						if(destroyed)
							c->targets.erase(c->targets.begin()+j);
					}
					if(c->targets.empty())
						objectives[0].status=(i==0?Objective::FAILURE:Objective::SUCCESS);
					break;
				}
			case Objective::MOVE_TO:
				if(playerAircraft){
					float maxDist=3;
					Vector3 pos=playerAircraft->getPos(),destPos=c->pos;
					if(Vector2(pos.x,pos.z).getDistanceFrom(Vector2(destPos.x,destPos.z))<=maxDist)
						objectives[0].status=(i==0?Objective::FAILURE:Objective::SUCCESS);
				}
				break;
			case Objective::WAIT:
				if(getTime()-c->initTime>c->time)
					objectives[0].status=(i==0?Objective::FAILURE:Objective::SUCCESS);
				break;
		}
	}

	void Map::update(){
		if(!objectives.empty())
			if(objectives[0].status==Objective::PENDING){
				if(objectives[0].failure)
					foo(0);
				if(objectives[0].success)
					foo(1);
			}
			else{
				InGameAppState *inGameState=(InGameAppState*)gm->getStateManager()->getState(AbstractAppState::IN_GAME_STATE);
				if(objectives[0].status==Objective::SUCCESS&&objectives.size()==1){
					inGameState->endLevel(true);
					return;
				}
				else if(objectives[0].status==Objective::FAILURE){
					inGameState->endLevel(false);
					return;
				}
				objectives.erase(objectives.begin());
				objective++;
			}
	}

	template<typename T>
	void Map::getLineData(string line,T *data,int numData,int offset){
		int offsetComma=0,c1=0;
		for(int i=0;i<line.length()&&offsetComma<offset;i++)
			if(line.c_str()[i]==','){
				c1=i+1;
				offsetComma++;
			}
		int c2=c1;
		for(int i=0;i<numData;i++){
			for(int j=c1;j<line.length();j++)	
				if(line.c_str()[j]==','){
					c2=j;
					break;
				}
			data[i]=atof(line.substr(c1,c2-c1).c_str());
			c2++;
			c1=c2;
		}
	}

	void Map::createStructure(InGameAppState *inGameState,int id,int faction,Vector3 pos,Quaternion rot,int *upgrades,bool ai){
		Structure *s;
		switch(id){
			case Type::SAM_SITE:
				s=new SAM(gm,faction,pos,rot);
				break;
			case Type::CHINESE_HELICOPTER:
			case Type::JAPANESE_HELICOPTER:
			case Type::KOREAN_HELICOPTER:
				s=new Helicopter(gm,id,faction,pos,rot,ai,upgrades);
				break;
			case Type::CHINESE_FIGHTER:
			case Type::JAPANESE_FIGHTER:
			case Type::KOREAN_FIGHTER:
			case Type::CHINESE_FIGHTER_BOMBER:
			case Type::JAPANESE_FIGHTER_BOMBER:
			case Type::KOREAN_FIGHTER_BOMBER:
				s=new Jet(gm,id,faction,pos,rot,ai,upgrades);
				break;
			case Type::CHINESE_CARRIER:
			case Type::JAPANESE_CARRIER:
			case Type::KOREAN_CARRIER:
				s=new AircraftCarrier(gm,id,faction,pos,rot);
				break;
			case Type::RUNWAY:
				s=new Runway(gm,faction,pos,rot);
				break;
			case Type::HELIPAD:
				s=new Helipad(gm,faction,pos);
				break;
			default:
				s=new Structure(gm,id,faction,pos,rot);
				break;
		}
		inGameState->addStructure(s);
	}

	void Map::createObjective(InGameAppState *inGameState,Objective::Condition *c,string line,int successOffset){
		switch(c->type){
			case Objective::DESTROY:{
				int numTargets[1];
				getLineData(line,numTargets,1,1+successOffset);
				const int num=numTargets[0];
				int targets[num];
				getLineData(line,targets,num,2+successOffset);
				for(int j=0;j<num;j++)
					c->targets.push_back(inGameState->getStructure(targets[j]));
				break;
			}
			case Objective::MOVE_TO:{
				float data[3];
				getLineData(line,data,3,1+successOffset);
				Vector3 destination=Vector3(data[0],data[1],data[2]);
				c->pos=destination;
				break;
			}
			case Objective::WAIT:{
				int data[1];
				getLineData(line,data,1,1+successOffset);
				c->initTime=getTime();
				c->time=(s64)data[0];
				break;
			}
		}
	}
}
