#include "NamedPipeClient.h"
#include <array>
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <windows.h>
#include "Utility/InputBuffer.h"
#include "Utility/OutputBuffer.h"
#include "Utility/StringHelpers.h"

namespace Client {

class NamedPipeClientPrivate {
public:
    explicit NamedPipeClientPrivate(const std::string& pipename);
    ~NamedPipeClientPrivate();

    Protocol::Response request(const Protocol::Request& request);

private:
    HANDLE     m_handle;
    std::mutex m_mutex;

};


NamedPipeClient::NamedPipeClient(const std::string& pipename) :
    m_data(std::make_unique<NamedPipeClientPrivate>(pipename)) {
}

NamedPipeClient::~NamedPipeClient() {}

Protocol::Response NamedPipeClient::request(const Protocol::Request& request) {
    return m_data->request(request);
}


NamedPipeClientPrivate::NamedPipeClientPrivate(const std::string& pipename) {

    std::wstring name_w = Utility::convertUtf8StringToWString(pipename);

    for (;;) {

        m_handle = ::CreateFileW(
            name_w.c_str(),   // pipe name
            GENERIC_READ | GENERIC_WRITE, // read and write access
            0,              // no sharing
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe
            0,              // default attributes
            NULL);          // no template file

        if (m_handle != INVALID_HANDLE_VALUE) {
            /// We've opened the pipe
            break;
        }

        DWORD err_num = ::GetLastError();
        if (err_num != ERROR_PIPE_BUSY) {
            throw std::system_error(std::error_code(err_num, std::system_category()));
        }

        static constexpr int timeout_s = 60;
        std::cout << "Pipe is busy - waiting for " << timeout_s << " s" << std::endl;

        BOOL pipe_available = ::WaitNamedPipeW(name_w.c_str(), timeout_s * 1000);

        if (!pipe_available) {
            throw std::system_error(std::error_code(err_num, std::system_category()));
        }

    }

    DWORD mode = PIPE_READMODE_MESSAGE;
    BOOL mode_set = ::SetNamedPipeHandleState(
        m_handle,   // pipe handle
        &mode,      // new pipe mode
        NULL,       // don't set maximum bytes
        NULL);      // don't set maximum time

    if (!mode_set) {
        DWORD err_num = ::GetLastError();
        throw std::system_error(std::error_code(err_num, std::system_category()));
    }

}

Protocol::Response NamedPipeClientPrivate::request(const Protocol::Request& request) {
    /// Only one thread at time can do the request. This is to prevent responses mix-up,
    /// as in case of simulaneous requests there would be no guarantee regarding receiving
    /// the response for the exact given response
    std::lock_guard<std::mutex> lock(m_mutex);
    /// Send request
    Utility::OutputBuffer output;
    output << request;
    DWORD bytes_written = 0;
    BOOL write_success = WriteFile(m_handle, output.data(), static_cast<DWORD>(output.size()), &bytes_written, NULL);
    if (!write_success) {
        DWORD err_num = ::GetLastError();
        throw std::system_error(std::error_code(err_num, std::system_category()));
    }

    /// Read response
    Utility::InputBuffer input;
    for (;;) {
        std::array<char, Protocol::Response::maxLength()> read_buffer;
        DWORD bytes_read = 0;
        BOOL read_success = ReadFile(m_handle, &read_buffer, static_cast<DWORD>(read_buffer.size()), &bytes_read, NULL);
        if (read_success) {
            input.append(static_cast<std::size_t>(bytes_read), read_buffer.data());
            break;
        }
        DWORD err_num = ::GetLastError();
        if (err_num != ERROR_MORE_DATA) {
            throw std::system_error(std::error_code(err_num, std::system_category()));
        }
    }

    Protocol::Response response;
    input >> response;
    return response;
}

NamedPipeClientPrivate::~NamedPipeClientPrivate() {
    std::lock_guard<std::mutex> lock(m_mutex);
    ::CloseHandle(m_handle);
}

}