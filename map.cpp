#include"map.h"
#include"gameManager.h"
#include<model.h>
#include<root.h>
#include<node.h>
#include<light.h>
#include<material.h>
#include"util.h"

using namespace std;
using namespace vb01;

namespace fsim{
	Map::Map(GameManager *gm,string path){
		this->gm=gm;
		rootNode=gm->getRoot()->getRootNode();

		mapModel=new Model(path+"level.obj");
		Material *mat=new Material();
		mat->addDiffuseMap(path+"defaultTexture.jpg");
		mapModel->setMaterial(mat);
		//rootNode->attachChild(mapModel);

		vector<string> lines;
		readFile(path+"level.cfg",lines);
		int lightsLine=0,structuresLine=0,unitsLine=0;
		for(int i=0;i<lines.size();i++)
			if(lines[i]=="lights:")
				lightsLine=i;
			else if(lines[i]=="structures:")
				structuresLine=i;
			else if(lines[i]=="units:")
				unitsLine=i;

		for(int i=lightsLine+1;i<structuresLine;i++){
			Light *light=new Light();
			Light::Type t;
			int semicolon=0;
			for(int j=0;j<lines[i].length();j++)
				if(lines[i].c_str()[j]==':')
					semicolon=j;

			if(lines[i].substr(0,semicolon)=="directional"){
				const int numCoords=6;
				float coords[numCoords];
				for(int j=0;j<numCoords;j++){
					int c1=semicolon+1,c2=c1;
					for(int z=c1;z<lines[i].length()&&c2==c1;z++)	
						if(lines[i].c_str()[z]==',')
							c2=z;
					coords[j]=atof(lines[i].substr(c1,c2-c1).c_str());
				}
				t=Light::DIRECTIONAL;
				Vector3 dir=Vector3(coords[0],coords[1],coords[2]);
				Vector3 color=Vector3(coords[3],coords[4],coords[5]);
				light->setDirection(dir);
				light->setColor(color);
			}
			else if(lines[i].substr(0,semicolon)=="point"){
				t=Light::POINT;
			}

		}

		for(int i=structuresLine+1;i<unitsLine;i++){}

		for(int i=unitsLine+1;i<lines.size();i++){}

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
}
