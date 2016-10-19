#ifndef SUIOBJECT_H
#define SUIOBJECT_H
#include <vector>

class SUIObject
{
public:
    SUIObject();
    void addChild(SUIObject *child);
    void setParent(SUIObject *parent);
private:
    std::vector<SUIObject *> node;
};

#endif // SUIOBJECT_H
