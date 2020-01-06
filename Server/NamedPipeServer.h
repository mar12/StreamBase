/**
 * Wrapper class for NamedPipe API server part
 *
 * Allows conducting requests to server and recieving the responses for these requests
 *
 */
#ifndef SERVER_NAMEDPIPESERVER_H_
#define SERVER_NAMEDPIPESERVER_H_
#include <memory>
#include <string>
#include "Protocol/Request.h"
#include "Protocol/Response.h"

namespace Server {

class NamedPipeServerPrivate;

class NamedPipeServer {
public:
    explicit NamedPipeServer(const std::string& pipename);
    virtual ~NamedPipeServer();

    void run();

    virtual Protocol::Response process(const Protocol::Request& request) = 0;

private:
    std::unique_ptr<NamedPipeServerPrivate> m_data;

};

}

#endif // SERVER_NAMEDPIPESERVER_H_