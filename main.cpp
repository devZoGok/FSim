#include<root.h>
#include<quad.h>
#include<box.h>
#include<node.h>
#include<text.h>
#include<string>

using namespace vb01;
using namespace std;

int main(){
	string basePath="/home/dominykas/c++/FSim/";

	Root *r=Root::getSingleton();
	r->start(800,600);
	Node *guiNode=r->getGuiNode(),*rootNode=r->getRootNode();

	Camera *cam=r->getCamera();
	//cam->lookAt(Vector3(1,0,1).norm());

	while(true){;
		r->update();
	}
	return 0;
}
