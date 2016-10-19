#include "suiobject.h"
#include "suidebug.h"

SUIObject::SUIObject()
{

}

void SUIObject::addChild(SUIObject *child)
{
    node.push_back(child);
    SUIDEBUG_INFO("SUIObject: add child object.\n");
}

void SUIObject::setParent(SUIObject *parent)
{
    //TODO: if we have muilt parent, how can i fix the problem?
    parent->addChild(this);
    SUIDEBUG_INFO("SUIObject: set parent object.\n");
}
