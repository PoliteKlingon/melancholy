// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once
#include "glm_headers.hpp"
#include "gui_application.h"
#include "program.hpp"

class OpenGLApplication : public GUIApplication {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
  protected:
    /** The default lit program. */
    ShaderProgram default_lit_program;
    /** The default unlit program. */
    ShaderProgram default_unlit_program;
    /** The absolute path to framework's shaders. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path framework_shaders_path;
    /** The absolute path to framework's textures. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path framework_textures_path;
    /** The absolute path to application shaders. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path lecture_shaders_path;
    /** The absolute path to application textures. Loaded from {@link configuration} if a configuration file is available. */
    std::filesystem::path lecture_textures_path;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link OpenGLApplication with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The command line arguments used to obtain the application directory.
     */
    OpenGLApplication(int initial_width, int initial_height, std::vector<std::string> arguments);

    /** The destructor that releases all resources. */
    virtual ~OpenGLApplication();

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------
  public:
    /**
     * Activates a custom frame buffer. The methods also automatically sets the full screen size viewport and possibly
     * also clears the buffer and sets the depth test.
     *
     * @param 	buffer	  	The buffer to activate (0 is the main screen).
     * @param 	clear	  	If @p true the bounded frame buffer will be cleared.
     * @param 	depth_test	if @p true the depth test will be activated; if @p false the depth test will be deactivated.
     */
    static void activate_render_buffer(GLint buffer = 0, bool clear = true, bool depth_test = true);
};
