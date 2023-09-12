#include "application.hpp"

#include <memory>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "irrKlang.h"

using std::make_shared;
using namespace irrklang;

GLuint load_texture_2d(const std::filesystem::path filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.generic_string().data(), &width, &height, &channels, 4);

    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(texture, std::log2(width), GL_RGBA8, width, height);

    glTextureSubImage2D(texture,
                        0,                         //
                        0, 0,                      //
                        width, height,             //
                        GL_RGBA, GL_UNSIGNED_BYTE, //
                        data);

    stbi_image_free(data);

    glGenerateTextureMipmap(texture);

    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

float random_pos() { return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) + 1.0f); }

float random_neg() { return rand() / (static_cast<float>(RAND_MAX) + 1.0f) * 2.0f - 1.0f; }

Application::Application(int initial_width, int initial_height, std::vector<std::string> arguments)
    : PV112Application(initial_width, initial_height, arguments) {
    this->width = initial_width;
    this->height = initial_height;
    
    images_path = lecture_folder_path / "images";
    objects_path = lecture_folder_path / "objects";

    // --------------------------------------------------------------------------
    //  Load/Create Objects
    // --------------------------------------------------------------------------

    geometries.push_back(make_shared<Cube>());
    cube = geometries[0];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "lamp.obj")));                     
    lamp = geometries[1];                                                                                                

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "sign.obj")));                     
    sign = geometries[2];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bench_frame.obj")));                     
    bench_frame = geometries[3];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building_far.obj")));                     
    building_far = geometries[4];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building_left.obj")));                     
    building_left = geometries[5];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building_right_center.obj")));
    building_right_center = geometries[6];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building_right_back.obj")));                     
    building_right_back = geometries[7];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building_right_front.obj")));
    building_right_front = geometries[8];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "wall_front.obj")));                                         
    wall_front = geometries[9];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "wall_back.obj")));                     
    wall_back = geometries[10];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "pickup_karoserie.obj")));                     
    pickup_karoserie = geometries[11];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "ford_karoserie.obj")));                     
    ford_karoserie = geometries[12];                                                                                                    

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "ford_podvozek.obj")));                     
    ford_podvozek = geometries[13];                                                             

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "ford_sklo.obj")));                     
    ford_sklo = geometries[14];                                                             

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "ford_okna.obj")));                     
    ford_okna = geometries[15];                                                             

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "ford_chrom.obj")));                     
    ford_chrom = geometries[16];                                                             
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "inner_lights.obj")));                     
    inner_lights = geometries[17];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "outer_lights.obj")));                     
    outer_lights = geometries[18];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "taillights.obj")));                     
    taillights = geometries[19];                                                                                                    
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "turn_signals.obj")));                     
    turn_signals = geometries[20];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "lamp_glass.obj")));                     
    lamp_glass = geometries[21];                                                                                                    

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "bench_wood.obj")));                     
    bench_wood = geometries[22];                                                                                                          

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "pickup_okna.obj")));                     
    pickup_okna = geometries[23];                                                                                                    

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "pickup_kola.obj")));                     
    pickup_kola = geometries[24];                                                                                                    

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "pickup_seda.obj")));                     
    pickup_seda = geometries[25];                                                                                                    

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "pickup_podvozek.obj")));                     
    pickup_podvozek = geometries[26];                                                                                                    

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "pickup_svetla.obj")));                     
    pickup_svetla = geometries[27];     

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "interior.obj")));                     
    ford_interior = geometries[28]; 

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/cihly.obj")));                     
    building1_brick = geometries[29];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/ac.obj")));                     
    building1_ac = geometries[30];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/door.obj")));                     
    building1_door = geometries[31];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/door2.obj")));                     
    building1_door2 = geometries[32];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/ramy.obj")));                     
    building1_ramy = geometries[33];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/store.obj")));                     
    building1_store = geometries[34];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/top.obj")));                     
    building1_top = geometries[35];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/vydech.obj")));                     
    building1_vydech = geometries[36];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/w2.obj")));                     
    building1_w2 = geometries[37];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/wf.obj")));                     
    building1_wf = geometries[38];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/window1.obj")));                     
    building1_window1 = geometries[39];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/window2.obj")));                     
    building1_window2 = geometries[40];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/window3.obj")));                     
    building1_window3 = geometries[41];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/window2_lit.obj")));                     
    building1_window2_lit = geometries[42];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building1/window3_lit.obj")));                     
    building1_window3_lit = geometries[43];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building5/okna_lit.obj")));                     
    building5_window_lit = geometries[44];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building5/okna.obj")));                     
    building5_window = geometries[45];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building5/okna_inactive.obj")));                     
    building5_window_inactive = geometries[46];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building5/cihly.obj")));                     
    building5_brick = geometries[47];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building5/paint.obj")));                     
    building5_paint = geometries[48];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building3/zdi.obj")));
    building3_brick = geometries[49];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building3/okna.obj")));
    building3_window = geometries[50];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building3/okna_lit.obj")));
    building3_window_lit = geometries[51];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building3/schody.obj")));
    building3_stairs = geometries[52];
    
    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building3/ramy.obj")));
    building3_frames = geometries[53];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building4/cihly.obj")));
    building4_brick = geometries[54];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building4/kraje.obj")));
    building4_frames = geometries[55];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building4/okna.obj")));
    building4_window = geometries[56];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building4/bocni_okna.obj")));
    building4_side_window = geometries[57];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building4/top.obj")));
    building4_top = geometries[58];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building4/dvere.obj")));
    building4_door = geometries[59];

    geometries.push_back(make_shared<Sphere>());
    sphere = geometries[60];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building2/brick.obj")));
    building2_brick = geometries[61];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building2/door.obj")));
    building2_door = geometries[62];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building2/top.obj")));
    building2_top = geometries[63];

    geometries.push_back(make_shared<Geometry>(Geometry::from_file(objects_path / "building2/window.obj")));
    building2_window = geometries[64];


    wood_texture = load_texture_2d(images_path / "wood.jpg");
    asphalt_texture = load_texture_2d(images_path / "asphalt_striped.jpg");
    asphalt_NM = load_texture_2d(images_path /      "asphalt_striped_NM.png");
    asphalt_tilted_texture = load_texture_2d(images_path / "asphalt_endless_tilted.png");
    asphalt_tilted_NM = load_texture_2d(images_path /      "asphalt_endless_tilted_NM.png");
    pavement_texture = load_texture_2d(images_path / "chodnik.jpg");
    pavement_NM = load_texture_2d(images_path /      "chodnik_NM.png");
    black_metal_texture = load_texture_2d(images_path / "black_metal.jpg");
    building1_brick_texture = load_texture_2d(images_path /   "building1/brick.jpg");
    building1_brick_NM = load_texture_2d(images_path /        "building1/brickNM.png");
    building1_ac_texture = load_texture_2d(images_path /      "building1/ac.jpg");
    building1_door_texture = load_texture_2d(images_path /    "building1/door.png");
    building1_door2_texture = load_texture_2d(images_path /   "building1/door2.png");
    building1_store_texture = load_texture_2d(images_path /   "building1/store.png");
    building1_store_NM = load_texture_2d(images_path /        "building1/storeNM.png");
    building1_top_texture = load_texture_2d(images_path /     "building1/top.jpg");
    building1_w2_texture = load_texture_2d(images_path /      "building1/w2.png");
    building1_wf_texture = load_texture_2d(images_path /      "building1/wf.jpg");
    building1_window1_texture = load_texture_2d(images_path / "building1/window1.jpg");
    building1_window2_texture = load_texture_2d(images_path / "building1/window2.png");
    building1_window3_texture = load_texture_2d(images_path / "building1/window3.png");
    building2_brick_texture = load_texture_2d(images_path /  "building2/brick.png");
    building2_door_texture = load_texture_2d(images_path /   "building2/door.jpg");
    building2_top_texture = load_texture_2d(images_path /    "building2/top.jpg");
    building2_window_texture = load_texture_2d(images_path / "building2/window.jpg");
    building5_brick_texture = load_texture_2d(images_path /       "building5/brick1.png");
    building5_window_texture = load_texture_2d(images_path /      "building5/window1.png");
    building5_window_dark_texture = load_texture_2d(images_path / "building5/window2.png");
    building3_brick_texture = load_texture_2d(images_path / "building3/brick.jpg");
    building3_brick_NM = load_texture_2d(images_path /      "building3/brick_NM.png");
    glass_texture = load_texture_2d(images_path / "glass.png");
    building4_brick_texture = load_texture_2d(images_path /       "building4/cihly.jpg");
    building4_brick_NM = load_texture_2d(images_path /            "building4/brick_NM.png");
    building4_frame_texture = load_texture_2d(images_path /       "building4/drevo.jpg");
    building4_window_texture = load_texture_2d(images_path /      "building4/window.jpg");
    building4_side_window_texture = load_texture_2d(images_path / "building4/side_window.jpg");
    building4_door_texture = load_texture_2d(images_path /        "building4/dvere.jpg");
    building4_top_texture = load_texture_2d(images_path /         "building4/top.jpg");
    sign_texture = load_texture_2d(images_path / "sign.jpg");
    moon_texture = load_texture_2d(images_path / "moon.jpg");
    sky_texture = load_texture_2d(images_path /  "sky_transparent_no_stars.png");
    rain_texture = load_texture_2d(images_path / "rain.png");
    

    // --------------------------------------------------------------------------
    // Initialize UBO Data
    // --------------------------------------------------------------------------
    camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
    camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
    camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 moon_position = glm::vec3(-400.0f, 120.0f, 20.0f);

    light_ubo.position = glm::vec4(moon_position, 1.0f);
    light_ubo.ambient_color = glm::vec4(0.5f);
    light_ubo.diffuse_color = glm::vec4(300.0f, 300.0f, 400.0f, 0.0f);
    light_ubo.specular_color = glm::vec4(400.0f, 400.0f, 600.0f, 2000.0f);


    for (size_t i = 0; i < 5; i++) {
        lights.push_back({
            glm::vec4(-26.0f + 13.0f * i, 4.2f, 3.7f, 1.0f), // position
            glm::vec4(0.125f, 0.125f, 0.075f, 0.0f),                               // ambient
            glm::vec4(0.25f, 0.25f, 0.15f, 0.0f),                               // diffuse
            glm::vec4(0.5f, 0.5f, 0.3f, 0.0f)                                // specular
        });

        lamps_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-26.0f + i * 13.0f, 0.15f, 3.7f)), //lamp
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
                            .specular_color = glm::vec4(100.0f, 100.0f, 100.0f, 500.0f)});
        lamps_glass_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-26.0f + i * 13.0f, 0.15f, 3.7f)), //lamp_glass
                            .ambient_color = glm::vec4(10.0f, 10.0f, 4.0f, 0.0f),
                            .diffuse_color = glm::vec4(0.0f, 0.0f, 0.0f, 0.9f),
                            .specular_color = glm::vec4(0.0f)});   

        lights.push_back({
            glm::vec4(-26.0f + 13.0f * i, 4.2f, -3.7f, 1.0f), // position
            glm::vec4(0.125f, 0.125f, 0.075f, 0.0f),                               // ambient
            glm::vec4(0.25f, 0.25f, 0.15f, 0.0f),                               // diffuse
            glm::vec4(0.5f, 0.5f, 0.3f, 0.0f)                                // specular
        });

        lamps_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-26.0f + i * 13.0f, 0.15f, -3.7f)), //lamp
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
                            .specular_color = glm::vec4(100.0f, 100.0f, 100.0f, 500.0f)});   
        lamps_glass_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-26.0f + i * 13.0f, 0.15f, -3.7f)), //lamp_glass
                            .ambient_color = glm::vec4(100.0f, 100.0f, 40.0f, 0.0f),
                            .diffuse_color = glm::vec4(0.0f, 0.0f, 0.0f, 0.9f),
                            .specular_color = glm::vec4(0.0f)});  
    }

    lights.push_back({
            glm::vec4(1.0f, 0.5f, 2.55f, 1.0f), // position
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // ambient
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // diffuse
            glm::vec4(0.025f, 0.0f, 0.0f, 0.0f)                                // specular
        });

    lights.push_back({
            glm::vec4(1.0f, 0.5f, 0.6f, 1.0f), // position
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // ambient
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // diffuse
            glm::vec4(0.025f, 0.0f, 0.0f, 0.0f)                                // specular
        });
    lights.push_back({
            glm::vec4(1.0f, 0.5f, 2.55f, 1.0f), // position
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // ambient
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // diffuse
            glm::vec4(0.01f, 0.0025f, 0.0f, 0.0f)                                // specular
        });

    lights.push_back({
            glm::vec4(-4.75f, 0.5f, 2.55f, 1.0f), // position
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // ambient
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // diffuse
            glm::vec4(0.005f, 0.005f, 0.002f, 0.0f)                                // specular
        });
    lights.push_back({
            glm::vec4(-4.75f, 0.5f, 0.6f, 1.0f), // position
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // ambient
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // diffuse
            glm::vec4(0.005f, 0.005f, 0.002f, 0.0f)                                // specular
        });
    lights.push_back({
            glm::vec4(-4.75f, 0.5f, 2.55f, 1.0f), // position
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // ambient
            glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),                               // diffuse
            glm::vec4(0.01f, 0.0025f, 0.0f, 0.0f)                                // specular
        });

    benches_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.8f, 0.19f, 4.0f)),
                        .ambient_color = glm::vec4(0.01f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                        .specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f)});
    benches_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.8f, 0.19f, 4.0f)),
                        .ambient_color = glm::vec4(0.01f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                        .specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f)});
    benches_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-24.5f, 0.19f, 4.0f)),
                        .ambient_color = glm::vec4(0.01f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                        .specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f)});
    benches_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(24.5f, 0.19f, 4.0f)),
                        .ambient_color = glm::vec4(0.01f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                        .specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f)});
    benches_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-27.5f, 0.19f, 4.0f)),
                        .ambient_color = glm::vec4(0.01f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                        .specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f)});
    benches_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(27.5f, 0.19f, 4.0f)),
                        .ambient_color = glm::vec4(0.01f),
                        .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                        .specular_color = glm::vec4(1.0f, 1.0f, 1.0f, 200.0f)});


    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //identity
                            .ambient_color = glm::vec4(0.25f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 0.1f, 3.2f)), glm::vec3(0.0f, -0.75f, 0.0f)), //asphalt main
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(3.0f, 3.0f, 3.0f, 500.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(3.2f, 0.1f, 32.0f)), glm::vec3(11.0f, -0.75f, 0.0f)), //asphalt front
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(3.0f, 3.0f, 3.0f, 500.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(3.2f, 0.1f, 32.0f)), glm::vec3(-11.0f, -0.75f, 0.0f)), //asphalt back
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(3.0f, 3.0f, 3.0f, 500.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 0.25f, 5.0f)), glm::vec3(0.0f, -0.25f, 1.6f)), //chodnik nalevo
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(3.0f, 3.0f, 3.0f, 500.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 0.25f, 5.0f)), glm::vec3(0.0f, -0.25f, -1.6f)), //chodnik napravo
                            .ambient_color = glm::vec4(0.01f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(2.0f, 2.0f, 2.0f, 500.0f)});
    /* FORD */
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //blinkry
                            .ambient_color = glm::vec4(50.0f, 5.0f, 0.0f, 0.0f),
                            .diffuse_color = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //zadni svetla
                            .ambient_color = glm::vec4(100.0f, 0.0f, 0.0f, 0.0f),
                            .diffuse_color = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //vnejsi svetla
                            .ambient_color = glm::vec4(6.0f, 6.0f, 0.0f, 0.0f),
                            .diffuse_color = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //vnitrni svetla
                            .ambient_color = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f),
                            .diffuse_color = glm::vec4(5.0f, 5.0f, 5.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    
    objects_ubos.push_back({.model_matrix = glm::rotate(glm::mat4(1.0f),(float)(M_PI), glm::vec3(0, 1, 0)), //budova na konci vzadu
                            .ambient_color = glm::vec4(0.25f),
                            .diffuse_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //karoserie
                            .ambient_color = glm::vec4(0.2f, 0.2f, 0.2f, 0.0f),
                            .diffuse_color = glm::vec4(3.0f, 0.0f, 0.0f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //okna
                            .ambient_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                            .diffuse_color = glm::vec4(0.2f, 0.2f, 0.2f, 0.75f),
                            .specular_color = glm::vec4(1500.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //predni sklo
                            .ambient_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                            .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f),
                            .specular_color = glm::vec4(1500.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //podvozek
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f),
                            .diffuse_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
                            .specular_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //chrom
                            .ambient_color = glm::vec4(1.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //interior
                            .ambient_color = glm::vec4(0.05f),
                            .diffuse_color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                            .specular_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)});
    /* PICKUP */
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //karoserie
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f),
                            .diffuse_color = glm::vec4(0.0f, 0.0f, 0.5f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //okna
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f),
                            .diffuse_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //kola
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f),
                            .diffuse_color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //seda
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.1f),
                            .diffuse_color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //podvozek
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f),
                            .diffuse_color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //svetla
                            .ambient_color = glm::vec4(0.1f, 0.1f, 0.1f, 0.0f),
                            .diffuse_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
                            .specular_color = glm::vec4(5.0f, 5.0f, 5.0f, 250.0f)});
    
    
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //lit identity
                            .ambient_color = glm::vec4(0.25f),
                            .diffuse_color = glm::vec4(50.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //dim lit identity
                            .ambient_color = glm::vec4(0.25f),
                            .diffuse_color = glm::vec4(20.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //ambiently lit identity
                            .ambient_color = glm::vec4(250.0f),
                            .diffuse_color = glm::vec4(1.0f),
                            .specular_color = glm::vec4(0.0f)});

    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //dark identity
                            .ambient_color = glm::vec4(0.1f),
                            .diffuse_color = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    
    objects_ubos.push_back({.model_matrix = glm::mat4(1.0f), //slightly dark identity
                            .ambient_color = glm::vec4(0.2f),
                            .diffuse_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});

    objects_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), moon_position) * glm::scale(glm::mat4(1.0f), glm::vec3(8.0f)), //moon
                            .ambient_color = glm::vec4(1.0f),
                            .diffuse_color = glm::vec4(50000.0f, 50000.0f, 50000.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 53.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(300.0f, 1.0f, 300.0f)), //clouds
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(150.0f, 150.0f, 150.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});
    objects_ubos.push_back({.model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.0005f, 3.0f, 0.0005f)), //rain
                            .ambient_color = glm::vec4(0.0f),
                            .diffuse_color = glm::vec4(10.0f, 10.0f, 10.0f, 1.0f),
                            .specular_color = glm::vec4(0.0f)});

    // --------------------------------------------------------------------------
    // Create Buffers
    // --------------------------------------------------------------------------
    glCreateBuffers(1, &camera_buffer);
    glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &light_buffer);
    glNamedBufferStorage(light_buffer, sizeof(LightUBO), &light_ubo, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &objects_buffer);
    glNamedBufferStorage(objects_buffer, sizeof(ObjectUBO) * objects_ubos.size(), objects_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &lamps_buffer);
    glNamedBufferStorage(lamps_buffer, sizeof(ObjectUBO) * lamps_ubos.size(), lamps_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &lamps_glass_buffer);
    glNamedBufferStorage(lamps_glass_buffer, sizeof(ObjectUBO) * lamps_glass_ubos.size(), lamps_glass_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &benches_buffer);
    glNamedBufferStorage(benches_buffer, sizeof(ObjectUBO) * benches_ubos.size(), benches_ubos.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &lights_buffer);
    glNamedBufferStorage(lights_buffer, lights.size() * sizeof(LightUBO), lights.data(), GL_DYNAMIC_STORAGE_BIT);


    for (size_t i = 0; i < rain_density; i++) {
        float x = random_neg() * 50;
        float y = random_pos() * 20 + 20.0f;
        float z = random_neg() * 15;
        rain_x_positions.push_back(x);
        rain_y_positions.push_back(y);
        rain_z_positions.push_back(z);
        instanced_objects.push_back({
            glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.003f, 2.0f, 0.003f)),
            glm::vec4(1.0f),                       // ambient
            glm::vec4(15.0f, 15.0f, 18.0f, 0.5f),  // diffuse
            glm::vec4(0.0f),                       // specular
            glm::vec4(0.0f)                        // padding
        });
    }

    glCreateBuffers(1, &instanced_objects_buffer);
    glNamedBufferStorage(instanced_objects_buffer, instanced_objects.size() * sizeof(ObjectUBO), instanced_objects.data(),
                         GL_DYNAMIC_STORAGE_BIT);

    engine = createIrrKlangDevice();
    engine->play2D("rain.mp3", true);
    ISound* music = engine->play3D("engine2.mp3",
	                               vec3df(0,0,-1), true, false, true);
    music->setMinDistance(5.0f);

    compile_shaders();
}

