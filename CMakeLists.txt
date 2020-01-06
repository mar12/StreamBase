
cmake_minimum_required (VERSION 3.8)

project ("StreamBase")

if (MSVC)
    # avoid command line warning D9025 : overriding '/W3' with '/W4'
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

include_directories(".")

set(SHARED_SOURCES
    "Protocol/Request.cpp"
    "Protocol/Response.cpp"
    "Utility/InputBuffer.cpp"
    "Utility/OutputBuffer.cpp"
    "Utility/RawData.cpp"
    "Utility/StringHelpers.cpp"
)

set(SERVER_SOURCES
    "Server/NamedPipeServer.cpp"
    "Server/StreamBaseServer.cpp"
)

set(CLIENT_SOURCES
    "Client/NamedPipeClient.cpp"
    "Client/StreamBaseClient.cpp"
)

set(TEST_SOURCES
    "Tests/UnitTests.cpp"
    "Tests/TestRawData.cpp"
    "Tests/TestClassRegistry.cpp"
    "Tests/TestBuffers.cpp"
)

add_executable (StreamBaseServer ServerMain.cpp ${SERVER_SOURCES} ${SHARED_SOURCES})
add_executable (StreamBaseClient ClientMain.cpp ${CLIENT_SOURCES} ${SHARED_SOURCES})
add_executable (UnitTests ${SERVER_SOURCES} ${CLIENT_SOURCES} ${SHARED_SOURCES} ${TEST_SOURCES})
