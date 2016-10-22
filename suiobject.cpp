#include "suiobject.h"
#include "suidebug.h"

namespace sui {

std::vector<SUIObject *> SUIObject::objNode;

SUIObject::SUIObject(SUIObject *parent)
{
    addObjNode();
    setParent(parent);
}

void SUIObject::addChild(SUIObject *child)
{
    node.push_back(child);
    SUIDEBUG_INFO("SUIObject: add child object.\n");
}

void SUIObject::setParent(SUIObject *parent)
{
    //TODO: if we have muilt parent, how can i fix the problem?
    parentObj = parent;
    if(parent != nullptr)
        parent->addChild(this);
    SUIDEBUG_INFO("SUIObject: set parent object.\n");
}

void SUIObject::addObjNode(void)
{
    SUIDEBUG_INFO("SUIObject: add object.\n");
    objNode.push_back(this);
}

}
