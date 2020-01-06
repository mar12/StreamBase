/**
 * The StreamBase client main class
 *
 * Uses `NamedPipeClient` for low-level API
 */
#ifndef CLIENT_STREAMBASECLIENT_H_
#define CLIENT_STREAMBASECLIENT_H_
#include <atomic>
#include "NamedPipeClient.h"

namespace Client {

class StreamBaseClient {
public:
    StreamBaseClient(const std::string& pipename);

    void runInteractively();

protected:
    void processCommand(std::string command_line);

    void listClasses();

    void listAttribs(const std::string& class_id);
    void listMethods(const std::string& class_id);
    void listObjects(const std::string& class_id);

    void createObject(const std::string& class_id, const std::string& object_id);
    void removeObject(const std::string& class_id, const std::string& object_id);

    void callMethod(const std::string& class_id, const std::string& object_id, const std::string& member_id);
    void getAttrib (const std::string& class_id, const std::string& object_id, const std::string& member_id);
    void setAttrib (const std::string& class_id, const std::string& object_id, const std::string& member_id, const std::string& value);

    void sendRawData(const std::string& data);
    void recvRawData();


    NamedPipeClient   m_pipe;
    std::atomic<bool> m_is_async;
};

}

#endif // CLIENT_STREAMBASECLIENT_H_