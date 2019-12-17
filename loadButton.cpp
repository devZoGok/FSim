#define MYSQLPP_MYSQL_HEADERS_BURIED

#include"gameManager.h"
#include"structureData.h"
#include"stateManager.h"
#include"inGameAppState.h"
#include"jetAppState.h"
#include"helicopterAppState.h"
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
	LoadButton::LoadButton(GameManager *gm,Vector2 pos, Vector2 size):Button(gm,pos,size,"Load",Mapping::LOAD){}

	void LoadButton::onClick(){
		class PilotSelectionButton : public Button{
			public:
				PilotSelectionButton(GameManager *gm, Vector2 pos, Vector2 size,Listbox *listbox) : Button(gm,pos,size,"Ok"){
					this->listbox=listbox;
				}
				void onClick(){
					class SaveSelectionButton : public Button{
						public:
							SaveSelectionButton(GameManager *gm,Vector2 pos,Vector2 size,Listbox *l,int pilotId) : Button(gm,pos,size,"Ok"){
								this->l=l;
								this->pilotId=pilotId;
							}
							void onClick(){
								Connection conn(false);
								conn.connect("fsim","localhost",gm->getOptions().databaseUser.c_str(),"");
								StateManager *stateManager=gm->getStateManager();

								int saveId=l->getSelectedOption(),faction=(int)conn.query("select faction from pilots where pid="+to_string(pilotId)+";").store()[0][0];
								StoreQueryResult res=conn.query("select uid,puid,lid,oid from pilots p inner join saves s inner join save_units su on p.pid=s.pid and s.sid=su.sid where p.pid="+to_string(pilotId)+" and s.sid="+to_string(saveId)+";").store();
								int level=(int)res[0][2],objective=(int)res[0][3],playerId=(int)res[0][1],unitId=(int)res[playerId][0];
								stateManager->attachState(new InGameAppState(gm,pilotId,playerId,faction,saveId,level,objective));
								switch(unitId){
									case Type::CHINESE_HELICOPTER:
									case Type::JAPANESE_HELICOPTER:
									case Type::KOREAN_HELICOPTER:
										stateManager->attachState(new HelicopterAppState(gm,playerId));
									break;
									default:
										stateManager->attachState(new JetAppState(gm,playerId));
									break;
								}

								GuiAppState *guiState=(GuiAppState*)stateManager->getState(AbstractAppState::GUI_STATE);
								guiState->removeAllGUIElements();
							}
						private:
							int pilotId;
							Listbox *l;
					};

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
					Listbox *l=new Listbox(gm,Vector2(300,300),Vector2(100,20),lines,lines.size()>5?5:lines.size());
					guiState->addListbox(l);
					guiState->addButton(new SaveSelectionButton(gm,Vector2(300,400),Vector2(100,50),l,pilotId));

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
			guiState->addButton(new PilotSelectionButton(gm,Vector2(300,400),Vector2(100,40),listbox));

			guiState->removeButton(this);
		}
	}
}
