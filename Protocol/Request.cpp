#include "Request.h"

namespace Protocol {

Utility::OutputBuffer& operator<<(Utility::OutputBuffer& stream, const Request& request) {
    return stream << static_cast<int>(request.m_action_id) << request.m_class_id
        << request.m_object_id << request.m_member_id << request.m_data;
}

Utility::InputBuffer& operator>>(Utility::InputBuffer& stream, Request& request) {
    int action_id;
    stream >> action_id >> request.m_class_id >> request.m_object_id >> request.m_member_id >> request.m_data;
    request.m_action_id = static_cast<ActionId>(action_id);
    return stream;
}

}