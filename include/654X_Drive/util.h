#pragma once

#include "vex.h"

typedef struct point{
  double x;
  double y;
};

/** @brief Explicitly specifies rotation direction when turning or swinging. */
enum class direction {
    FASTEST, // Direction chosen automatically (shortest path)
    CW,      // Clockwise rotation
    CCW      // Counter‑clockwise rotation
};

/**
 * @brief "Clamps" a number between a min and max.
 * Does no error checking and assumes min is less
 * than or equal to max.
 * @param input The number to be clamped.
 * @param min Minimum edge of the clamp.
 * @param max Maximum edge of the clamp.
 * @return Clamped number.
 */
float clamp(float input, float min, float max);

/**
 * @brief Deadband function for joystick control.
 * If the joystick value is small, we should just consider it 0.
 * @param input The input joystick value.
 * @param width Minimum value to not get zeroed out.
 * @return The deadbanded value.
 */
float deadband(float input, float width);

/**
 * @brief Deadband function for joystick control.
 * If the joystick value is small, we should just consider it 0.
 * Inputs are squared which allows finer control.
 * @param input The input joystick value.
 * @param width Minimum value to not get zeroed out.
 * @return The deadbanded value.
 */
float deadband_squared(float input, float width);

/**
 * @brief Scales a joystick to drive voltage scale.
 * Values get multiplied by 12 because motors can
 * output a max of 12 volts.
 * @param percent The input joystick reading.
 * @return The equivalent value in volts.
 */
float percent_to_volt(float percent);

/**
 * @brief Scales a percent value to the voltage scale.
 * -100, 100 is converted to a -12, 12 scale.
 * @param volt voltage units.
 * @return The equivalent value in percent.
 */
float volt_to_percent(float volt);

/**
 * @brief Converts an angle in degrees to radians.
 * @param angle The angle in degrees.
 * @return Angle in radians.
 */
float to_rad(float angle_deg);

/**
 * @brief Converts an angle in radians to degrees.
 * @param angle The angle in radians.
 * @return Angle in degrees.
 */
float to_deg(float angle_rad);

/**
 * @brief Converts an angle to an equivalent one in the range [-180, 180).
 * @param angle The angle to be reduced in degrees.
 * @return Reduced angle.
 */
float reduce_negative_180_to_180(float angle);

/**
 * @brief Converts an angle to an equivalent one in the range [-90, 90).
 * If the angle has no equivalent, then the angle halfway around
 * the circle is returned.
 * @param angle The angle to be reduced in degrees.
 * @return Reduced angle.
 */
float reduce_negative_90_to_90(float angle);

/**
 * @brief Converts an angle to an equivalent one in the range [0, 360).
 * @param angle The angle to be reduced in degrees.
 * @return Reduced angle.
 */
float reduce_0_to_360(float angle);

/**
 * @brief Conditionally mirror an angle (degrees) across the 0°/360° axis.
 *
 *
 * @param angle Angle in degrees.
 * @param mirror Whether to apply mirroring.
 * @return Mirrored (or original) angle in degrees.
 */
float mirror_angle(float angle, bool mirror);

/**
 * @brief Conditionally mirror a rotation direction (swap CW ↔ CCW).
 *
 * If `mirror` is true, direction::CW becomes direction::CCW and vice versa.
 * direction::FASTEST is returned unchanged. When `mirror` is false, `dir` is
 * returned as-is.
 *
 * @param dir    Input rotation direction (CW, CCW, or FASTEST).
 * @param mirror Whether to apply mirroring.
 * @return Mirrored (or original) direction value.
 */
direction mirror_direction(direction dir, bool mirror);

/**
 * @brief Conditionally mirror an X coordinate (negate when mirrored).
 *
 * @param x X coordinate value.
 * @param mirror Whether to apply mirroring.
 * @return Mirrored (negated) X coordinate if mirrored; otherwise the original.
 */
float mirror_x(float x, bool mirror);

/**
 * @brief Conditionally mirror a Y coordinate (negate when mirrored).
 *
 * @param y Y coordinate value.
 * @param mirror Whether to apply mirroring.
 * @return Mirrored (negated) Y coordinate if mirrored; otherwise the original.
 */
