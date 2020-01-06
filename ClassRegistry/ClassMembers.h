/**
 * Registered class members for class `T`
 *
 * Stores list of methods and attributes, as well as allows to
 * set or get them for provided instance and call the method for it.
 */
#ifndef CLASSREGISTRY_CLASSMEMBERS_H_
#define CLASSREGISTRY_CLASSMEMBERS_H_
#include "Utility/RawData.h"
#include "MemberId.h"

namespace ClassRegistry {

template <typename T>
class ClassMembers {
public:
	using MethodPtr = void(T::*)();

	template <typename U> using AttribPtr = U T::*;

	class AttribInterface {
	public:
		virtual ~AttribInterface() = default;
		virtual Utility::RawData getValue(const T& object) const = 0;
		virtual bool setValue(T& object, const Utility::RawData&) const = 0;
	};

	template <typename U>
	class Attrib : public AttribInterface {
	public:
		explicit Attrib(AttribPtr<U> ptr) : m_ptr(ptr) {}

		Utility::RawData getValue(const T& object) const override {
			return Utility::RawData::fromValue<U>(object.*m_ptr);
		}

		bool setValue(T& object, const Utility::RawData& data) const override {
			return data.toValue<U>(object.*m_ptr);
		}

	private:
		AttribPtr<U> m_ptr;

	};

	template <typename U>
	void addAttrib(const MemberId& id, AttribPtr<U> attrib_ptr) {
		auto attrib = std::make_shared<Attrib<U>>(attrib_ptr);
		m_attribs.emplace(id, attrib);
	}

	/// Currently, only methods without parametrs and without return values can be registered
	void addMethod(const MemberId& id, MethodPtr method) {
		m_methods.emplace(id, method);
	}


	Utility::RawData getAttrib(const T& object, const MemberId& id) const {
		return m_attribs.at(id)->getValue(object);
	}

	bool setAttrib(T& object, const MemberId& id, const Utility::RawData& data) const {
		return m_attribs.at(id)->setValue(object, data);
	}

	void callMethod(T& object, const MemberId& id) const {
		auto method = m_methods.at(id);
		(object.*method)();
	}

	std::string listMethods() const {
		std::string result;
		bool first = true;
		for (const auto& method : m_methods) {
			if (first) {
				first = false;
			}
			else {
				result.push_back('\0');
			}
			result += method.first;
		}
		return result;
	}

	std::string listAttribs() const {
		std::string result;
		bool first = true;
		for (const auto& attrib: m_attribs) {
			if (first) {
				first = false;
			}
			else {
				result.push_back('\0');
			}
			result += attrib.first;
		}
		return result;
	}

private:
	std::map<MemberId, std::shared_ptr<AttribInterface>> m_attribs;
	std::map<MemberId, MethodPtr> m_methods;

};

}

#endif // CLASSREGISTRY_CLASSMEMBERS_H_