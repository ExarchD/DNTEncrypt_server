include_directories(../include)
set(HEADER_FILES ../include/objects.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
find_library(SQLITE3_LIBRARY sqlite3)
if(NOT SQLITE3_INCLUDE_DIR)
    message(SEND_ERROR "Failed to find sqlite3.h")
endif()
if(NOT SQLITE3_LIBRARY)
    message(SEND_ERROR "Failed to find the sqlite3 library")
endif()

add_library(sql sql.cpp)


add_executable(encrypt2_server  server.cxx ${HEADER_FILES} ${CMAKE_CXX_FLAGS})
target_compile_features(encrypt2_server PRIVATE cxx_range_for)
target_link_libraries(encrypt2_server sql ${SQLITE3_LIBRARY})
###

install(TARGETS encrypt2_server
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

