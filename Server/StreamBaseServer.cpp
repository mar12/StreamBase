#include "StreamBaseServer.h"
#include "ClassRegistry/ClassesPool.h"

namespace Server {

using namespace ClassRegistry;
using namespace Protocol;

StreamBaseServer::StreamBaseServer(const std::string& pipename) :
    NamedPipeServer(pipename) {
}

static Response processListClasses() {
    ClassesPool& pool = ClassesPool::instance();
    return Response(ResultId::Success, Utility::RawData::fromValue(pool.listClasses()));
}


static Response processListObjects(const ClassId& class_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        return Response(ResultId::Success, Utility::RawData::fromValue(class_interface.listObjects()));
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

static Response processListAttribs(const ClassId& class_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        return Response(ResultId::Success, Utility::RawData::fromValue(class_interface.listAttribs()));
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

static Response processListMethods(const ClassId& class_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        return Response(ResultId::Success, Utility::RawData::fromValue(class_interface.listMethods()));
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

static Response processCreateObject(const ClassId& class_id, const ObjectId& object_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        class_interface.createObject(object_id);
        return Response(ResultId::Success);
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

static Response processRemoveObject(const ClassId& class_id, const ObjectId& object_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        try {
            class_interface.removeObject(object_id);
            return Response(ResultId::Success);
        }
        catch (const std::out_of_range&) {
            return Response(ResultId::InvalidObject);
        }
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

static Response processCallMethod(const ClassId& class_id, const ObjectId& object_id, const MemberId& member_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        try {
            ObjectInterface& object = class_interface.getObject(object_id);
            try {
                object.callMethod(member_id);
                return Response(ResultId::Success);
            }
            catch (const std::out_of_range&) {
                return Response(ResultId::InvalidMember);
            }
        }
        catch (const std::out_of_range&) {
            return Response(ResultId::InvalidObject);
        }
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

static Response processGetAttrib(const ClassId& class_id, const ObjectId& object_id, const MemberId& member_id) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        try {
            ObjectInterface& object = class_interface.getObject(object_id);
            try {
                return Response(ResultId::Success, object.getAttrib(member_id));
            }
            catch (const std::out_of_range&) {
                return Response(ResultId::InvalidMember);
            }
        }
        catch (const std::out_of_range&) {
            return Response(ResultId::InvalidObject);
        }
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}


static Response processSetAttrib(const ClassId& class_id, const ObjectId& object_id, const MemberId& member_id, const Utility::RawData& data) {
    ClassesPool& pool = ClassesPool::instance();
    try {
        ClassInterface& class_interface = pool.getClass(class_id);
        try {
            ObjectInterface& object = class_interface.getObject(object_id);
            try {
                object.setAttrib(member_id, data);
                return Response(ResultId::Success);
            }
            catch (const std::out_of_range&) {
                return Response(ResultId::InvalidMember);
            }
        }
        catch (const std::out_of_range&) {
            return Response(ResultId::InvalidObject);
        }
    }
    catch (const std::out_of_range&) {
        return Response(ResultId::InvalidClass);
    }
}

Response StreamBaseServer::process(const Request& request) {
    try {

        switch (request.actionId()) {

        case ActionId::SendRawData:
            m_last_raw_data = request.data();
            return Response(ResultId::Success);

        case ActionId::RecvRawData:
            return Response(ResultId::Success, m_last_raw_data);

        case ActionId::CreateObject:
            return processCreateObject(request.classId(), request.objectId());

        case ActionId::RemoveObject:
            return processRemoveObject(request.classId(), request.objectId());

        case ActionId::ListClasses:
            return processListClasses();

        case ActionId::ListObjects:
            return processListObjects(request.classId());

        case ActionId::ListAttribs:
            return processListAttribs(request.classId());

        case ActionId::ListMethods:
            return processListMethods(request.classId());

        case ActionId::GetAttrib:
            return processGetAttrib(request.classId(), request.objectId(), request.memberId());

        case ActionId::SetAttrib:
            return processSetAttrib(request.classId(), request.objectId(), request.memberId(), request.data());

        case ActionId::CallMethod:
            return processCallMethod(request.classId(), request.objectId(), request.memberId());

        default:
            return Response(ResultId::InvalidAction);

        }
    }
    catch (...) {
        return Response(ResultId::UnknownError);
    }

}

}