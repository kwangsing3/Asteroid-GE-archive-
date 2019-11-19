# CMakeList.txt: CMakeProject1 的 CMake 專案，在此包含來源及定義
# 專案專屬邏輯。
#
cmake_minimum_required (VERSION 3.10)


project(Asteroid-GameEngine VERSION 1.0)



#--------------------------------------------------------------------
# Add Library
#--------------------------------------------------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glfw-3.3)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/glm)


set(CMAKE_CXX_STANDARD 17)

##設定頭文件(.h)位置

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/lib/glm)

#link_directories(${CMAKE_SOURCE_DIR}/lib)

# 添加目錄下所有的來源檔(.Cpp)並新增至DIR_SRCS變量
aux_source_directory(${CMAKE_SOURCE_DIR}/src DIR_SRCS)


# 將來源新增到此專案的可執行檔。
add_executable (${PROJECT_NAME} ${DIR_SRCS})




#--------------------------------------------------------------------
# Add GLFW-Lib
#--------------------------------------------------------------------

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_glu_LIBRARY} ${GLFW_LIBRARIES})

#--------------------------------------------------------------------
# Copy Folder
#--------------------------------------------------------------------
add_custom_command(
         TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/Shader $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shader
         COMMENT "Copying Shader" VERBATIM
         )
		 add_custom_command(
         TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND ${CMAKE_COMMAND} -E copy_directory              
             ${CMAKE_SOURCE_DIR}/src/Texture $<TARGET_FILE_DIR:${PROJECT_NAME}>/Texture
         COMMENT "Copying Texture" VERBATIM
         )