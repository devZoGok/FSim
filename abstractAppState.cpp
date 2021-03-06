#include"abstractAppState.h"
#include"util.h"
#include"defConfigs.h"
#include"mapping.h"

using namespace std;

namespace fsim{
	AbstractAppState::AbstractAppState(GameManager *gm){this->gm=gm;}

	AbstractAppState::~AbstractAppState(){}

	void AbstractAppState::onAttached(){
		attached=true;
		vector<string> lines;
		int startLine=0,endLine;
		for(int i=1;i<int(type);i++)
			startLine+=numBinds[i];
		endLine=startLine+numBinds[type];
		readFile(PATH+"../options.cfg",lines,startLine,endLine);

		for(int i=0;i<lines.size();i++){
			Mapping::BindType type;
			bool action;
			int trigger,semicolon;
			for(int j=0;j<lines[i].length();j++)	
				if(lines[i].c_str()[j]==':')
					semicolon=j;

			type=(Mapping::BindType)atoi(lines[i].substr(semicolon+1,1).c_str());
			action=atoi(lines[i].substr(semicolon+3,1).c_str());
			trigger=atoi(lines[i].substr(semicolon+5,string::npos).c_str());

			Mapping *m=new Mapping;
			m->bind=(Mapping::Bind)(3+startLine+i);
			m->type=type;
			m->action=action;
			m->trigger=trigger;
			mappings.push_back(m);	
		}
	}

	void AbstractAppState::onDettached(){
		attached=false;
	}

	void AbstractAppState::update(){}

	void AbstractAppState::removeMapping(Mapping::Bind bind){
		int id=-1;
		for(int i=0;i<mappings.size();i++)
			if(mappings[i]->bind==bind)
				id=i;
		if(id!=-1)
			mappings.erase(mappings.begin()+id);
	}
}
