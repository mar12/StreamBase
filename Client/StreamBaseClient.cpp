#include "StreamBaseClient.h"
#include <string>
#include <iostream>
#include <thread>
#include "Utility/StringHelpers.h"

using namespace Protocol;

namespace Client {

StreamBaseClient::StreamBaseClient(const std::string& pipename) :
    m_pipe(pipename), m_is_async(false) {
}

void StreamBaseClient::runInteractively() {
    std::cout << "Enter command. For help, type `help` and press ENTER" << std::endl;
    for (;;) {
        /// Read command from command line
        std::cout << std::endl;
        std::cout << "StreamBase>";
        std::string command_line;
        std::getline(std::cin, command_line);
        std::cout << std::endl;
        if (command_line == "exit") {
            std::cout << "Good Bye!" << std::endl;
            return;
        }
        if (m_is_async) {
            /// `std::async` not used due to the synchronous destruction of `std::future`
            std::thread(&StreamBaseClient::processCommand, std::ref(*this), command_line).detach();
        } else {
            processCommand(command_line);
        }
    }
}

void StreamBaseClient::processCommand(std::string command_line) {
    /// Split command into words
    std::vector<std::string> command_words = Utility::split(command_line, ' ');
    if (command_words.size() < 1 || command_words.at(0).empty()) {
        return;
    }
    /// Process the command
    std::string command = command_words.at(0);
    if (command == "help") {
        std::cout << " attribs <class>                          List registered atrrib" << std::endl;
        std::cout << " call <class> <object> <method>           Call method" << std::endl;
        std::cout << " classes                                  List registered classes" << std::endl;
        std::cout << " create <class> <object>                  Create object" << std::endl;
        std::cout << " exit                                     Quits the client" << std::endl;
        std::cout << " get <class> <object> <attrib>            Retrieve attribute value" << std::endl;
        std::cout << " help                                     Show this screen" << std::endl;
        std::cout << " methods <class>                          List registered methods" << std::endl;
        std::cout << " mode [sync|async]                        Change communication mode" << std::endl;
        std::cout << " objects <class>                          List objects" << std::endl;
        std::cout << " recv                                     Receive raw data" << std::endl;
        std::cout << " remove <class> <object>                  Remove object" << std::endl;
        std::cout << " send <data>                              Send raw data" << std::endl;
        std::cout << " set <class> <object> <attrib> <value>    Set attribute value" << std::endl;
    }
    else if (command == "classes") {
        listClasses();
    }
    else if (command == "attribs") {
        if (command_words.size() < 2) {
            std::cout << "Usage: attribs <class>" << std::endl;
        }
        else {
            listAttribs(command_words.at(1));
        }
    }
    else if (command == "methods") {
        if (command_words.size() < 2) {
            std::cout << "Usage: methods <class>" << std::endl;
        }
        else {
            listMethods(command_words.at(1));
        }
    }
    else if (command == "objects") {
        if (command_words.size() < 2) {
            std::cout << "Usage: objects <class>" << std::endl;
        }
        else {
            listObjects(command_words.at(1));
        }
    }
    else if (command == "create") {
        if (command_words.size() < 3) {
            std::cout << "Usage: create <class> <object>" << std::endl;
        }
        else {
            createObject(command_words.at(1), command_words.at(2));
        }
    }
    else if (command == "remove") {
        if (command_words.size() < 3) {
            std::cout << "Usage: remove <class> <object>" << std::endl;
        }
        else {
            removeObject(command_words.at(1), command_words.at(2));
        }
    }
    else if (command == "call") {
        if (command_words.size() < 4) {
            std::cout << "Usage: call <class> <object> <method>" << std::endl;
        }
        else {
            callMethod(command_words.at(1), command_words.at(2), command_words.at(3));
        }
    }
    else if (command == "get") {
        if (command_words.size() < 4) {
            std::cout << "Usage: get <class> <object> <method>" << std::endl;
        }
        else {
            getAttrib(command_words.at(1), command_words.at(2), command_words.at(3));
        }
    }
    else if (command == "set") {
        if (command_words.size() < 5) {
            std::cout << "Usage: set <class> <object> <method> <value>" << std::endl;
        }
        else {
            setAttrib(command_words.at(1), command_words.at(2), command_words.at(3), command_words.at(4));
        }
    }
    else if (command == "send") {
        if (command_words.size() < 2) {
            std::cout << "Usage: send <data>" << std::endl;
        }
        sendRawData(command_words.at(1));
    }
    else if (command == "recv") {
        recvRawData();
    }
    else if (command == "mode") {
        if (command_words.size() >= 2) {
            /// Mode given as parameter - set it
            if (command_words.at(1) == "sync") {
                /// Set synchronous mode
                m_is_async = false;
                std::cout << "Mode set to synchronous" << std::endl;
            }
            else if (command_words.at(1) == "async") {
                /// Set asynchronous mode
                m_is_async = true;
                std::cout << "Mode set to asynchronous" << std::endl;
            }
            else {
                /// Invalid argument
                std::cout << "Usage: mode [sync|async]" << std::endl;
            }
        }
        else {
            /// Mode not given - just print current mode
            if (m_is_async) {
                std::cout << "Current mode is asynchronous" << std::endl;
            } else {
                std::cout << "Current mode is synchronous" << std::endl;
            }
        }

    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
    }
}

void StreamBaseClient::listClasses() {
    Request request(ActionId::ListClasses);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::string value;
    response.data().toValue(value);
    std::vector<std::string> list = Utility::split(value, '\0');
    for (const std::string& id : list) {
        std::cout << id << std::endl;
    }
}

void StreamBaseClient::listAttribs(const std::string& class_id) {
    Request request(ActionId::ListAttribs, class_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::string value;
    response.data().toValue(value);
    std::vector<std::string> list = Utility::split(value, '\0');
    for (const std::string& id : list) {
        std::cout << id << std::endl;
    }
}

void StreamBaseClient::listMethods(const std::string& class_id) {
    Request request(ActionId::ListMethods, class_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::string value;
    response.data().toValue(value);
    std::vector<std::string> list = Utility::split(value, '\0');
    for (const std::string& id : list) {
        std::cout << id << std::endl;
    }
}

void StreamBaseClient::listObjects(const std::string& class_id) {
    Request request(ActionId::ListObjects, class_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::string value;
    response.data().toValue(value);
    std::vector<std::string> list = Utility::split(value, '\0');
    for (const std::string& id : list) {
        std::cout << id << std::endl;
    }
}

void StreamBaseClient::createObject(const std::string& class_id, const std::string& object_id) {
    Request request(ActionId::CreateObject, class_id, object_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::cout << "Object `" << object_id << "` successfully created" << std::endl;
}

void StreamBaseClient::removeObject(const std::string& class_id, const std::string& object_id) {
    Request request(ActionId::RemoveObject, class_id, object_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::cout << "Object `" << object_id << "` successfully removed" << std::endl;
}

void StreamBaseClient::callMethod(const std::string& class_id, const std::string& object_id, const std::string& member_id) {
    Request request(ActionId::CallMethod, class_id, object_id, member_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::cout << "Member `" << member_id << "` successfully called" << std::endl;
}

void StreamBaseClient::getAttrib(const std::string& class_id, const std::string& object_id, const std::string& member_id) {
    Request request(ActionId::GetAttrib, class_id, object_id, member_id);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::string value;
    response.data().toValue(value);
    std::cout << "Attribute `" << member_id << "` is `" << value << "`" << std::endl;
}

void StreamBaseClient::setAttrib(const std::string& class_id, const std::string& object_id, const std::string& member_id, const std::string& value) {
    Request request(ActionId::SetAttrib, class_id, object_id, member_id, Utility::RawData::fromValue(value));
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::cout << "Attribute `" << member_id << "` set to `" << value << "`" << std::endl;
}

void StreamBaseClient::sendRawData(const std::string& data) {
    Request request(ActionId::SendRawData, Utility::RawData::fromValue(data));
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::cout << "Raw data successfully sent" << std::endl;
}

void StreamBaseClient::recvRawData() {
    Request request(ActionId::RecvRawData);
    Response response = m_pipe.request(request);
    if (response.resultId() != ResultId::Success) {
        std::cout << "Error: " << response.errorMessage() << std::endl;
        return;
    }
    std::string value;
    response.data().toValue(value);
    std::cout << "Raw data received (" << response.data().size() << " bytes):" << std::endl << value << std::endl;
}

}

