/**
 * StreamBase client main function
 *
 * Processes the command line argument and runs the `Client::StreamBaseClient` client.
 *
 */
#include <iostream>
#include <string>
#include "Client/StreamBaseClient.h"

int main(int argc, char* argv[]) {

    std::string pipename = "\\\\.\\pipe\\streambase";

    std::cout << "StreamBaseClient v1.0.0" << std::endl;
    std::cout << "(c) Mariian Berezovskyi, 2020 <mariyan.berezovskyy@gmail.com>" << std::endl;

    if (argc > 2) {
        std::cerr << "Error: Invalid command-line arguments";
    }
    else if (argc == 2) {
        /// Second argument goes for the named pipe path
        if (argv[1][0] == '/') {
            std::cout << std::endl;
            std::cout << "Usage:  StreamBaseClient [Pipename]" << std::endl;
            std::cout << std::endl;
            return 0;
        }
        else {
            pipename = std::string(argv[1]);
        }
    }

    try {
        Client::StreamBaseClient client(pipename);
        client.runInteractively();
    }
    catch (std::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}