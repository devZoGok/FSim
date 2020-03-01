#include"button.h"
#include"gameManager.h"
#include<root.h>
#include<quad.h>
#include<node.h>
#include<material.h>
#include<text.h>
#include<texture.h>
#include"defConfigs.h"

using namespace std;
using namespace vb01;

namespace fsim{
	Button::Button(GameManager *gm, Vector2 pos, Vector2 size,string name,Mapping::Bind bind,bool separate,string imagePath){
		this->gm=gm;
		this->pos=pos;
		this->size=size;
		this->name=name;
		this->separate=separate;
		this->imagePath=imagePath;

		mapping.bind=bind;
		mapping.trigger=buttonTriggers[(int)bind-(int)Mapping::NONE-1];
		mapping.action=1;
		mapping.type=Mapping::KEYBOARD;

		guiNode=gm->getRoot()->getGuiNode();
		rect=new Quad(Vector3(size.x,size.y,0),false);
		rectNode=new Node(Vector3(pos.x,pos.y,0));
		Material *mat=new Material(Material::MATERIAL_GUI);
		if(imagePath==""){
			mat->setTexturingEnabled(false);
			mat->setDiffuseColor(color);
		}
		else{
			mat->setTexturingEnabled(true);
			mat->addDiffuseMap(imagePath);
			textures.push_back(mat->getDiffuseMap(0));
		}
		rect->setMaterial(mat);
		rectNode->attachMesh(rect);
		guiNode->attachChild(rectNode);	

		if(separate){
			text=new Text(PATH+"Fonts/batang.ttf",name);
			text->setScale(.2);
			textNode=new Node(Vector3(pos.x,pos.y+size.y,-.1));
			textNode->addText(text);
			guiNode->attachChild(textNode);
		}
	}

	Button::~Button(){
		if(textNode){
			guiNode->dettachChild(textNode);
			delete textNode;
		}
		guiNode->dettachChild(rectNode);
		delete rectNode;
	}

	void Button::update(){}

	void Button::onMouseOver(){
		mouseOver=true;
	}

	void Button::onMouseOff(){
		mouseOver=false;
	}

	void Button::setPos(Vector2 pos){
		this->pos=pos;
		float z1=rectNode->getPosition().z;
		rectNode->setPosition(Vector3(pos.x,pos.y,z1));
		if(textNode){
			float z2=textNode->getPosition().z;
			textNode->setPosition(Vector3(pos.x,pos.y+size.y,z2));
		}
	}

	void Button::setSize(Vector2 size){
		this->size=size;
		rect->setSize(Vector3(size.x,size.y,0));
	}

	void Button::setColor(Vector4 c){
		this->color=c;
		rect->getMaterial()->setDiffuseColor(color);
	}
	
	void Button::setZOrder(float zOrder){
		Vector3 rectPos=rectNode->getPosition();
		rectNode->setPosition(Vector3(rectPos.x,rectPos.y,zOrder));
		if(textNode){
			Vector3 textPos=textNode->getPosition();	
			textNode->setPosition(Vector3(textPos.x,textPos.y,zOrder-.1));
		}
	}

	void Button::setImage(string image){
		int texId=-1;
		Material *mat=rect->getMaterial();
		for(int i=0;i<textures.size();i++)
			if(textures[i]->getPath()==image)
				texId=i;
		if(texId==-1){
			string p[]{image};
			textures.push_back(new Texture(p,1));
			mat->setDiffuseMap(textures[textures.size()-1],0);
		}
		else
			mat->setDiffuseMap(textures[texId],0);
		this->imagePath=image;
	}
}
