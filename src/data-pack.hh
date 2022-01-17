#pragma once

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "gl/gl.hh"
struct DataPack
{
    DataPack(std::string path)
    {
        std::ifstream data(path + "/data");
        std::ifstream vert(path + "/vert");

        if (data.fail() || vert.fail())
            throw std::invalid_argument("No such data: " + path);

        data >> life_span;
        data >> initial_color.r >> initial_color.g >> initial_color.b
            >> initial_color.a;

        float v;
        while (vert >> v)
            vertices.emplace_back(v);

        update_path = path + "/update.glsl";
        spawn_path = path + "/spawn.glsl";
    }

    ~DataPack() = default;
    DataPack(const DataPack&) = delete;

    GLfloat life_span;
    gl::Color initial_color;

    std::vector<float> vertices;

    std::string update_path;
    std::string spawn_path;
};
