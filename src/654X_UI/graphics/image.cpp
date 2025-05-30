#include "vex.h"

image::image(const std::string& file_name, float x, float y, float w, float h, UI_distance_units units) :
    file_name(file_name), units(units)
{
    this->x = to_pixels(x, units);
    this->y = to_pixels(y, units);
    this->w = to_pixels(w, units);
    this->h = to_pixels(h, units);
};

int image::get_x_pos() { return(x); }
int image::get_y_pos() { return(y); }
int image::get_width() { return(w); }
int image::get_height() { return(h); }
void image::set_x_pos(int x) { this->x = x; }
void image::set_y_pos(int y) { this->y = y; }
void image::set_position(int x, int y) { this->x = x; this->y = y; }
void image::set_width(int w) { this->w = w; }
void image::set_height(int h) { this->h = h; }

void image::render() {
    Brain.Screen.drawImageFromFile(file_name.c_str(), x, y);
}
