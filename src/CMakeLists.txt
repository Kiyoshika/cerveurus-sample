add_library(myserver_person Person.c)
target_include_directories(myserver_person PUBLIC 
	${Cerveurus_SOURCE_DIR}/include
	${CerveurusSample_SOURCE_DIR}/include
	${cJSON_SOURCE_DIR})
target_link_libraries(myserver_person cerveurus cjson)

add_executable(myserver myserver.c)
target_include_directories(myserver PUBLIC 
	${Cerveurus_SOURCE_DIR}/include
	${CerveurusSample_SOURCE_DIR}/include)
target_link_libraries(myserver myserver_person cerveurus)
