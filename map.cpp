#include"map.h"
#include"gameManager.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"structure.h"
#include"structureData.h"
#include<model.h>
#include<root.h>
#include<node.h>
#include<light.h>
#include<material.h>
#include"util.h"

using namespace std;
using namespace vb01;

namespace fsim{
	Map::Map(GameManager *gm,string path,InGameAppState *inGameState){
		this->gm=gm;
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

		for(int i=structuresLine+1;i<lines.size();i++){
			string type;
			const int numCoords=6;
			float coords[numCoords];
			getCoords(lines[i],type,coords,numCoords);
			Vector3 pos=Vector3(coords[0],coords[1],coords[2]);
			Quaternion rot=Quaternion(coords[3],coords[4],coords[5],coords[6]);

			int structureId;
			if(type=="samSite")
				structureId=SAM_SITE;
			inGameState->addStructure(new Structure(gm,structureId,pos,rot));
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
