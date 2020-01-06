/**
 * Interface to the representation of the object of type `T`, created by client
 */
#ifndef CLASSREGISTRY_OBJECTINTERFACE_H_
#define CLASSREGISTRY_OBJECTINTERFACE_H_
#include "Utility/RawData.h"
#include "MemberId.h"

namespace ClassRegistry {

class ObjectInterface {
public:
    virtual ~ObjectInterface() = default;

    virtual Utility::RawData getAttrib(const MemberId& id) const = 0;

    virtual bool setAttrib(const MemberId& id, const Utility::RawData& value) = 0;

    virtual void callMethod(const MemberId& id) = 0;

};

}

#endif // CLASSREGISTRY_OBJECTINTERFACE_H_