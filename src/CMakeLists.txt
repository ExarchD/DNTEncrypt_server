include_directories(../include)
set(HEADER_FILES ../include/sql.h)


find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
find_library(SQLITE3_LIBRARY sqlite3)
if(NOT SQLITE3_INCLUDE_DIR)
    message(SEND_ERROR "Failed to find sqlite3.h")
endif()
if(NOT SQLITE3_LIBRARY)
    message(SEND_ERROR "Failed to find the sqlite3 library")
endif()

add_library(sql sql.cpp)

add_executable(DNTEncrypt_server  server.cxx ${HEADER_FILES})
target_compile_features(DNTEncrypt_server PRIVATE cxx_range_for)
target_link_libraries(DNTEncrypt_server sql ${SQLITE3_LIBRARY})
###

install(TARGETS DNTEncrypt_server
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

