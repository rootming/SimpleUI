#ifndef SUIOBJECT_H
#define SUIOBJECT_H
#include <vector>


namespace sui {

class SUIObject
{
public:
    SUIObject(SUIObject *parent = nullptr);
    void addChild(SUIObject *child);
    void setParent(SUIObject *parent);
    static std::vector<SUIObject *> objNode;

private:
    std::vector<SUIObject *> node;
    SUIObject *parentObj;
    void addObjNode(void);

};

}
#endif // SUIOBJECT_H
