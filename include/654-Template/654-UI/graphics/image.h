#pragma once

#include "vex.h"

class image : public drawable 
{
public:
    image(const std::string& file_name, int x, int y, int w, int h, UI_distance_units units);

    int get_x_pos() override;
    int get_y_pos() override;
    int get_width() override;
    int get_height() override;

    void set_x_pos(int x) override;
    void set_y_pos(int y) override;
    void set_position(int x, int y) override;

    void render() override;

private:
    std::string file_name;
    int x;
    int y;
    int w;
    int h;
    UI_distance_units units;
};