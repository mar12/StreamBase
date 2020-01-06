#include "TestClassRegistry.h"
#include <iostream>
#include "SampleClassTimer.h"
#include "ClassRegistry/ClassesPool.h"
#include "ClassRegistry/ObjectInterface.h"

namespace Tests {

using namespace ClassRegistry;
using namespace Utility;

static bool checkAttrib(ObjectInterface& object, const MemberId& id, int expected_value) {
    int actual_value;
    if (!object.getAttrib(id).toValue<int>(actual_value)) {
        std::cerr << __func__ << " - error: cannot convert attribute " << id << " to int " << std::endl;
        return false;
    }
    if (actual_value != expected_value) {
        std::cerr << __func__ << " - error: count expected to be " << expected_value << " instead of " << actual_value << std::endl;
        return false;
    }
    return true;
}

bool testClassRegistry() {
    bool success = true;

    /// Register a class
    ClassesPool& pool = ClassesPool::instance();

    std::string test_class_id = "SampleClassTimer";
    auto& registered_class = pool.addClass<SampleClassTimer>(test_class_id);

    registered_class.addAttrib("count",   &SampleClassTimer::m_count);
    registered_class.addAttrib("enabled", &SampleClassTimer::m_enabled);

    registered_class.addMethod("start", &SampleClassTimer::start);
    registered_class.addMethod("stop",  &SampleClassTimer::stop);
    registered_class.addMethod("tick",  &SampleClassTimer::tick);
    registered_class.addMethod("reset", &SampleClassTimer::reset);

    /// Create object of the class
    auto& class_interface = pool.getClass(test_class_id);

    std::string test_object_id = "test_object";
    class_interface.createObject(test_object_id);
    auto& object = registered_class.getObject(test_object_id);

    /// Set attributes
    static constexpr int enable_init_value = 1;
    static constexpr int count_init_value = 2;
    object.setAttrib("enabled", RawData::fromValue(enable_init_value));
    object.setAttrib("count", RawData::fromValue(count_init_value));

    /// Check if they were set properly and are readen correctly
    if (!checkAttrib(object, "enabled", enable_init_value)) {
        success = false;
    }
    if (!checkAttrib(object, "count", count_init_value)) {
        success = false;
    }

    /// Call methods
    object.callMethod("tick");
    if (!checkAttrib(object, "count", count_init_value + 1)) {
        success = false;
    }

    object.callMethod("stop");
    if (!checkAttrib(object, "enabled", 0)) {
        success = false;
    }

    object.callMethod("tick");
    if (!checkAttrib(object, "count", count_init_value + 1)) {
        success = false;
    }

    object.callMethod("reset");
    if (!checkAttrib(object, "count", 0)) {
        success = false;
    }

    object.callMethod("start");
    if (!checkAttrib(object, "enabled", 1)) {
        success = false;
    }

    object.callMethod("tick");
    if (!checkAttrib(object, "count", 1)) {
        success = false;
    }

    class_interface.removeObject(test_object_id);

    return success;

}

}