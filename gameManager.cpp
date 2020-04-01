#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"gameManager.h"
#include"inputManager.h"
#include"stateManager.h"
#include"util.h"
#include<root.h>
#include<mysql++.h>

using namespace vb01;
using namespace std;
using namespace mysqlpp;

namespace fsim{
	GameManager::GameManager(){
		readOptions();

		root = Root::getSingleton();
		running=true;

		root->start(width,height);

		stateManager=new StateManager();
		inputManager=new InputManager(this,root->getWindow());

		Connection conn(false);					
		if(!conn.connect("fsim","localhost",options.databaseUser.c_str(),"")){
			cout<<"Failed to connect to FSim database\n";
			exit(-1);
		}
		string savesQuery="create table saves(sid int not null,name varchar(100) not null,pid int not null,puid int not null,lid int not null,oid int not null,primary key (sid));",
			   saveUnitsQuery="create table save_units(sid int not null,uid int not null,faction int not null",
			   statsQuery="create table stats(pid int not null,score int not null,deaths int not null";
		conn.query("create table pilots(pid int not null,faction int not null,level int not null,score int not null,name varchar(40),fighter_upgrades varchar(5),fighter_bomber_upgrades varchar(5),helicopter_upgrades varchar(5),primary key (pid));").store();
		
		char *coords[]{"w","x","y","z"};
		for(int i=1;i<4;i++)
			saveUnitsQuery+=",pos_"+string(coords[i])+" decimal(50,10) not null";
		for(int i=0;i<4;i++)
			saveUnitsQuery+=",rot_"+string(coords[i])+" decimal(50,10) not null";
		saveUnitsQuery+=");";

		string structures[]{"buildings","aircraft","vehicles","ships"};
		for(int i=0;i<4;i++)
			statsQuery+=","+structures[i]+"_destroyed int not null";	
		statsQuery+=");";
		conn.query(savesQuery).store();
		conn.query(saveUnitsQuery).store();
		conn.query(statsQuery).store();
		root->setHDREnabled(false);
		root->setBloom(false);
		root->setBlurLevel(10);
	}

	GameManager::~GameManager(){}

	void GameManager::update(){
		root->update();	

		stateManager->update();
		inputManager->update();
	}

	void GameManager::readOptions(){
		int firstLine=0;
		for(int i=0;i<numStates;i++)
			firstLine+=numBinds[i];
		vector<string> lines;
		readFile("../options.cfg",lines,firstLine);

		const int numOptions=lines.size();
		int colonIds[numOptions];
		for(int i=0;i<numOptions;i++)
			for(int j=0;j<lines[i].length();j++)
				if(lines[i].c_str()[j]==':')
					colonIds[i]=j;
		width=atoi(lines[0].substr(colonIds[0]+1,string::npos).c_str());
		height=atoi(lines[1].substr(colonIds[1]+1,string::npos).c_str());
		options.width=width;
		options.height=height;
		options.textureLevel=atoi(lines[2].substr(colonIds[2]+1,string::npos).c_str());
		options.databaseUser=lines[numOptions-1].substr(colonIds[numOptions-1]+1,string::npos);
	}
}
