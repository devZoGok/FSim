#include"button.h"
#include"gameManager.h"
#include<root.h>
#include<quad.h>
#include<node.h>
#include<material.h>
#include<text.h>
#include"defConfigs.h"

using namespace std;
using namespace vb01;

namespace fsim{
	Button::Button(GameManager *gm, Vector2 pos, Vector2 size,string name,string imagePath){
		this->gm=gm;
		this->pos=pos;
		this->size=size;
		this->name=name;

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
		}
		rect->setMaterial(mat);
		rectNode->attachMesh(rect);
		guiNode->attachChild(rectNode);	

		/*
		*/
		text=new Text(PATH+"batang.ttf",name);
		//text->setScale(.5);
		textNode=new Node(Vector3(pos.x,pos.y,-.1));
		textNode->addText(text);
		guiNode->attachChild(textNode);
	}

	Button::~Button(){}

	void Button::update(){}
}