Application::~Application() {
    delete_shaders();

    glDeleteBuffers(1, &camera_buffer);
    glDeleteBuffers(1, &light_buffer);
    glDeleteBuffers(1, &objects_buffer);
    glDeleteBuffers(1, &lights_buffer);
    glDeleteBuffers(1, &lamps_buffer);
    glDeleteBuffers(1, &lamps_glass_buffer);
    glDeleteBuffers(1, &benches_buffer);
    glDeleteBuffers(1, &instanced_objects_buffer);

    engine->drop();
}

// ----------------------------------------------------------------------------
// Methods
// ----------------------------------------------------------------------------

void Application::delete_shaders() {}

void Application::compile_shaders() {
    delete_shaders();
    main_program = create_program(lecture_shaders_path / "main.vert", lecture_shaders_path / "main.frag");
    rain_program = create_program(lecture_shaders_path / "instantiated.vert", lecture_shaders_path / "instantiated.frag");
}

void Application::update(float delta) {}

void Application::render() {
    //hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);        

    // --------------------------------------------------------------------------
    // Update UBOs
    // --------------------------------------------------------------------------
    // Camera
    camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
    camera_ubo.projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
    camera_ubo.view = camera.get_view_matrix();
    glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

// flickering lamps

    for (int i = 0; i < 10; i++) {
        lights[i].diffuse_color = glm::vec4(0.25f, 0.25f, 0.15f, 0.0f);
    }
    float noise = float(rand()) / float(rand());
    noise -= floor(noise);
    noise /= 10;
    float noises[] = {noise, 0.0f, 0.0f, 0.0f, 0.0f};
    noise = noises[rand() % 5];
    lights[rand() % 10].diffuse_color = glm::vec4(0.25f - noise, 0.25f - noise, 0.15f - noise, 0.0f);
    
    glNamedBufferSubData(lights_buffer, 0, lights.size() * sizeof(LightUBO), lights.data());

// rain
    for (size_t i = 0; i < rain_density; i++) {
        if (rain_y_positions[i] < -2.0f) {
            rain_x_positions[i] = random_neg() * 50;
            rain_y_positions[i] = random_pos() * 20 + 20.0f;
            rain_z_positions[i] = random_neg() * 15;
        } else {
            rain_y_positions[i] -= 1.5f;
        }
        instanced_objects[i].model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(rain_x_positions[i], rain_y_positions[i], rain_z_positions[i])) * glm::scale(glm::mat4(1.0f), glm::vec3(0.003f, 2.0f, 0.003f));
    }
    
    glNamedBufferSubData(instanced_objects_buffer, 0, instanced_objects.size() * sizeof(ObjectUBO), instanced_objects.data());

