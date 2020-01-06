/**
 * StreamBase server main function
 *
 * Registers the `SampleClassTimer` class, then processes the command line argument,
 * and, finally, runs the `Server::StreamBaseServer` server.
 *
 */
#include <string>
#include <iostream>
#include "Server/StreamBaseServer.h"
#include "ClassRegistry/ClassesPool.h"
#include "Tests/SampleClassTimer.h"

using namespace ClassRegistry;
using namespace Server;
using namespace Tests;

static void registerSampleClassTimer() {
    ClassesPool& pool = ClassesPool::instance();

    std::string test_class_id = "SampleClassTimer";
    auto& registered_class = pool.addClass<SampleClassTimer>(test_class_id);

    registered_class.addAttrib("count", &SampleClassTimer::m_count);
    registered_class.addAttrib("enabled", &SampleClassTimer::m_enabled);

    registered_class.addMethod("start", &SampleClassTimer::start);
    registered_class.addMethod("stop", &SampleClassTimer::stop);
    registered_class.addMethod("tick", &SampleClassTimer::tick);
    registered_class.addMethod("reset", &SampleClassTimer::reset);
}

static void registerClasses() {
    registerSampleClassTimer();
}

int main(int argc, char* argv[]) {

    registerClasses();

    std::string pipename = "\\\\.\\pipe\\streambase";

    std::cout << "StreamBaseServer v1.0.0" << std::endl;
    std::cout << "(c) Mariian Berezovskyi, 2020 <mariyan.berezovskyy@gmail.com>" << std::endl;

    if (argc > 2) {
        std::cerr << "Error: Invalid command-line arguments";
    }
    else if (argc == 2) {
        /// Second argument goes for named pipe path
        if (argv[1][0] == '/') {
            std::cout << std::endl;
            std::cout << "Usage:  StreamBaseServer [Pipename]" << std::endl;
            std::cout << std::endl;
            return 0;
        }
        else {
            pipename = std::string(argv[1]);
        }
    }

    try {
        StreamBaseServer server(pipename);
        server.run();
    }
    catch (std::system_error & e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
