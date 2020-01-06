/**
 * Unit test application
 *
 * Currently, it covers only a few classes.
 *
 * @todo Cover more code with tests
 * @todo Use `CppUnit` or smth like this
 *
 */
#include <iostream>
#include "TestRawData.h"
#include "TestClassRegistry.h"
#include "TestBuffers.h"

int main() {
	std::cout << "Running tests..." << std::endl;

	bool success = true;

	if (!Tests::testRawData()) {
		success = false;
	}

	if (!Tests::testClassRegistry()) {
		success = false;
	}

	if (!Tests::testBuffers()) {
		success = false;
	}

	if (!success) {
		std::cout << "Testing failed!" << std::endl;
		return 1;
	}
	else {
		std::cout << "Tests ran successfully" << std::endl;
		return 0;
	}

}