//audio
    glm::vec3 listener_position = camera.get_eye_position();
    engine->setListenerPosition(vec3df(listener_position.x, listener_position.y, listener_position.z), vec3df(0.0f, 0.0f, 0.0f) - vec3df(listener_position.x, listener_position.y, listener_position.z));


    // --------------------------------------------------------------------------
    // Draw scene
    // --------------------------------------------------------------------------
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Configure fixed function pipeline
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    // Draw objects
    glUseProgram(main_program);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, light_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, lights_buffer);

/*benches*/

    glUniform1i(glGetUniformLocation(main_program, "has_texture"), false);
    for (int i = 0; i < 6; i++) {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, benches_buffer, i * 256, sizeof(ObjectUBO));
        
        bench_frame->draw();
    }
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), true);
    glBindTextureUnit(3, wood_texture);
    for (int i = 0; i < 6; i++) {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, benches_buffer, i * 256, sizeof(ObjectUBO));
        
        bench_wood->draw();
    }


/*road*/

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 1 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), true);
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), true);
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 7);
    glUniform1i(glGetUniformLocation(main_program, "light_reflection"), true);

    glBindTextureUnit(3, asphalt_texture);
    glBindTextureUnit(4, asphalt_NM);

    cube->draw();

    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);

//konce:
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 3 * 256, sizeof(ObjectUBO));
    
    glBindTextureUnit(3, asphalt_tilted_texture);
    glBindTextureUnit(4, asphalt_tilted_NM);
    cube->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 2 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), -1); //mirrored texture
    cube->draw();


