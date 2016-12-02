#pragma once

#include <vector>
using namespace std;

namespace sui {

class SUIObject
{
public:
    SUIObject(SUIObject *parent = nullptr);
    void addChild(SUIObject *child);
    void setParent(SUIObject *parent);
    SUIObject *getParent(void) const { return parentObj; }
    static vector<SUIObject *> objNode;

private:
    vector<SUIObject *> node;
    SUIObject *parentObj;
    void addObjNode(void);

};

}
