// ################################################################################
// Common Framework for Computer Graphics Courses at FI MUNI.
//
// Copyright (c) 2021-2022 Visitlab (https://visitlab.fi.muni.cz)
// All rights reserved.
// ################################################################################

#pragma once

#include "camera.h"
#include "cube.hpp"
#include "pv112_application.hpp"
#include "sphere.hpp"
#include "teapot.hpp"

#include "irrKlang.h"

using namespace irrklang;

// ----------------------------------------------------------------------------
// UNIFORM STRUCTS
// ----------------------------------------------------------------------------
struct CameraUBO {
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 position;
};

struct LightUBO {
    glm::vec4 position;
    glm::vec4 ambient_color;
    glm::vec4 diffuse_color;
    glm::vec4 specular_color;
};

struct alignas(256) ObjectUBO {
    glm::mat4 model_matrix;  // [  0 -  64) bytes
    glm::vec4 ambient_color; // [ 64 -  80) bytes
    glm::vec4 diffuse_color; // [ 80 -  96) bytes

    // Contains shininess in .w element
    glm::vec4 specular_color; // [ 96 - 112) bytes
};

struct alignas(256) ObjectPaddedUBO {
    glm::mat4 model_matrix;  // [  0 -  64) bytes
    glm::vec4 ambient_color; // [ 64 -  80) bytes
    glm::vec4 diffuse_color; // [ 80 -  96) bytes

    // Contains shininess in .w element
    glm::vec4 specular_color; // [ 96 - 112) bytes
    glm::vec4 padding;
};

// Constants
const float clear_color[4] = {0.0, 0.0, 0.0, 1.0};
const float clear_depth[1] = {1.0};

class Application : public PV112Application {

    // ----------------------------------------------------------------------------
    // Variables
    // ----------------------------------------------------------------------------
    std::filesystem::path images_path;
    std::filesystem::path objects_path;

    // Main program
    GLuint main_program;
    GLuint rain_program;
    // TODO: feel free to add as many as you need/like

    // List of geometries used in the project
    std::vector<std::shared_ptr<Geometry>> geometries;
    // Shared pointers are pointers that automatically count how many times they are used. When there are 0 pointers to the object pointed
    // by shared_ptrs, the object is automatically deallocated. Consequently, we gain 3 main properties:
    // 1. Objects are not unnecessarily copied
    // 2. We don't have to track pointers
    // 3. We don't have to deallocate these geometries
    Cube instantiable_cube{};
    std::shared_ptr<Geometry> cube;
    std::shared_ptr<Geometry> sphere;
    std::shared_ptr<Geometry> lamp;
    std::shared_ptr<Geometry> lamp_glass;
    std::shared_ptr<Geometry> bench_frame;
    std::shared_ptr<Geometry> bench_wood;
    std::shared_ptr<Geometry> sign;
    std::shared_ptr<Geometry> pickup_karoserie;
    std::shared_ptr<Geometry> pickup_okna;
    std::shared_ptr<Geometry> pickup_kola;
    std::shared_ptr<Geometry> pickup_podvozek;
    std::shared_ptr<Geometry> pickup_svetla;
    std::shared_ptr<Geometry> pickup_seda;
    std::shared_ptr<Geometry> ford_karoserie;
    std::shared_ptr<Geometry> ford_okna;
    std::shared_ptr<Geometry> ford_sklo;
    std::shared_ptr<Geometry> ford_podvozek;
    std::shared_ptr<Geometry> ford_chrom;
    std::shared_ptr<Geometry> ford_interior;
    std::shared_ptr<Geometry> inner_lights;
    std::shared_ptr<Geometry> outer_lights;
    std::shared_ptr<Geometry> taillights;
    std::shared_ptr<Geometry> turn_signals;
    std::shared_ptr<Geometry> building_left;
    std::shared_ptr<Geometry> building_right_center;
    std::shared_ptr<Geometry> building_right_front;
    std::shared_ptr<Geometry> building_right_back;
    std::shared_ptr<Geometry> building_far;
    std::shared_ptr<Geometry> wall_front;
    std::shared_ptr<Geometry> wall_back;
    std::shared_ptr<Geometry> building1_brick;
    std::shared_ptr<Geometry> building1_ac;
    std::shared_ptr<Geometry> building1_door;
    std::shared_ptr<Geometry> building1_door2;
    std::shared_ptr<Geometry> building1_store;
    std::shared_ptr<Geometry> building1_top;
    std::shared_ptr<Geometry> building1_w2;
    std::shared_ptr<Geometry> building1_wf;
    std::shared_ptr<Geometry> building1_window1;
    std::shared_ptr<Geometry> building1_window2;
    std::shared_ptr<Geometry> building1_window3;
    std::shared_ptr<Geometry> building1_window2_lit;
    std::shared_ptr<Geometry> building1_window3_lit;
    std::shared_ptr<Geometry> building1_ramy;
    std::shared_ptr<Geometry> building1_vydech;
    std::shared_ptr<Geometry> building2_brick;
    std::shared_ptr<Geometry> building2_door;
    std::shared_ptr<Geometry> building2_top;
    std::shared_ptr<Geometry> building2_window;
    std::shared_ptr<Geometry> building3_brick;
    std::shared_ptr<Geometry> building3_window;
    std::shared_ptr<Geometry> building3_window_lit;
    std::shared_ptr<Geometry> building3_stairs;
    std::shared_ptr<Geometry> building3_frames;
    std::shared_ptr<Geometry> building4_brick;
    std::shared_ptr<Geometry> building4_frames;
    std::shared_ptr<Geometry> building4_window;
    std::shared_ptr<Geometry> building4_side_window;
    std::shared_ptr<Geometry> building4_top;
    std::shared_ptr<Geometry> building4_door;
    std::shared_ptr<Geometry> building5_brick;
    std::shared_ptr<Geometry> building5_window;
    std::shared_ptr<Geometry> building5_window_lit;
    std::shared_ptr<Geometry> building5_window_inactive;
    std::shared_ptr<Geometry> building5_paint;
    

