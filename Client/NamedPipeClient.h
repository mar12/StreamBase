/**
 * Wrapper class for NamedPipe API client part
 *
 * Allows conducting requests to server and recieving the responses for these requests
 */
#ifndef CLIENT_NAMEDPIPECLIENT_H_
#define CLIENT_NAMEDPIPECLIENT_H_
#include <memory>
#include "Protocol/Request.h"
#include "Protocol/Response.h"

namespace Client {

class NamedPipeClientPrivate;

class NamedPipeClient {
public:
    explicit NamedPipeClient(const std::string& pipename);
    ~NamedPipeClient();

    Protocol::Response request(const Protocol::Request& request);

private:
    std::unique_ptr<NamedPipeClientPrivate> m_data;

};

}

#endif // CLIENT_NAMEDPIPECLIENT_H_