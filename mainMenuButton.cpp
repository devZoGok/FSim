#include"mainMenuButton.h"
#include"gameManager.h"
#include"stateManager.h"
#include"guiAppState.h"
#include"util.h"

using namespace vb01;

namespace fsim{
	MainMenuButton::MainMenuButton(GameManager *gm, Vector2 pos, Vector2 size) : Button(gm,pos,size,"Main Menu"){}
	
	void MainMenuButton::onClick(){
		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		Button *e[]{this};
		guiState->removeAllGUIElements(false);
		guiState->removeAllButtons(e,1);
		makeTitleScreenButtons(gm);
		guiState->removeButton(this);
	}
}
