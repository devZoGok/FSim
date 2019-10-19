#include"abstractAppState.h"
#include"util.h"
#include"defConfigs.h"
#include"mapping.h"

using namespace std;

namespace fsim{
	AbstractAppState::AbstractAppState(){}

	AbstractAppState::~AbstractAppState(){}

	void AbstractAppState::onAttached(){
		attached=true;
		vector<string> lines;
		int startLine=0,endLine;
		for(int i=1;i<numStates;i++)
			startLine+=numBinds[i];
		endLine=startLine+numBinds[type];
		readFile(PATH+"../options.cfg",lines,startLine,endLine);

		for(int i=startLine;i<endLine;i++){
			Mapping::Type type;
			bool action;
			int trigger,semicolon;
			for(int j=0;j<lines[i].length();j++)	
				if(lines[i].c_str()[j]==':')
					semicolon=j;

			type=(Mapping::Type)atoi(lines[i].substr(semicolon+1,1).c_str());
			action=atoi(lines[i].substr(semicolon+3,1).c_str());
			trigger=atoi(lines[i].substr(semicolon+5,string::npos).c_str());

			Mapping *m=new Mapping;
			m->bind=(Mapping::Bind)i;
			mappings.push_back(m);	
		}
	}

	void AbstractAppState::onDettached(){
		attached=false;
	}

	void AbstractAppState::update(){}
}