/* CHODNIKY */

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 4 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 35);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 7);
    
    glBindTextureUnit(3, pavement_texture);
    glBindTextureUnit(4, pavement_NM);
    cube->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 5 * 256, sizeof(ObjectUBO));
    cube->draw();

    
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), false);
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), -1);
    glUniform1i(glGetUniformLocation(main_program, "light_reflection"), false);

/* STREET SIGN */

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 0 * 256, sizeof(ObjectUBO));
    glBindTextureUnit(3, sign_texture);
    sign->draw();

/* BUILDING 1 */
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), true);
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), true);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 0 * 256, sizeof(ObjectUBO));
    glBindTextureUnit(3, building1_brick_texture);
    glBindTextureUnit(4, building1_brick_NM);
    building1_brick->draw();
    glBindTextureUnit(3, building1_store_texture);
    glBindTextureUnit(4, building1_store_NM);
    building1_store->draw();
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), false);
    glBindTextureUnit(3, building1_ac_texture);
    building1_ac->draw();
    glBindTextureUnit(3, building1_door_texture);
    building1_door->draw();
    glBindTextureUnit(3, building1_door2_texture);
    building1_door2->draw();
    glBindTextureUnit(3, building1_wf_texture);
    building1_wf->draw();
    building1_ramy->draw();
    building1_vydech->draw();
    glBindTextureUnit(3, building1_w2_texture);
    building1_w2->draw();
    glBindTextureUnit(3, building1_window1_texture);
    building1_window1->draw();
    glBindTextureUnit(3, building1_window2_texture);
    building1_window2->draw();
    glBindTextureUnit(3, building1_window3_texture);
    building1_window3->draw();
    glBindTextureUnit(3, building1_top_texture);
    building1_top->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 23 * 256, sizeof(ObjectUBO));
    glBindTextureUnit(3, building1_window2_texture);
    building1_window2_lit->draw();
    glBindTextureUnit(3, building1_window3_texture);
    building1_window3_lit->draw();
    
