#pragma once

#include "vex.h"

class label : public UI_component
{
public:
    label(const std::string& label, int x, int y, UI_distance_units units);

    template<typename T>
    label(const std::string& label, T& data, int x, int y, UI_distance_units units);
    
    template<typename F>
    label(const std::string& label, F&& data_func, int x, int y, UI_distance_units units);

    int get_x_pos() override;
    int get_y_pos() override;
    int get_width() override;
    int get_height() override;

    void set_x_pos(int x) override;
    void set_y_pos(int y) override;
    void set_position(int x, int y) override;

    void render() override;

private:
    std::string label_text;
    std::function<std::string()> data_func;
    int x;
    int y;
    int w;
    int h;
    UI_distance_units units;
};

template<typename T>
label::label(const std::string& label, T& data, int x, int y, UI_distance_units units): 
    label_text(label),
    data_func([&data]() {
        std::ostringstream oss;
        if (std::is_floating_point<T>::value) {
            oss << std::fixed << std::setprecision(5);
        }
        oss << data;
        return oss.str();
    }),
    x(x), y(y), units(units)
{
    this->x = to_pixels(x, units);
    this->y = to_pixels(y, units);
};


template<typename F>
label::label(const std::string& label, F&& data_func, int x, int y, UI_distance_units units): 
    label_text(label),
    data_func([data_func = std::forward<F>(data_func)]() {
        std::ostringstream oss;
        auto value = data_func();
        if (std::is_floating_point<decltype(value)>::value) {
            oss << std::fixed << std::setprecision(5);
        }
        oss << value;
        return oss.str();
    }),
    x(x), y(y), units(units) 
{
    this->x = to_pixels(x, units);
    this->y = to_pixels(y, units);
};