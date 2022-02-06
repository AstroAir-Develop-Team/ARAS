#Websocketpp用于网页端通信
find_path(PathWs server.hpp /usr/include/websocketpp)
find_path(PathWs server.hpp /usr/local/include/websocketpp)
if(PathWs)
    message("-- Found Websocketpp Header File in ${PathWs}")
else()
    message("-- Countn't Found Websocketpp Header File")
endif()

#图形界面，决定使用较为简单的IMGUI
#add_library(Gui src/core/gui.cpp)

#target_link_directories(Gui PUBLIC includes/imgui)
#target_include_directories(Gui PUBLIC includes/imgui)
#target_link_libraries(aras PUBLIC 