float mirror_y(float y, bool mirror);

/**
 * @brief Normalize an angular error according to a direction preference.
 *
 * For direction::CW, negative `error` values are wrapped by adding 360 so the
 * result is ≥ 0. For direction::CCW, positive `error` values are wrapped by
 * subtracting 360 so the result is ≤ 0. For direction::FASTEST (default), the
 * value is reduced to the range (-180, 180] via reduce_negative_180_to_180().
 *
 * @param error Signed angular difference in degrees (e.g., target - current).
 * @param dir Direction preference: CW, CCW, or FASTEST (default).
 * @return Normalized angular error in degrees consistent with `dir`.
 */
float angle_error(float error, direction dir = direction::FASTEST);

/**
 * @brief Settling control for odometry functions.
 * Draws a line perpendicular to the line from the robot to the desired 
 * endpoint, and checks if the robot has crossed that line. Allows for
 * very quick settling, and thereby chaining for fast motion control.
 * @param desired_X The ending X position in inches.
 * @param desired_Y The ending Y position in inches.
 * @param desired_angle_deg The direction of the line to be drawn.
 * @param current_X The robot's X position in inches.
 * @param current_Y The robot's Y position in inches.
 * @return Whether the robot can be considered settled.
 */
bool is_line_settled(float desired_X, float desired_Y, float desired_angle_deg, float current_X, float current_Y);

/**
 * @brief Voltage scaling to keep from applying more than 12 volts to either side of the drive.
 * Divides both drive and heading output proportionally to get a similar result to the
 * desired one.
 * @param drive_output The forward output of the drive.
 * @param heading_output The angular output of the drive.
 * @return The scaled voltage for the left side of the robot.
 */
float left_voltage_scaling(float drive_output, float heading_output);

/**
 * Voltage scaling to keep from applying more than 12 volts to either side of the drive.
 * Divides both drive and heading output proportionally to get a similar result to the
 * desired one.
 * @param drive_output The forward output of the drive.
 * @param heading_output The angular output of the drive.
 * @return The scaled voltage for the right side of the robot.
 */
float right_voltage_scaling(float drive_output, float heading_output);

/**
 * @brief Brings an output up to the minimum voltage if it's too slow.
 * Used for minimum voltage calculations for movement chaining.
 * Has no effect on 0 voltage output, because how do we know 
 * which way it's supposed to be going?
 * @param drive_output The forward output of the drive.
 * @param drive_min_voltage The minimum output of the drive.
 * @return The voltage with the minimum applied.
 */
float clamp_min_voltage(float drive_output, float drive_min_voltage);

/** @return the distance between two x, y points */
float dist(point p1, point p2);

/** @return -1 for negative numbers and 1 for positive numbers */
template <typename T>
constexpr T sign(T value) {
	return value < 0 ? -1 : 1;
}

/**
 * @brief Compute the intersection points between a circle and a line segment.
 * Determines where (if anywhere) the closed segment from `p1` to `p2`
 * intersects the circle centered at `center` with radius `radius`.
 * Intersection points that fall outside the segment bounds are discarded.
 * Segment endpoints that lie on the circle count as intersections.
 *
 * @param center Center of the circle.
 * @param radius Radius of the circle (same units as the point coordinates).
 * @param p1 First endpoint of the line segment.
 * @param p2 Second endpoint of the line segment.
 * 
 * @return A vector containing 0, 1, or 2 intersection points that lie on the
 * segment. In a tangential (single-touch) case, floating-point effects
 * may produce two nearly identical points; callers may wish to
 * deduplicate.
 */
std::vector<point> line_circle_intersections(point center, float radius, point p1, point p2);

/**
 * @brief Checks to see whether specified text file exists on SD card.
 * Prints to console if file not found.
 * @param file_name The name of file on SD. 
 * @return True if the file is found
 */
bool SD_text_file_exists(const std::string& file_name);

/**
 * @brief Makes a file on the SD card have no data.
 * @param file_name The name of file on SD. MUST USE .TXT FILE.
 */
void wipe_SD_file(const std::string& file_name);

