find_package(protobuf CONFIG REQUIRED)

add_library(proto_messages STATIC)

target_link_libraries(proto_messages PUBLIC protobuf::libprotoc protobuf::libprotobuf)

protobuf_generate(
        TARGET proto_messages
        PROTOS
            ${CMAKE_CURRENT_SOURCE_DIR}/messages.proto
)

target_include_directories(proto_messages PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)