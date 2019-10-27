#include"util.h"
#include"mapping.h"
#include"guiAppState.h"
#include"gameManager.h"
#include"stateManager.h"
#include"abstractAppState.h"
#include"playButton.h"
#include"loadButton.h"
#include"optionsButton.h"
#include"exitButton.h"
#include<glfw3.h>
#include<vector.h>

using namespace std;
using namespace vb01;
using namespace chrono;
using namespace boost::filesystem;

namespace fsim{
	std::string keyStrings[]{
		"Escape",
		"Enter",
		"Tab",
		"Backspace",
		"Insert",
		"Delete",
		"Right",
		"Left",
		"Up",
		"Down",
		"PageUp",
		"PageDown",
		"Home",
		"End",
		"CapsLock",
		"ScrollLock",
		"NumLock",
		"PrintScreen",
		"Pause",
		"Numpad .",
		"Numpad /",
		"Numpad *",
		"Numpad -",
		"Numpad +",
		"Numpad enter",
		"=",
		"Left shift",
		"Left control",
		"Left alt",
		"Left super",
		"Right shift",
		"Right control",
		"Right alt",
		"Right super",
		"Left click",
		"Right click",
		"Middle click"
	};
	int triggers[]{
		GLFW_KEY_ESCAPE,
		GLFW_KEY_ENTER,
		GLFW_KEY_TAB,
		GLFW_KEY_BACKSPACE,
		GLFW_KEY_INSERT,
		GLFW_KEY_DELETE,
		GLFW_KEY_RIGHT,
		GLFW_KEY_LEFT,
		GLFW_KEY_UP,
		GLFW_KEY_DOWN,
		GLFW_KEY_PAGE_UP,
		GLFW_KEY_PAGE_DOWN,
		GLFW_KEY_HOME,
		GLFW_KEY_END,
		GLFW_KEY_CAPS_LOCK,
		GLFW_KEY_SCROLL_LOCK,
		GLFW_KEY_NUM_LOCK,
		GLFW_KEY_PRINT_SCREEN,
		GLFW_KEY_PAUSE,
		GLFW_KEY_KP_DECIMAL,
		GLFW_KEY_KP_DIVIDE,
		GLFW_KEY_KP_MULTIPLY,
		GLFW_KEY_KP_SUBTRACT,
		GLFW_KEY_KP_ADD,
		GLFW_KEY_KP_ENTER,
		GLFW_KEY_EQUAL,
		GLFW_KEY_LEFT_SHIFT,
		GLFW_KEY_LEFT_CONTROL,
		GLFW_KEY_LEFT_ALT,
		GLFW_KEY_LEFT_SUPER,
		GLFW_KEY_RIGHT_SHIFT,
		GLFW_KEY_RIGHT_CONTROL,
		GLFW_KEY_RIGHT_ALT,
		GLFW_KEY_RIGHT_SUPER,
		GLFW_MOUSE_BUTTON_LEFT,
		GLFW_MOUSE_BUTTON_RIGHT,
		GLFW_MOUSE_BUTTON_MIDDLE
	};

	bool isSuchFile(boost::filesystem::path path){
		bool found=false,foundFileName=false;
		string fileName;
		boost::filesystem::path mapDir;
		int length=path.string().length();
		for(int i=length;i>=0&&!foundFileName;i--)
			if(path.string().c_str()[i]=='/'){
				fileName=path.string().substr(i+1,length-i);
				mapDir=path.string().substr(0,i);
				foundFileName=true;
			}
		directory_iterator dir;
		for(directory_iterator itr(mapDir);itr!=dir;itr++){
			if(itr->path().string()==path.string())
				found=true;
		}
		return found;
	}

	void getRecDirs(path p,std::vector<string> &list,bool showFiles, bool showDirs, bool rec){
		directory_iterator dir;
		for(directory_iterator itr(p);itr!=dir;itr++){
			if((showFiles&&is_regular_file(itr->path()))||(showDirs&&is_directory(itr->path())))
				list.push_back((string)itr->path().c_str());
			if(rec&&is_directory(itr->path()))
				getRecDirs(itr->path(),list,showFiles,showDirs,rec);
		}
	}
        