/* BUILDING 2 */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 0 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), -1);

    glBindTextureUnit(3, building2_brick_texture);
    building2_brick->draw();

    glBindTextureUnit(3, building2_window_texture);
    building2_window->draw();

    glBindTextureUnit(3, building2_door_texture);
    building2_door->draw();

    glBindTextureUnit(3, building2_top_texture);
    building2_top->draw();
 

/* BUILDING 3 */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 0 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 15);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 15);

    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), true);
    glBindTextureUnit(3, building3_brick_texture);
    glBindTextureUnit(4, building3_brick_NM);
    building3_brick->draw();
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), false);

    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), -1);
    glBindTextureUnit(3, black_metal_texture);
    building3_stairs->draw();
    glBindTextureUnit(3, building1_wf_texture);
    building3_frames->draw();

    glBindTextureUnit(3, building1_wf_texture);
    building3_window->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 25 * 256, sizeof(ObjectUBO));
    building3_window_lit->draw();

/* BUILDING 4 */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 26 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), -1);
    
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), true);
    glBindTextureUnit(3, building4_brick_texture);
    glBindTextureUnit(4, building4_brick_NM);
    building4_brick->draw();
    glUniform1i(glGetUniformLocation(main_program, "has_normal_map"), false);

    glBindTextureUnit(3, building4_window_texture);
    building4_window->draw();

    glBindTextureUnit(3, building4_side_window_texture);
    building4_side_window->draw();

    glBindTextureUnit(3, building4_door_texture);
    building4_door->draw();

    glBindTextureUnit(3, building4_frame_texture);
    building4_frames->draw();

    glBindTextureUnit(3, building4_window_texture);
    building4_window->draw();

    glBindTextureUnit(3, building4_top_texture);
    building4_top->draw();