    // Default camera that rotates around center.
    Camera camera;

    glm::vec3 camera_center = glm::vec3(0.0f);
    bool sprint = false;
    bool crunch = false;
    

    // UBOs
    GLuint camera_buffer = 0;
    CameraUBO camera_ubo;

    GLuint light_buffer = 0;
    LightUBO light_ubo;

    GLuint objects_buffer = 0;
    std::vector<ObjectUBO> objects_ubos;

    GLuint lamps_buffer = 0;
    std::vector<ObjectUBO> lamps_ubos;

    GLuint lamps_glass_buffer = 0;
    std::vector<ObjectUBO> lamps_glass_ubos;

    GLuint benches_buffer = 0;
    std::vector<ObjectUBO> benches_ubos;

    bool blink = false;
    int counter = 0;
    float moving_factor = 0.0f;
    int rain_density = 1024;
    bool inside_car = false;
    glm::vec3 last_camera_pos = glm::vec3(0.0f);

    ISoundEngine* engine = NULL;

    // Lights
    std::vector<LightUBO> lights;
    GLuint lights_buffer = 0;

    std::vector<ObjectPaddedUBO> instanced_objects;
    GLuint instanced_objects_buffer = 0;
    std::vector<float> rain_x_positions;
    std::vector<float> rain_y_positions;
    std::vector<float> rain_z_positions;

    // Textures
    GLuint wood_texture = 0;
    GLuint asphalt_texture = 0;
    GLuint asphalt_NM = 0;
    GLuint asphalt_tilted_texture = 0;
    GLuint asphalt_tilted_NM = 0;
    GLuint pavement_texture = 0;
    GLuint pavement_NM = 0;
    GLuint black_metal_texture = 0;
    GLuint building1_brick_texture = 0;
    GLuint building1_brick_NM = 0;
    GLuint building1_ac_texture = 0;
    GLuint building1_door_texture = 0;
    GLuint building1_door2_texture = 0;
    GLuint building1_store_texture = 0;
    GLuint building1_store_NM = 0;
    GLuint building1_top_texture = 0;
    GLuint building1_w2_texture = 0;
    GLuint building1_wf_texture = 0;
    GLuint building1_window1_texture = 0;
    GLuint building1_window2_texture = 0;
    GLuint building1_window3_texture = 0;
    GLuint building2_brick_texture = 0;
    GLuint building2_door_texture = 0;
    GLuint building2_top_texture = 0;
    GLuint building2_window_texture = 0;
    GLuint building5_brick_texture = 0;
    GLuint building5_window_texture = 0;
    GLuint building5_window_dark_texture = 0;
    GLuint building3_brick_texture = 0;
    GLuint building3_brick_NM = 0;
    GLuint glass_texture = 0;
    GLuint building4_brick_texture = 0;
    GLuint building4_brick_NM = 0;
    GLuint building4_frame_texture = 0;
    GLuint building4_window_texture = 0;
    GLuint building4_side_window_texture = 0;
    GLuint building4_door_texture = 0;
    GLuint building4_top_texture = 0;
    GLuint sign_texture = 0;
    GLuint moon_texture = 0;
    GLuint sky_texture = 0;
    GLuint rain_texture = 0;

    // ----------------------------------------------------------------------------
    // Constructors & Destructors
    // ----------------------------------------------------------------------------
  public:
    /**
     * Constructs a new @link Application with a custom width and height.
     *
     * @param 	initial_width 	The initial width of the window.
     * @param 	initial_height	The initial height of the window.
     * @param 	arguments	  	The command line arguments used to obtain the application directory.
     */
    Application(int initial_width, int initial_height, std::vector<std::string> arguments = {});

    /** Destroys the {@link Application} and releases the allocated resources. */
    ~Application() override;

    // ----------------------------------------------------------------------------
    // Methods
    // ----------------------------------------------------------------------------

    /** @copydoc PV112Application::compile_shaders */
    void compile_shaders() override;

    /** @copydoc PV112Application::delete_shaders */
    void delete_shaders() override;

    /** @copydoc PV112Application::update */
    void update(float delta) override;

    /** @copydoc PV112Application::render */
    void render() override;

    /** @copydoc PV112Application::render_ui */
    void render_ui() override;

    // ----------------------------------------------------------------------------
    // Input Events
    // ----------------------------------------------------------------------------

    /** @copydoc PV112Application::on_resize */
    void on_resize(int width, int height) override;

    /** @copydoc PV112Application::on_mouse_move */
    void on_mouse_move(double x, double y) override;

    /** @copydoc PV112Application::on_mouse_button */
    void on_mouse_button(int button, int action, int mods) override;

    /** @copydoc PV112Application::on_key_pressed */
    void on_key_pressed(int key, int scancode, int action, int mods) override;
};
