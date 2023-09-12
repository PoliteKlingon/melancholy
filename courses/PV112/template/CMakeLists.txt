################################################################################
# Common Framework for Computer Graphics Courses at FI MUNI.
#
# Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
# All rights reserved.
#
# Course: PV112 (Project Template)
################################################################################

# Generates the lecture.
visitlab_generate_lecture(PV112 project_template)

# Shared Library | Binary Form Only | Windows Only 
target_link_libraries(PV112_project_template 
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/libIrrKlang.so" 
) 

target_include_directories(PV112_project_template 
    PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include" 
)

add_custom_command(
    TARGET PV112_project_template
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/libIrrKlang.so" 
    "$<TARGET_FILE_DIR:PV112_project_template>/irrKlang.so"
)
add_custom_command(
    TARGET PV112_project_template
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/ikpFlac.so" 
    "$<TARGET_FILE_DIR:PV112_project_template>/ikpFlac.so"
)
add_custom_command(
    TARGET PV112_project_template
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    "${CMAKE_CURRENT_SOURCE_DIR}/ikpMP3.so" 
    "$<TARGET_FILE_DIR:PV112_project_template>/ikpMP3.so"
)