#include "NamedPipeServer.h"
#define NOMINMAX    /// To avoid `min` and `max` defines, allowing the use of `std::min` and `std::max` instead.
#include <array>
#include <string>
#include <algorithm>
#include <iostream>
#include <functional>
#include <thread>
#include <system_error>
#include <windows.h>
#include <fileapi.h>
#include "Utility/InputBuffer.h"
#include "Utility/OutputBuffer.h"
#include "Utility/StringHelpers.h"

namespace Server {

class NamedPipeServerPrivate {
public:
    NamedPipeServerPrivate(const std::string& pipename, NamedPipeServer& self);

    void run();

protected:
    void instanceThread(HANDLE handle);

private:
    NamedPipeServer& m_self;
    std::string m_pipename;

};

NamedPipeServer::NamedPipeServer(const std::string& pipename) :
    m_data(std::make_unique<NamedPipeServerPrivate>(pipename, *this)) {
}

NamedPipeServer::~NamedPipeServer() {}

void NamedPipeServer::run() {
    m_data->run();
}

NamedPipeServerPrivate::NamedPipeServerPrivate(const std::string& pipename, NamedPipeServer& self) :
    m_self(self), m_pipename(pipename) {
}

void NamedPipeServerPrivate::run() {
    /// Convert pipe name from Utf8 to Utf16
    std::wstring name_w = Utility::convertUtf8StringToWString(m_pipename);
    /// Get the necessary buffer length
    DWORD buffer_size = static_cast<DWORD>(std::max(Protocol::Request::maxLength(), Protocol::Response::maxLength()));
    /// Loop
    for (;;) {

        /// Create named pipe instance
        HANDLE handle = ::CreateNamedPipeW(
            name_w.c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS,
            PIPE_UNLIMITED_INSTANCES,
            buffer_size,
            buffer_size,
            0,
            NULL);
        if (handle == INVALID_HANDLE_VALUE) {
            DWORD err_num = ::GetLastError();
            throw std::system_error(std::error_code(err_num, std::system_category()));
        }
        /// Wait for client connection
        if (::ConnectNamedPipe(handle, NULL) != 0 || ::GetLastError() == ERROR_PIPE_CONNECTED) {
            std::cout << __func__ << ": client connected" << std::endl;
            try {
                std::thread instance_thread(&NamedPipeServerPrivate::instanceThread, std::ref(*this), handle);
                instance_thread.detach();
            }
            catch (...) {
                ::DisconnectNamedPipe(handle);
                ::CloseHandle(handle);
                throw;
            }
        }
        else {
            ::CloseHandle(handle);
        }
    }
}

void NamedPipeServerPrivate::instanceThread(HANDLE handle) {
    try {
        std::array<char, Protocol::Request::maxLength()> buffer;
        for (;;) {
            /// Read request
            DWORD bytes_readen = 0;
            BOOL read_success = ::ReadFile(handle, buffer.data(), static_cast<DWORD>(buffer.size()), &bytes_readen, NULL);
            if (!read_success || bytes_readen == 0) {
                if (::GetLastError() == ERROR_BROKEN_PIPE) {
                    std::cout << __func__ << ": client disconected" << std::endl;
                }
                else {
                    DWORD err_num = ::GetLastError();
                    throw std::system_error(std::error_code(err_num, std::system_category()));
                }
                break;
            }
            Utility::InputBuffer input_stream(bytes_readen, buffer.data());
            Protocol::Request request;
            input_stream >> request;
            /// Process the request
            Protocol::Response response = m_self.process(request);
            /// Send the response
            Utility::OutputBuffer output_stream;
            output_stream << response;
            DWORD bytes_written = 0;
            BOOL write_success = ::WriteFile(handle, output_stream.data(), static_cast<DWORD>(output_stream.size()), &bytes_written, NULL);
            if (!write_success || bytes_written != output_stream.size()) {
                DWORD err_num = ::GetLastError();
                throw std::system_error(std::error_code(err_num, std::system_category()));
            }
        }
    }
    catch (...) {
        ::FlushFileBuffers(handle);
        ::DisconnectNamedPipe(handle);
        ::CloseHandle(handle);
        throw;
    }
    ::FlushFileBuffers(handle);
    ::DisconnectNamedPipe(handle);
    ::CloseHandle(handle);
}

}