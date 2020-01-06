/**
 * Interface for a representation of any registered class
 *
 * Allows to list registered class members and objects of the class.
 */
#ifndef CLASSREGISTRY_CLASSINTERFACE_H_
#define CLASSREGISTRY_CLASSINTERFACE_H_
#include <string>
#include <memory>
#include "Utility/RawData.h"
#include "ObjectId.h"

namespace ClassRegistry {

class ObjectInterface;

class ClassInterface {
public:
	virtual ~ClassInterface() {};

	virtual void createObject(const ObjectId& id) = 0;
	virtual void removeObject(const ObjectId& id) = 0;
	virtual ObjectInterface& getObject(const ObjectId& id) = 0;

	virtual std::string listObjects() const = 0;
	virtual std::string listAttribs() const = 0;
	virtual std::string listMethods() const = 0;
};

}

#endif  // CLASSREGISTRY_CLASSINTERFACE_H_