/* BUILDING 5 */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 27 * 256, sizeof(ObjectUBO));
    
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 15);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 15);

    glBindTextureUnit(3, building5_brick_texture);
    building5_brick->draw();
    glBindTextureUnit(3, building1_wf_texture);
    building5_paint->draw();
    glBindTextureUnit(3, building5_window_texture);
    
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 50);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 80);

    building5_window->draw();
    building5_window_inactive->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 24 * 256, sizeof(ObjectUBO));
    glBindTextureUnit(3, building5_window_dark_texture);
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 5);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 3);
    building5_window_lit->draw();

/* BUILDING 5 MIRRORED */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 10 * 256, sizeof(ObjectUBO));
    
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 15);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 15);

    glBindTextureUnit(3, building5_brick_texture);
    building5_brick->draw();
    glBindTextureUnit(3, building1_wf_texture);
    building5_paint->draw();
    glBindTextureUnit(3, building5_window_texture);
    
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 50);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 80);

    building5_window->draw();
    building5_window_inactive->draw();
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 5);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 3);
    building5_window_lit->draw();

/* WALLS */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 0 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 5);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 5);
    glBindTextureUnit(3, building3_brick_texture);
    wall_front->draw();
    wall_back->draw();

    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), -1);

/* MOON */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 28 * 256, sizeof(ObjectUBO));
    glBindTextureUnit(3, moon_texture);
    sphere->draw();

