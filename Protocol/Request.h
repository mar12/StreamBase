/**
 * Representation of the data contained in the request
 *
 * Request uses the following format:
 *
 * +-----------+----------+-----------+-----------+------+
 * | Action ID | Class ID | Object ID | Member ID | Data |
 * +-----------+----------+-----------+-----------+------+
 *
 * Depending on the action requested, some fields are not used in the particular request.
 * The exact binary representation of each field is governed by the type of the field,
 * as well as by the type serialization operator implementation for `Utility::InputBuffer`
 * and `Utility::OutputBuffer` classes.
 *
 * The length of the request must not exceed the `maxLength()`
 *
 */
#ifndef PROTOCOL_REQUEST_H_
#define PROTOCOL_REQUEST_H_
#include "ActionId.h"
#include "ClassRegistry/ClassId.h"
#include "ClassRegistry/ObjectId.h"
#include "ClassRegistry/MemberId.h"
#include "Utility/RawData.h"
#include "Utility/InputBuffer.h"
#include "Utility/OutputBuffer.h"

namespace Protocol {

class Request {
public:
    Request() {}

    Request(ActionId action_id, ClassRegistry::ClassId class_id = "", ClassRegistry::ObjectId object_id = "",
            ClassRegistry::MemberId member_id = "", const Utility::RawData& data = Utility::RawData()) :
        m_action_id(action_id), m_class_id(class_id), m_object_id(object_id), m_member_id(member_id), m_data(data) {
    }

    Request(ActionId action_id, const Utility::RawData& data) : Request(action_id, "", "", "", data) {}

    ActionId                actionId() const { return m_action_id; }
    ClassRegistry::ClassId  classId () const { return m_class_id; }
    ClassRegistry::ObjectId objectId() const { return m_object_id; }
    ClassRegistry::MemberId memberId() const { return m_member_id; }
    const Utility::RawData& data    () const { return m_data; }

    static constexpr std::size_t maxLength() { return 4096; }

private:
    ActionId                m_action_id;
    ClassRegistry::ClassId  m_class_id;
    ClassRegistry::ObjectId m_object_id;
    ClassRegistry::MemberId m_member_id;
    Utility::RawData        m_data;

    friend Utility::OutputBuffer& operator<<(Utility::OutputBuffer& stream, const Request& request);
    friend Utility::InputBuffer&  operator>>(Utility::InputBuffer&  stream, Request& request);
};

Utility::OutputBuffer& operator<<(Utility::OutputBuffer& stream, const Request& request);
Utility::InputBuffer& operator>>(Utility::InputBuffer& stream, Request& request);

}

#endif // PROTOCOL_REQUEST_H_