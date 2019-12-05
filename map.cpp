#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"map.h"
#include"upgradeData.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"jet.h"
#include"helicopter.h"
#include"structureData.h"
#include<model.h>
#include<root.h>
#include<node.h>
#include<light.h>
#include<material.h>
#include<mysql++.h>
#include"util.h"

using namespace std;
using namespace vb01;
using namespace mysqlpp;

namespace fsim{
	Map::Map(GameManager *gm,string path,InGameAppState *inGameState,int saveId,int level,int objective,int pilotId){
		this->gm=gm;
		this->level=level;
		this->objective=objective;
		rootNode=gm->getRoot()->getRootNode();

		mapModel=new Model(path+"level.obj");
		Material *mat=new Material();
		mat->addDiffuseMap(path+"defaultTexture.jpg");
		mapModel->setMaterial(mat);
		rootNode->attachChild(mapModel);

		vector<string> lines;
		readFile(path+"level.cfg",lines);
		int lightsLine=0,structuresLine=0;
		for(int i=0;i<lines.size();i++)
			if(lines[i]=="Lighting:")
				lightsLine=i;
			else if(lines[i]=="Structures:")
				structuresLine=i;

		for(int i=lightsLine+1;i<structuresLine;i++){
			Light *light=new Light();
			Light::Type t;
			string type;
			const int numCoords=6;
			float coords[numCoords];
			getCoords(lines[i],type,coords,numCoords);
			if(type=="directional"){
				t=Light::DIRECTIONAL;
				Vector3 dir=Vector3(coords[0],coords[1],coords[2]);
				Vector3 color=Vector3(coords[3],coords[4],coords[5]);
				light->setDirection(dir);
				light->setColor(color);
				//rootNode->addLight(light);
			}
			else if(type=="point"){
				t=Light::POINT;
			}

		}

		if(saveId==-1){
			inGameState->addStructure(new Helicopter(gm,Type::HELICOPTER,2,Vector3(0,20,0),Quaternion(1,0,0,0)));
			for(int i=structuresLine+1;i<lines.size();i++){
				string type;
				const int numCoords=7;
				float coords[numCoords];
				getCoords(lines[i],type,coords,numCoords);
				Vector3 pos=Vector3(coords[0],coords[1],coords[2]);
				Quaternion rot=Quaternion(coords[3],coords[4],coords[5],coords[6]);

				int structureId,faction=0;
				if(type=="samSite")
					structureId=SAM_SITE;
				inGameState->addStructure(new Structure(gm,structureId,faction,pos,rot));
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
				Structure *s;
				int *upgrades=nullptr;
				if(i==playerId){
					upgrades=new int[numUpgrades];
					StoreQueryResult upgradesResults=conn.query("select fighter_upgrades,fighter_bomber_upgrades,helicopter_upgrades from pilots where pid="+to_string(pilotId)+";").store();
					string upgradesString=(string)upgradesResults[0][unitId];
					for(int j=0;j<upgradesString.length();j++){
						upgrades[j]=atoi(upgradesString.substr(j,1).c_str());
					}
				}
				switch(unitId){
					case Type::FIGHTER_BOMBER:
					case Type::FIGHTER:
						s=new Jet(gm,unitId,faction,pos,rot,upgrades);
						break;
					case Type::HELICOPTER:
						s=new Helicopter(gm,unitId,faction,pos,rot,upgrades);
						break;
					default:
						s=new Structure(gm,unitId,faction,pos,rot);
						break;
				}
				inGameState->addStructure(s);
			}
		}


		string t[]={
			path+"top.jpg",
			path+"bottom.jpg",
			path+"left.jpg",
			path+"right.jpg",
			path+"front.jpg",
			path+"back.jpg"
		};
		gm->getRoot()->createSkybox(t);
	}

	Map::~Map(){}

	void Map::update(){}

	void Map::getCoords(string line, string &type, float *coords,const int numCoords){
		int colonId,dotId=-1;
		for(int j=0;j<line.length();j++){
			if(line.c_str()[j]==':')
				colonId=j;
			else if(line.c_str()[j]=='.')
				dotId=j;
		}
		int c1=colonId+1,c2=c1;
		for(int j=0;j<numCoords;j++){
			for(int z=c1;z<line.length();z++)	
				if(line.c_str()[z]==','){
					c2=z;
					break;
				}
			coords[j]=atof(line.substr(c1,c2-c1).c_str());
			c2++;
			c1=c2;
		}
		type=line.substr(0,colonId);
		if(dotId!=-1)
			type=type.substr(0,dotId);
	}
}
