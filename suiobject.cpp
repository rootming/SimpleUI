#include "suiobject.h"
#include "suidebug.h"

using namespace std;

namespace sui {

vector<SUIObject *> SUIObject::objNode;

SUIObject::SUIObject(SUIObject *parent)
{
    addObjNode();
    setParent(parent);
}

void SUIObject::addChild(SUIObject *child)
{
    node.push_back(child);
    SUI_DEBUG_INFO("SUIObject: add child object.\n");
}

void SUIObject::setParent(SUIObject *parent)
{
    //TODO: if we have muilt parent, how can i fix the problem?
    parentObj = parent;
    if(parent != nullptr)
        parent->addChild(this);
    SUI_DEBUG_INFO("SUIObject: set parent object.\n");
}

void SUIObject::addObjNode(void)
{
    SUI_DEBUG_INFO("SUIObject: add object.\n");
    objNode.push_back(this);
}

void foreachChild()
{

}

void SUIObject::showChild()
{

}

}
