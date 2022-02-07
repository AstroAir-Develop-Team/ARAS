#Websocketpp用于网页端通信
find_path(PathWs server.hpp /usr/include/websocketpp)
find_path(PathWs server.hpp /usr/local/include/websocketpp)
if(PathWs)
    message("-- Found Websocketpp Header File in ${PathWs}")
else()
    message("-- Countn't Found Websocketpp Header File")
endif()

#图形界面，决定使用IMGUI
find_path(path_glfw glfw3.h /usr/include/GLFW)
find_path(path_glfw glfw3.h /usr/local/include/GLFW)
if(path_glfw)
    message("-- Found GL header file in ${IMGUI_GL}")
else()
    message("-- Could not found GL library.You should run 'sudo apt install libglfw3-dev libgl-dev'")
endif()