	void readFile(string path,std::vector<string> &lines,int firstLine,int lastLine){
		std::ifstream inFile(path);
		if(lastLine!=-1)
			for(int i=0;i<lastLine;i++){
				string l;
				inFile>>l;
				if(i>=firstLine)
					lines.push_back(l);
			}
		else{
			string l;
			while(getline(inFile,l))
				lines.push_back(l);
		} 
		inFile.close();
	}

	void writeFile(string path,std::vector<string> &lines){
		std::ofstream outFile(path);
		for(string l : lines)
			outFile<<l<<"\n";
		outFile.close();
	}

	string fromIntToString(int data[3]){
		string output;

		switch((Mapping::BindType)data[0]){
			case Mapping::KEYBOARD:
			case Mapping::MOUSE_KEY:
				{
					bool specialKey=false;
					int numSpecialKeys=sizeof(triggers)/sizeof(int);
					for(int i=0;i<numSpecialKeys;i++)
					if(data[2]==triggers[i]){
						specialKey=true;
						output=keyStrings[i];
					}
					if(!specialKey){
						char c[]{data[2]};
						output=string(c);
					}
					break;
				}
			case Mapping::MOUSE_AXIS:
				output="MOUSE_AXIS_"+to_string(data[2])+(data[1]?"+":"-");
				break;
			case Mapping::JOYSTICK_KEY:
				output="JOYSTICK_BUTTON_"+to_string(data[2]);
				break;
			case Mapping::JOYSTICK_AXIS:
				output="JOYSTICK_AXIS_"+to_string(data[2]/2)+(data[1]?"+":"-");
				break;
		}
		return output;
	}

	int* fromStringToInt(string data){
		int *output=new int[3],length=data.length();

		if(data.substr(0,11)=="MOUSE_AXIS_"){
			output[0]=Mapping::MOUSE_AXIS;
			output[1]=0;
			output[2]=atoi(data.substr(11,string::npos-1).c_str())+(int)data.c_str()[12]=='+';
		}
		else if(data.substr(0,16)=="JOYSTICK_BUTTON_"){
			output[0]=Mapping::JOYSTICK_KEY;
			output[1]=1;
			output[2]=atoi(data.substr(16,string::npos-1).c_str());
		}
		else if(data.substr(0,14)=="JOYSTICK_AXIS_"){
			output[0]=Mapping::JOYSTICK_AXIS;
			output[1]=0;
			output[2]=2*atoi(data.substr(14,string::npos).c_str())+(data.c_str()[15]=='+'?1:0);
		}
		else{
			bool specialKey=false;
			int numSpecialKeys=sizeof(triggers)/sizeof(int);
			for(int i=0;i<numSpecialKeys;i++)
				if(data==keyStrings[i]){
					specialKey=true;
					output[0]=numSpecialKeys-i<3?Mapping::MOUSE_KEY:Mapping::KEYBOARD;
					output[2]=triggers[i];
				}
			if(!specialKey){
				output[0]=Mapping::KEYBOARD;
				output[2]=data.c_str()[0];
			}
			output[1]=1;
		}

		return output;
	}

	void makeTitleScreenButtons(GameManager *gm){
		GuiAppState *guiState=(GuiAppState*)gm->getStateManager()->getState(AbstractAppState::GUI_STATE);
		guiState->addButton(new PlayButton(gm,Vector2(100,100),Vector2(100,50)));
		guiState->addButton(new LoadButton(gm,Vector2(100,160),Vector2(100,50)));
		guiState->addButton(new OptionsButton(gm,Vector2(100,220),Vector2(100,50)));
		guiState->addButton(new ExitButton(gm,Vector2(100,280),Vector2(100,50)));
	}
}