/* PICKUP */
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), false);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 17 * 256, sizeof(ObjectUBO));
    pickup_karoserie->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 18 * 256, sizeof(ObjectUBO));
    pickup_okna->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 19 * 256, sizeof(ObjectUBO));
    pickup_kola->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 20 * 256, sizeof(ObjectUBO));
    pickup_seda->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 21 * 256, sizeof(ObjectUBO));
    pickup_podvozek->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 22 * 256, sizeof(ObjectUBO));
    pickup_svetla->draw();

/* RAIN */
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 30 * 256, sizeof(ObjectUBO));
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), false);

    glUseProgram(rain_program);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, instanced_objects_buffer);
    instantiable_cube.bind_vao();
    glDrawElementsInstanced(instantiable_cube.mode, instantiable_cube.draw_elements_count, GL_UNSIGNED_INT, nullptr, instanced_objects.size());
    
    glUseProgram(main_program);

    
/*LAMPS*/
    
    for (int i = 0; i < 10; i++) {        
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, lamps_buffer, i * 256, sizeof(ObjectUBO));
        lamp->draw();

        glBindBufferRange(GL_UNIFORM_BUFFER, 2, lamps_glass_buffer, i * 256, sizeof(ObjectUBO));
        lamp_glass->draw();
    }

/* FORD */   
    if (counter > 25) {
        blink = !blink;
        glUniform1i(glGetUniformLocation(main_program, "blink"), blink);
        counter = 0;
    }
    counter++;

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 7 * 256, sizeof(ObjectUBO));
    taillights->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 8 * 256, sizeof(ObjectUBO));
    outer_lights->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 9 * 256, sizeof(ObjectUBO));
    inner_lights->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 11 * 256, sizeof(ObjectUBO));
    ford_karoserie->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 14 * 256, sizeof(ObjectUBO));
    ford_podvozek->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 15 * 256, sizeof(ObjectUBO));
    ford_chrom->draw();
    
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 16 * 256, sizeof(ObjectUBO));
    ford_interior->draw();

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 12 * 256, sizeof(ObjectUBO));
    ford_okna->draw();
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 13 * 256, sizeof(ObjectUBO));
    ford_sklo->draw();
    
    if (blink) {
        glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 6 * 256, sizeof(ObjectUBO));
        turn_signals->draw();
    }


