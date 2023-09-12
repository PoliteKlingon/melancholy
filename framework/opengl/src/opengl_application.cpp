// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
// 
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#include "opengl_application.hpp"

// ----------------------------------------------------------------------------
// Constructors
// ----------------------------------------------------------------------------
OpenGLApplication::OpenGLApplication(int initial_width, int initial_height, std::vector<std::string> arguments)
    : GUIApplication(initial_width, initial_height, arguments) {
    framework_shaders_path = framework_folder_path / "opengl" / "shaders";
    framework_textures_path = framework_folder_path / "opengl" / "textures";
    lecture_shaders_path = lecture_folder_path / "shaders";
    lecture_textures_path = lecture_folder_path / "textures";

    /** The default unlit program. */
    default_unlit_program = ShaderProgram(framework_shaders_path / "object_330.vert", framework_shaders_path / "unlit.frag");
}

OpenGLApplication::~OpenGLApplication() = default;

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void OpenGLApplication::activate_render_buffer(GLint buffer, bool clear, bool depth_test) {
    // Binds the frame buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);

    // Clears the color and depth attachments (this is also fine if there is no color attachment).
    if (clear) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    // Activates (or deactivates) the depth test.
    if (depth_test) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}
