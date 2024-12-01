#pragma once

#include "vex.h"

class drawable 
{
public:
    virtual ~drawable() = default;

    virtual void render() = 0;

    virtual int get_x_pos() = 0;
    virtual int get_y_pos() = 0;

    virtual int get_width() = 0;
    virtual int get_height() = 0;

    virtual void set_x_pos(int x) = 0;
    virtual void set_y_pos(int y) = 0;
    virtual void set_position(int x, int y) = 0;
};