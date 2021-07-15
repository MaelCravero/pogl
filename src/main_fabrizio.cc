#include <GL/freeglut.h>
#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <vector>

//#include <glt_transform.hh>

#include "image.hh"
#include "image_io.hh"
#include "object_vbo.hh"

//#define SAVE_RENDER

#define TEST_OPENGL_ERROR()                                                    \
    do                                                                         \
    {                                                                          \
        GLenum err = glGetError();                                             \
        if (err != GL_NO_ERROR)                                                \
            std::cerr << "OpenGL ERROR!" << __LINE__ << std::endl;             \
    } while (0)

GLuint teapot_vao_id;
GLuint program_id;

void window_resize(int width, int height)
{
    // std::cout << "glViewport(0,0,"<< width << "," << height <<
    // ");" << std::endl;
    glViewport(0, 0, width, height);
}

#if defined(SAVE_RENDER)
bool saved = false;
#endif

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(teapot_vao_id);
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_data.size());
    glBindVertexArray(0);
#if defined(SAVE_RENDER)
    if (!saved)
    {
        tifo::rgb24_image* texture = new tifo::rgb24_image(800, 590);
        glReadPixels(150, 350, 800, 590, GL_RGB, GL_UNSIGNED_BYTE,
                     texture->pixels);
        // glReadPixels(0, 0, 1024, 1024, GL_RGB, GL_UNSIGNED_BYTE,
        // texture->pixels);
        tifo::save_image(*texture, "render.tga");
        std::cout << "Save " << std::endl;
        delete texture;
        // saved = true;
    }
#endif
    glutSwapBuffers();
}

void init_glut(int& argc, char* argv[])
{
    // glewExperimental = GL_TRUE;
    glutInit(&argc, argv);
    glutInitContextVersion(4, 5);
    glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_DEBUG);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1024, 1024);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Shader Programming");
    glutDisplayFunc(display);
    glutReshapeFunc(window_resize);
}

bool init_glew()
{
    if (glewInit())
    {
        std::cerr << " Error while initializing glew";
        return false;
    }
    return true;
}

