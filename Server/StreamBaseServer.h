/**
 * The StreamBase server main class
 *
 * Uses `NamedPipeServer` for low-level API
 */
#ifndef SERVER_STREAMBASESERVER_H_
#define SERVER_STREAMBASESERVER_H_
#include <iostream>
#include "NamedPipeServer.h"
#include "Utility/RawData.h"

namespace Server {

class StreamBaseServer : public NamedPipeServer {
public:
    explicit StreamBaseServer(const std::string& pipename);

    Protocol::Response process(const Protocol::Request&) override;

    const Utility::RawData& lastRawData() const {
        return m_last_raw_data;
    }

private:
    Utility::RawData m_last_raw_data;

};

}

#endif // SERVER_STREAMBASESERVER_H_