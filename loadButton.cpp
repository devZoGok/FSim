#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"gameManager.h"
#include"stateManager.h"
#include"loadButton.h"
#include"okButton.h"
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
		class SaveSelectionButton : public Button{
			public:
				SaveSelectionButton(GameManager *gm, Vector2 pos, Vector2 size,Listbox *listbox) : Button(gm,pos,size,"Ok"){
					this->listbox=listbox;
				}
				void onClick(){
					int pilotId=listbox->getSelectedOption();
					GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
					Button *e[]{this};
					guiState->removeAllGUIElements(false);
					guiState->removeAllButtons(e,1);

					Connection conn(false);
					conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
					StoreQueryResult res=conn.query("select name from saves where pid="+to_string(pilotId)+";").store();
					vector<string> lines;
					for(int i=0;i<res.size();i++)
						lines.push_back((string)res[i][0]);
					guiState->addListbox(new Listbox(gm,Vector2(300,300),Vector2(100,20),lines,lines.size()>5?5:lines.size()));
					guiState->addButton(new OkButton(gm,Vector2(300,400),Vector2(100,40),nullptr,pilotId));

					guiState->removeButton(this);
				}
			private:
				Listbox *listbox;
		};

		Connection conn(false);
		conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
		StoreQueryResult res=conn.query("select name,faction from pilots;").store();
		int numPilots=res.num_rows();
		if(numPilots>0){
			vector<string> lines;
			GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
			Button *e[]{this};
			guiState->removeAllButtons(e,1);

			for(int i=0;i<numPilots;i++)
				lines.push_back((string)res[i][0]);
			Listbox *listbox=new Listbox(gm,Vector2(300,300),Vector2(100,20),lines,numPilots<5?numPilots:5);
			guiState->addListbox(listbox);
			guiState->addButton(new MainMenuButton(gm,Vector2(400,400),Vector2(100,40)));
			guiState->addButton(new SaveSelectionButton(gm,Vector2(300,400),Vector2(100,40),listbox));

			guiState->removeButton(this);
		}
	}
}