void init_GL()
{
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glClearColor(0.4, 0.4, 0.4, 1.0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
}

void init_object_vbo()
{
    int max_nb_vbo = 5;
    int nb_vbo = 0;
    int index_vbo = 0;
    GLuint vbo_ids[max_nb_vbo];

    GLint vertex_location = glGetAttribLocation(program_id, "position");
    GLint normal_flat_location = glGetAttribLocation(program_id, "normalFlat");
    GLint normal_smooth_location =
        glGetAttribLocation(program_id, "normalSmooth");
    GLint color_location = glGetAttribLocation(program_id, "color");
    GLint uv_location = glGetAttribLocation(program_id, "uv");

    glGenVertexArrays(1, &teapot_vao_id);
    glBindVertexArray(teapot_vao_id);

    if (vertex_location != -1)
        nb_vbo++;
    if (normal_flat_location != -1)
        nb_vbo++;
    if (normal_smooth_location != -1)
        nb_vbo++;
    if (color_location != -1)
        nb_vbo++;
    if (uv_location != -1)
        nb_vbo++;
    glGenBuffers(nb_vbo, vbo_ids);

    if (vertex_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(float),
                     vertex_buffer_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertex_location);
    }

    if (normal_flat_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        glBufferData(GL_ARRAY_BUFFER,
                     normal_flat_buffer_data.size() * sizeof(float),
                     normal_flat_buffer_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(normal_flat_location, 3, GL_FLOAT, GL_FALSE, 0,
                              0);
        glEnableVertexAttribArray(normal_flat_location);
    }

    if (normal_smooth_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        glBufferData(GL_ARRAY_BUFFER,
                     normal_smooth_buffer_data.size() * sizeof(float),
                     normal_smooth_buffer_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(normal_smooth_location, 3, GL_FLOAT, GL_FALSE, 0,
                              0);
        glEnableVertexAttribArray(normal_smooth_location);
    }

    if (color_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        glBufferData(GL_ARRAY_BUFFER, color_buffer_data.size() * sizeof(float),
                     color_buffer_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(color_location);
    }

    if (uv_location != -1)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[index_vbo++]);
        glBufferData(GL_ARRAY_BUFFER, uv_buffer_data.size() * sizeof(float),
                     uv_buffer_data.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(uv_location);
    }

    // Break the existing vertex binding
    glBindVertexArray(0);
}

void init_textures()
{
    tifo::rgb24_image* texture = tifo::load_image("texture.tga");
    tifo::rgb24_image* lighting = tifo::load_image("lighting.tga");
    tifo::rgb24_image* normalmap = tifo::load_image("normalmap.tga");
    GLuint texture_id;
    GLuint lighting_id;
    GLuint normalmap_id;
    GLint tex_location;
    GLint light_location;
    GLint normalmap_location;

    std::cout << "texture " << texture->sx << " ," << texture->sy << "\n";
    std::cout << "light " << lighting->sx << " ," << lighting->sy << "\n";
    std::cout << "normalmap " << normalmap->sx << " ," << normalmap->sy
              << std::endl;

    GLint texture_units, combined_texture_units;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combined_texture_units);
    std::cout << "Limit 1 " << texture_units << " limit 2 "
              << combined_texture_units << std::endl;

    glGenTextures(1, &texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->sx, texture->sy, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, texture->pixels);
    tex_location = glGetUniformLocation(program_id, "texture_sampler");
    std::cout << "tex_location " << tex_location << std::endl;
    glUniform1i(tex_location, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &lighting_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lighting_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lighting->sx, lighting->sy, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, lighting->pixels);
    light_location = glGetUniformLocation(program_id, "lighting_sampler");
    std::cout << "light_location " << light_location << std::endl;
    glUniform1i(light_location, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenTextures(1, &normalmap_id);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normalmap_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, normalmap->sx, normalmap->sy, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, normalmap->pixels);
    normalmap_location = glGetUniformLocation(program_id, "normalmap_sampler");
    std::cout << "normalmap_location " << normalmap_location << std::endl;
    glUniform1i(normalmap_location, 2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    delete texture;
    delete lighting;
    delete normalmap;
}

std::string load(const std::string& filename)
{
    std::ifstream input_src_file(filename, std::ios::in);
    std::string ligne;
    std::string file_content = "";
    if (input_src_file.fail())
    {
        std::cerr << "FAIL\n";
        return "";
    }
    while (getline(input_src_file, ligne))
    {
        file_content = file_content + ligne + "\n";
    }
    file_content += '\0';
    input_src_file.close();
    return file_content;
}

bool init_shaders()
{
    std::string vertex_src = load("vertex.glsl");
    std::string fragment_src = load("fragment.glsl");
    GLuint shader_id[2];
    GLint compile_status = GL_TRUE;
    char* vertex_shd_src =
        (char*)std::malloc(vertex_src.length() * sizeof(char));
    char* fragment_shd_src =
        (char*)std::malloc(fragment_src.length() * sizeof(char));
    vertex_src.copy(vertex_shd_src, vertex_src.length());
    fragment_src.copy(fragment_shd_src, fragment_src.length());

    shader_id[0] = glCreateShader(GL_VERTEX_SHADER);
    shader_id[1] = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader_id[0], 1, (const GLchar**)&(vertex_shd_src), 0);
    glShaderSource(shader_id[1], 1, (const GLchar**)&(fragment_shd_src), 0);
    for (int i = 0; i < 2; i++)
    {
        glCompileShader(shader_id[i]);
        glGetShaderiv(shader_id[i], GL_COMPILE_STATUS, &compile_status);
        if (compile_status != GL_TRUE)
        {
            GLint log_size;
            char* shader_log;
            glGetShaderiv(shader_id[i], GL_INFO_LOG_LENGTH, &log_size);
            shader_log = (char*)std::malloc(
                log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
            if (shader_log != 0)
            {
                glGetShaderInfoLog(shader_id[i], log_size, &log_size,
                                   shader_log);
                std::cerr << "SHADER " << i << ": " << shader_log << std::endl;
                std::free(shader_log);
            }
            std::free(vertex_shd_src);
            std::free(fragment_shd_src);
            glDeleteShader(shader_id[0]);
            glDeleteShader(shader_id[1]);
            return false;
        }
    }
    std::free(vertex_shd_src);
    std::free(fragment_shd_src);

    GLint link_status = GL_TRUE;
    program_id = glCreateProgram();

    if (program_id == 0)
        return false;
    for (int i = 0; i < 2; i++)
    {
        glAttachShader(program_id, shader_id[i]);
    }
    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLint log_size;
        char* program_log;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_size);
        program_log = (char*)std::malloc(
            log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
        if (program_log != 0)
        {
            glGetProgramInfoLog(program_id, log_size, &log_size, program_log);
            std::cerr << "Program " << program_log << std::endl;
            std::free(program_log);
        }
        glDeleteProgram(program_id);
        glDeleteShader(shader_id[0]);
        glDeleteShader(shader_id[1]);
        program_id = 0;
        return false;
    }
    glUseProgram(program_id);
    return true;
}

/*void tmp() {
  glt::matrix4 look = glt::matrix4::identity();
  glt::matrix4 frustum = glt::matrix4::identity();

  glt::frustum(frustum,
           -1, 1, -1, 1,
           5, 50000
         );

  glt::look_at(look,
           20, 20, 20,
           0, 0, 0,
           0, 1, 0
           );

  std::cout << "Look\n" << look << "\n";
  std::cout << "frustum\n" << frustum << std::endl;
  }*/

int main(int argc, char* argv[])
{
    //  tmp();
    init_glut(argc, argv);
    if (!init_glew())
        std::exit(-1);
    init_GL();
    init_shaders();
    init_object_vbo();
    init_textures();
    glutMainLoop();
}