/* CLOUDS */
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), true);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, objects_buffer, 29 * 256, sizeof(ObjectUBO));
    glBindTextureUnit(3, sky_texture);
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 8);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), 8);
    glUniform1i(glGetUniformLocation(main_program, "moving_texture"), true);
    moving_factor += 0.0005;
    glUniform1f(glGetUniformLocation(main_program, "moving_factor"), moving_factor);
    cube->draw();

    glUniform1i(glGetUniformLocation(main_program, "moving_texture"), false);
    glUniform1i(glGetUniformLocation(main_program, "has_texture"), false);
    glUniform1i(glGetUniformLocation(main_program, "x_repeat"), 1);
    glUniform1i(glGetUniformLocation(main_program, "y_repeat"), -1);
}

void Application::render_ui() { const float unit = ImGui::GetFontSize(); }

// ----------------------------------------------------------------------------
// Input Events
// ----------------------------------------------------------------------------

void Application::on_resize(int width, int height) {
    // Calls the default implementation to set the class variables.
    PV112Application::on_resize(width, height);
}

void Application::on_mouse_move(double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_button(int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(int key, int scancode, int action, int mods) {
    // Calls default implementation that invokes compile_shaders when 'R key is hit.
    PV112Application::on_key_pressed(key, scancode, action, mods);
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        if (inside_car) {
            camera.set_center(last_camera_pos);
        } else {
            last_camera_pos = camera.get_center();
            camera.set_center(glm::vec3(-1.8f, 1.2f, 1.98f));
        }
        inside_car = !inside_car;
    }
    if (!inside_car) {
        if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
            if (action == GLFW_PRESS) {
                camera.move(LEFT);
            } else if (action == GLFW_RELEASE) {
                camera.move(STOP_LEFT);
            }
        } else if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
            if (action == GLFW_PRESS) {
                camera.move(RIGHT);
            } else if (action == GLFW_RELEASE) {
                camera.move(STOP_RIGHT);
            }
        } else if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
            if (action == GLFW_PRESS) {
                camera.move(FORWARD);
            } else if (action == GLFW_RELEASE) {
                camera.move(STOP_FORWARD);
            }
        } else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
            if (action == GLFW_PRESS) {
                camera.move(BACKWARD);
            } else if (action == GLFW_RELEASE) {
                camera.move(STOP_BACKWARD);
            }
        } else if (key == GLFW_KEY_LEFT_CONTROL) {
            if (action == GLFW_PRESS) {
                camera.move(CRUNCH);
            } else if (action == GLFW_RELEASE) {
                camera.move(STOP_CRUNCH);
            }
        } else if (key == GLFW_KEY_LEFT_SHIFT) {
            if (action == GLFW_PRESS) {
                camera.move(SPRINT);
            } else if (action == GLFW_RELEASE) {
                camera.move(STOP_SPRINT);
            }
        }
    }
}