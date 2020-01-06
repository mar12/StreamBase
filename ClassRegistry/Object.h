/**
 * Representation of the object of type `T`, created by client
 *
 * Contains the object itself, as well as reference to `ClassMembers<T>` which contains pointers
 * to methods and attributes.
 */
#ifndef CLASSREGISTRY_OBJECT_H_
#define CLASSREGISTRY_OBJECT_H_
#include "ObjectInterface.h"

namespace ClassRegistry {

template <typename T>
class Object : public ObjectInterface {
public:
	Object(ClassMembers<T>& members) : m_object(), m_members(members) {}
	~Object() {}

	Utility::RawData getAttrib(const MemberId& id) const override {
		return m_members.getAttrib(m_object, id);
	}

	bool setAttrib(const MemberId& id, const Utility::RawData& data) override {
		return m_members.setAttrib(m_object, id, data);
	}

	void callMethod(const MemberId& id) override {
		m_members.callMethod(m_object, id);
	}

private:
	T m_object;
	ClassMembers<T>& m_members;

};

}

#endif // CLASSREGISTRY_OBJECT_H_
