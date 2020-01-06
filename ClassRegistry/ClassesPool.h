/**
 * Registered classes pool
 *
 * Allows to register a class, list registered classes, and retrieve the registered class interface.
 */
#ifndef CLASSREGISTRY_CLASSESPOOL_H_
#define CLASSREGISTRY_CLASSESPOOL_H_
#include <string>
#include <map>
#include <memory>
#include "Class.h"
#include "ClassId.h"

namespace ClassRegistry {

class ClassesPool {
public:
	static ClassesPool& instance() {
		static ClassesPool singleton;
		return singleton;
	}

	template <typename T>
	Class<T>& addClass(const ClassId& id) {
		auto ptr = std::make_shared<Class<T>>();
		m_classes.emplace(id, ptr);
		return *ptr;
	}

	ClassInterface& getClass(const ClassId& id) const {
		return *m_classes.at(id);
	}

	std::string listClasses() const {
		std::string result;
		bool first = true;
		for (const auto& class1 : m_classes) {
			if (first) {
				first = false;
			}
			else {
				result.push_back('\0');
			}
			result += class1.first;
		}
		return result;
	}

private:
	ClassesPool() = default;
	ClassesPool(const ClassesPool&) = delete;
	ClassesPool(ClassesPool&&) = delete;
	ClassesPool& operator=(const ClassesPool&) = delete;
	ClassesPool& operator=(ClassesPool&&) = delete;
	~ClassesPool() = default;

	std::map<ClassId, std::shared_ptr<ClassInterface>> m_classes;

};

}

#endif  // CLASSREGISTRY_CLASSESPOOL_H_