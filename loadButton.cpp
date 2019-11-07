#define MYSQLPP_MYSQL_HEADERS_BURIED
#include"gameManager.h"
#include"stateManager.h"
#include"loadButton.h"
#include"listbox.h"
#include"mainMenuButton.h"
#include"guiAppState.h"
#include<mysql++.h>

using namespace vb01;
using namespace std;
using namespace mysqlpp;

namespace fsim{
	LoadButton::LoadButton(GameManager *gm,Vector2 pos, Vector2 size):Button(gm,pos,size,"Load",true){}

	void LoadButton::onClick(){
		Connection conn(false);
		conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
		StoreQueryResult res=conn.query("select name from pilots;").store();
		int numPilots=res.num_rows();
		if(numPilots>0){
			vector<string> lines;
			GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
			Button *e[]{this};
			guiState->removeAllButtons(e,1);

			for(int i=0;i<numPilots;i++)
				lines.push_back((string)res[i][0]);
			guiState->addListbox(new Listbox(gm,Vector2(300,300),Vector2(100,20),lines,numPilots<5?numPilots:5));
			guiState->addButton(new MainMenuButton(gm,Vector2(400,400),Vector2(100,40)));

			guiState->removeButton(this);
		}
	}
}
