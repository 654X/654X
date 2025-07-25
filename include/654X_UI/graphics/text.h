#pragma once

#include "vex.h"

namespace mik {

class text : public drawable 
{
public:
    text(const std::string& text_label, float x, float y, UI_distance_units units);
    text(const std::string& text_label, float x, float y, uint32_t hue, UI_distance_units units);
    text(const std::string& text_label, float x, float y, vex::color hue, UI_distance_units units);

    int get_x_pos() override;
    int get_y_pos() override;
    int get_width() override;
    int get_height() override;
    std::string get_text() override;

    void set_x_pos(int x) override;
    void set_y_pos(int y) override;
    void set_position(int x, int y) override;
    
    void set_width(int w) override;
    void set_height(int h) override;
    void set_text(std::string text_label) override;

    void render() override;

private:
    std::string text_label;
    int x;
    int y;
    int w;
    int h;
    uint32_t hue;
    UI_distance_units units;
};
}