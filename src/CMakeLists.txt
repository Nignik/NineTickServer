file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(Server ${SOURCES})

find_package(boost_asio CONFIG REQUIRED)
find_package(boost_beast CONFIG REQUIRED)
find_package(boost_json CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

target_link_libraries(Server PRIVATE
        Boost::asio Boost::beast Boost::json
        proto_messages
)