#include"optionsButton.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"listbox.h"
#include<vector>

using namespace vb01;
using namespace std;

namespace fsim{
	OptionsButton::OptionsButton(GameManager *gm,Vector2 pos, Vector2 size) : Button(gm,pos,size,"Options",true){
	}

	void OptionsButton::onClick(){
		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		vector<string> lines;
		for(int i=0;i<50;i++)
			lines.push_back(to_string(i));
		Listbox *listbox=new Listbox(gm,Vector2(400,200),Vector2(100,20),lines,9);
		guiState->addListbox(listbox);
	}
}
