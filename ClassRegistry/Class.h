/**
 * Representation of a registered class `T`
 *
 * Contains the list of registered class members and objects of that particular class.
 */
#ifndef CLASSREGISTRY_CLASS_H_
#define CLASSREGISTRY_CLASS_H_
#include <string>
#include <map>
#include <mutex>
#include "Utility/RawData.h"
#include "ClassInterface.h"
#include "ClassMembers.h"
#include "Object.h"
#include "ObjectId.h"

namespace ClassRegistry {

class ObjectInterface;

template <typename T>
class Class : public ClassInterface {
public:
	explicit Class() {}
	~Class() {}

	void addMethod(const MemberId& id, typename ClassMembers<T>::MethodPtr method_ptr) {
		m_members.addMethod(id, method_ptr);
	}

	template <typename U>
	void addAttrib(const MemberId& id, ClassMembers<T>::AttribPtr<U> attrib_ptr) {
		m_members.addAttrib<U>(id, attrib_ptr);
	}


	void createObject(const ObjectId& id) override {
		std::lock_guard<std::mutex> lock(m_mutex);
		auto object = std::make_shared<Object<T>>(m_members);
		m_objects.emplace(id, object);
	}

	ObjectInterface& getObject(const ObjectId& id) override {
		std::lock_guard<std::mutex> lock(m_mutex);
		return *m_objects.at(id);
	}

	void removeObject(const ObjectId& id) override {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_objects.count(id) == 0) {
			throw std::out_of_range(__func__);
		}
		m_objects.erase(id);
	}

	std::string listObjects() const override {
		std::string result;
		bool first = true;
		std::lock_guard<std::mutex> lock(m_mutex);
		for (const auto& object: m_objects) {
			if (first) {
				first = false;
			}
			else {
				result.push_back('\0');
			}
			result += object.first;
		}
		return result;
	}

	std::string listAttribs() const override {
		return m_members.listAttribs();
	}

	std::string listMethods() const override {
		return m_members.listMethods();
	}

private:
	ClassMembers<T> m_members;
	std::map<ObjectId, std::shared_ptr<ObjectInterface>> m_objects;
	mutable std::mutex m_mutex;
};

}

#endif // CLASSREGISTRY_CLASS_H_