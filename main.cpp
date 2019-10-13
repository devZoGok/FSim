#include<root.h>
#include<quad.h>
#include<box.h>
#include<node.h>
#include<text.h>
#include<string>
#include"gameManager.h"
#include"guiAppState.h"
#include"stateManager.h"
#include"button.h"
#include<iostream>

using namespace vb01;
using namespace std;
using namespace fsim;

int main(){
	class B : public Button{
		public:
			B(GameManager *gm) : Button(gm,Vector2(100,100),Vector2(100,100),"Play"){
				
			}
			void onClick(){
				cout<<"KEK\n";
			}
		private:
	};
	GameManager *gm=new GameManager();
	GuiAppState *guiState=new GuiAppState(gm);
	B *b=new B(gm);
	guiState->addButton(b);
	gm->getStateManager()->attachState(guiState);

	while(true)
		gm->update();
	return 0;
}
