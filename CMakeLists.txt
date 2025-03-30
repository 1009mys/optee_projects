project (optee_my_gpio C)

set (SRC host/main.c)

add_executable (${PROJECT_NAME} ${SRC})

# add the include directories
include_directories(/tmp/optee_client/out/export/usr/include)

target_include_directories(${PROJECT_NAME}
			   PRIVATE ta/include
			   PRIVATE include)

target_link_libraries (${PROJECT_NAME} PRIVATE teec)

install (TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