/**
 * @brief Appends a string to a file on SD on a newline.
 * @param file_name The name of file on SD. MUST USE .TXT FILE.
 */
void write_to_SD_file(const std::string& file_name, const std::string& data);

/**
 * @brief Removes a line of text that contains the duplicate word.
 * For example a file that contains Hello, helo, hello, eloworld 
 * will have all its contents removed if "el" is used as duplicate word
 * or nothing removed if "HelloWorld" is used.
 * @param file_name The name of file on SD. MUST USE .TXT FILE.
 * @param duplicate_word The word needed to be found to have line removed
 */
void remove_duplicates_SD_file(const std::string& file_name, const std::string& duplicate_word);

/**
 * @brief Catagorizes each line of the text file into a vector
 * @param file_name The name of file on SD. MUST USE .TXT FILE.
 * @return A vector that contains each line of the text file
 */
std::vector<std::string> get_SD_file_txt(const std::string& file_name);

namespace mik {
    enum class color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, BRIGHT_BLACK, BRIGHT_RED, BRIGHT_GREEN, BRIGHT_YELLOW, BRIGHT_BLUE, BRIGHT_MAGENTA, BRIGHT_CYAN, BRIGHT_WHITE }; 
    constexpr color black         = color::BLACK;
    constexpr color red           = color::RED;
    constexpr color green         = color::GREEN;
    constexpr color yellow        = color::YELLOW;
    constexpr color blue          = color::BLUE;
    constexpr color magenta       = color::MAGENTA;
    constexpr color cyan          = color::CYAN;
    constexpr color white         = color::WHITE;
    constexpr color bright_black  = color::BRIGHT_BLACK;
    constexpr color bright_red    = color::BRIGHT_RED;
    constexpr color bright_green  = color::BRIGHT_GREEN;
    constexpr color bright_yellow = color::BRIGHT_YELLOW;
    constexpr color bright_blue   = color::BRIGHT_BLUE;
    constexpr color bright_magenta= color::BRIGHT_MAGENTA;
    constexpr color bright_cyan   = color::BRIGHT_CYAN;
    constexpr color bright_white  = color::BRIGHT_WHITE;
}

/** @brief Prints to terminal via serial.
 * @param data The datatype to be printed to terminal.
 * @param clr The color of the text in the terminal (default is white).
 */
void print(float num, const mik::color& clr = mik::color::WHITE);
void print(int num, const mik::color& clr = mik::color::WHITE);
void print(double num, const mik::color& clr = mik::color::WHITE);
void print(std::string str, const mik::color& clr = mik::color::WHITE);
void print(const char* str, const mik::color& clr = mik::color::WHITE);
void print(bool boolean, const mik::color& clr = mik::color::WHITE);
void print(long num, const mik::color& clr = mik::color::WHITE);
void print(long long num, const mik::color& clr = mik::color::WHITE);
void print(unsigned long long num, const mik::color& clr = mik::color::WHITE);
void print(unsigned int num, const mik::color& clr = mik::color::WHITE);
void print(char c, const mik::color& clr = mik::color::WHITE);

/** @brief Converts a integer port ex. PORT1, into triport format.
 *  @return Triport port.
*/
vex::triport::port& to_triport(int port);

std::string to_string_float(float num, int precision = 5, bool remove_trailing_zero = true);

inline std::string to_string(std::nullptr_t) { return "nullptr"; }
inline std::string to_string(bool v) { return v ? "true" : "false"; }
inline std::string to_string(char c) { return std::string(1, c); }
inline std::string to_string(const char* s) { return s ? std::string(s) : "<null>"; }
inline std::string to_string(char* s) { return s ? std::string(s) : "<null>"; }
inline const std::string& to_string(const std::string& s) { return s; }
inline std::string&& to_string(std::string&& s) { return std::move(s); }

template<typename T>
auto to_string(const T& value) -> typename std::enable_if<
    !std::is_pointer<T>::value &&
    !std::is_same<T, bool>::value &&
    !std::is_same<T, char>::value &&
    !std::is_same<T, std::nullptr_t>::value,
    std::string>::type
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}