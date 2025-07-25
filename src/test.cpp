#include "vex.h"

using namespace vex;
using namespace mik;

void test_drive() {
  chassis.drive_distance(6);
  chassis.drive_distance(12);
  chassis.drive_distance(18);
  chassis.drive_distance(-36);
}

void test_heading() {
  // odom_constants();
  // chassis.set_coordinates(0, 0, 0);
  // chassis.drive_to_point(6, 12);
  // chassis.drive_to_point(24, 24);
  // chassis.drive_to_point(0, 0);

  chassis.drive_distance(10, { .heading = 15 });
  chassis.drive_distance(20, { .heading = 45 });
  chassis.drive_distance(-30, { .heading = 0 });
}

void test_turn() {
  chassis.turn_to_angle(5, {.min_voltage = 5, .settle_time = 0, .settle_error = 3});
  chassis.turn_to_angle(30, {.min_voltage = 5, .settle_time = 0, .settle_error = 3});
  chassis.turn_to_angle(90, {.min_voltage = 5, .settle_time = 0, .settle_error = 3});
  chassis.turn_to_angle(225, {.min_voltage = 5, .settle_time = 0, .settle_error = 3});
  chassis.turn_to_angle(360, {.min_voltage = 5, .settle_time = 0, .settle_error = 3});
}

void test_swing() {
  chassis.left_swing_to_angle(110);
  chassis.right_swing_to_angle(0);
}


void test_full() {
  chassis.drive_distance(24);
  chassis.turn_to_angle(-45);
  chassis.drive_distance(-36);
  chassis.right_swing_to_angle(-90);
  chassis.drive_distance(24);
  chassis.turn_to_angle(0);
}

void test_odom_drive() {
  chassis.set_coordinates(0, 0, 0);
  chassis.drive_to_point(0, 6);
  chassis.drive_to_point(0, 18);
  chassis.drive_to_point(0, 36);
  chassis.drive_to_point(0, 0);
}

void test_odom_turn() {
  chassis.set_coordinates(0, 0, 0);
  chassis.turn_to_point( 9.96,  0.87);
  chassis.turn_to_point( 8.66,  5);
  chassis.turn_to_point( 0, 10);
  chassis.turn_to_point(-7.07, -7.07);
  chassis.turn_to_point(10,  0);
}

void test_odom() {}

void test_odom_heading() {
  chassis.set_coordinates(0, 0, 0);
  chassis.drive_to_point(5, 18);
  chassis.drive_to_point(20, 35);
  chassis.drive_to_point(0, 0);
}

void test_odom_boomerang() {
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.drive_settle_error = 1;
  chassis.drive_to_pose(24, 24, 90);
  // chassis.drive_to_pose(0, 24, 90);
  // chassis.drive_to_pose(24, 0, 135);
  // chassis.drive_to_point(0, 0);
  // chassis.turn_to_angle(0);
}
 
void test_odom_full() {
  odom_constants();
  chassis.set_coordinates(0, 0, 0);
  chassis.drive_to_point(0, 24);
  chassis.turn_to_point(26.833, 0, { .angle_offset = 180 });
  chassis.drive_to_point(26.833, 0);
  chassis.turn_to_point(0, 0);
  chassis.drive_to_point(0, 0);
  chassis.turn_to_angle(0);
}

pid_data data;
std::vector<std::string> error_data;
static vex::task user_control_task;
static vex::task update_controller_scr;
static vex::task pid_tuner_task;
static vex::task test_movements_task;
static float predicted_distance = 0;
static float prev_desired_distance = 0;
static std::function<void()> test_movements_func;

void config_test_drive() {
  data.variables = { {"drive_kp: ", chassis.drive_kp}, {"drive_ki: ", chassis.drive_ki}, {"drive_kd: ", chassis.drive_kd}, {"drive_stl_err: ", chassis.drive_settle_error}, 
    {"drive_stl_tm: ", chassis.drive_settle_time}, {"drive_tmout: ", chassis.drive_timeout}, {"drive_starti: ", chassis.drive_starti}, {"drive_max_volt: ", chassis.drive_max_voltage}
  };
  graph_scr->set_plot_bounds(-30, 50, 0, 15000, 1, 1);
  graph_scr->set_plot({
      [](double x){ return chassis.get_ForwardTracker_position(); }, 
      [](double x){ 
        if (chassis.desired_distance != prev_desired_distance) {
          predicted_distance += chassis.desired_distance;
          prev_desired_distance = chassis.desired_distance; 
        }
        return predicted_distance; 
      }
    },
    {{"Actual", 0x002E8B59}, 
    {"SetPoint", 0x00FA8072}}
  );
  UI_select_scr(graph_scr->get_graph_screen()); 

  test_movements_func = [](){
    chassis.forward_tracker.resetPosition();
    predicted_distance = 0;
    prev_desired_distance = 0;
    graph_scr->reset_graph();
    graph_scr->graph();

    test_drive();
  };

  PID_tuner();
}

void config_test_turn() {  
  data.variables = { {"turn_kp: ", chassis.turn_kp}, {"turn_ki: ", chassis.turn_ki}, {"turn_kd: ", chassis.turn_kd}, {"turn_stl_err: ", chassis.turn_settle_error}, 
    {"turn_stl_tm: ", chassis.turn_settle_time}, {"turn_tmout: ", chassis.turn_timeout}, {"turn_starti: ", chassis.turn_starti}, {"turn_max_volt: ", chassis.turn_max_voltage} };
  graph_scr->set_plot_bounds(-10, 370, 0, 5000, 1, 1);
  graph_scr->set_plot({
    [](double x){ return chassis.get_absolute_heading(); }, 
    [](double x){ return chassis.desired_angle; }},
    {{"Actual", 0x002E8B59}, 
    {"SetPoint", 0x00FA8072}}
  );
  UI_select_scr(graph_scr->get_graph_screen()); 

  test_movements_func = [](){
    chassis.set_heading(0);
    graph_scr->reset_graph();
    graph_scr->graph();

    test_turn();
  };

  PID_tuner();
}

void config_test_swing() {
  data.variables = { {"swing_kp: ", chassis.swing_kp }, {"swing_ki: ", chassis.swing_ki }, {"swing_kd: ", chassis.swing_kd}, {"swing_stl_err: ", chassis.swing_settle_error}, 
    {"swing_stle_tm: ", chassis.swing_settle_time}, {"swing_tmout: ", chassis.swing_timeout}, {"swing_starti: ", chassis.swing_starti}, {"swing_max_volt: ", chassis.turn_max_voltage} };
  graph_scr->set_plot_bounds(0, 360, 0, 3000, 1, 1);
  graph_scr->set_plot({
    [](double x){ return chassis.get_absolute_heading(); }, 
    [](double x){ return chassis.desired_angle; }},
    {{"Actual", 0x002E8B59}, 
    {"SetPoint", 0x00FA8072}}
  );
  UI_select_scr(graph_scr->get_graph_screen()); 

  test_movements_func = [](){
    chassis.set_heading(0);
    graph_scr->reset_graph();
    graph_scr->graph();
    
    test_swing();
  };

  PID_tuner();
}

void config_test_heading() {
  data.variables = { {"heading_kp: ", chassis.heading_kp}, {"heading_ki: ", chassis.heading_ki}, {"heading_kd: ", chassis.heading_kd}, {"heading_starti: ", chassis.heading_starti}, {"max_volt: ", chassis.heading_max_voltage} };
  graph_scr->set_plot_bounds(-30, 60, 0, 3000, 1, 1);
  graph_scr->set_plot({
    [](double x){ return chassis.inertial.rotation(); }, 
    [](double x){ return chassis.desired_heading; }},
    {{"Actual", 0x002E8B59}, 
    {"SetPoint", 0x00FA8072}}
  );
  UI_select_scr(graph_scr->get_graph_screen()); 

  test_movements_func = [](){
    chassis.set_heading(0);
    graph_scr->reset_graph();
    graph_scr->graph();

    test_heading();
  };

  PID_tuner();
}

void config_test_odom() {

}

inline int get_flicker_index(const std::string& value_str, float place) {
  int dot_pos = value_str.find('.');
  if (dot_pos == (int)std::string::npos) {
      int idx = value_str.size() - 1 - place; 
      return idx;
  }
  else {
    int idx;
    if (place > 0) {
        idx = dot_pos + place; 
    }
    else {
        idx = dot_pos - 1 + place;
    }
    return idx;
  }
}

inline int get_power(float n) {
    if (n <= 0) { return 1; }
    int power = 1;
    while (n >= 10) {
        n /= 10;
        power *= 10;
    }
    return power;
}

void PID_tuner() {
  auton_scr->disable_controller_overlay();
  disable_user_control = true;
  vex::task test;

  user_control_task = vex::task([](){
    while(1) {
      chassis.control(chassis.selected_drive_mode);
      vex::this_thread::sleep_for(5);
    }
    return 0;
  });

  pid_tuner_task = vex::task([](){
    static int flicker = 0;
    while(1) {
    data.max = std::max(3, data.index+1);
    data.min = data.max - 3;

    int j = 0;
    for(int i = data.min; i < data.max; ++i) {
      Controller.Screen.setCursor(j+1, 1);
      j++;
      std::string var = to_string_float(data.variables[i].second, 3, false);

      if (data.index == i) {
        flicker++;
        if(flicker % 2 == 0) {
          int idx = get_flicker_index(var, -std::log10(1.0 / data.modifer_scale));
          if(idx >= 0 && idx < (int)var.size()) {
            if(std::isdigit(var[idx])) {
              if(var[idx] == '1') {
                var[idx] = '-';
              } else {
                var[idx] = '_';
              }
            }
          }
        }
        else{}
      }

      Controller.Screen.print((data.variables[i].first + var).c_str());
      
      if (data.index == i) { 
        data.var_upper_size = get_power(data.variables[i].second);

        if (data.needs_update) {
          remove_duplicates_SD_file("pid_data.txt", data.variables[i].first);
          data.variables[i].second += data.modifier;
          write_to_SD_file("pid_data.txt", (data.variables[i].first + to_string(data.variables[i].second)));
          data.needs_update = false;
        }
        Controller.Screen.print("<            "); 
      } else { 
        Controller.Screen.print("             "); 
      }
    }

    this_thread::sleep_for(20);
    }
    return 0;
  });
  update_controller_scr = vex::task([](){
    while(1) {
      if (Controller.ButtonUp.pressing()) {
        if (data.index > 0) { data.index--; }
        data.modifer_scale = 1;
        task::sleep(200);
      }
      if (Controller.ButtonDown.pressing()) {
        if (data.index < data.variables.size() - 1) { data.index++; }
        data.modifer_scale = 1;
        task::sleep(200);
      }
      if (Controller.ButtonRight.pressing()) {
        data.modifier = 1 / data.modifer_scale;
        data.needs_update = true;
        task::sleep(200);
      }
      if (Controller.ButtonLeft.pressing()) {
        data.modifier = -1 / data.modifer_scale;
        data.needs_update = true;
        task::sleep(200);
      }
      if (Controller.ButtonY.pressing()) {
        data.modifer_scale /= 10;
        if (data.modifer_scale < (1 / data.var_upper_size)) {
          data.modifer_scale = (1 / data.var_upper_size);
        }
        task::sleep(200);
      }
      if (Controller.ButtonA.pressing()) {
        data.modifer_scale *= 10;
        if (data.modifer_scale > 1000) {
          data.modifer_scale = 1000;
        }
        task::sleep(200);
      }
      if (Controller.ButtonX.pressing()) {
        user_control_task.suspend();
        test_movements_task = vex::task([](){
          test_movements_func();
          return 0;
        });
      }
      if (Controller.ButtonB.pressing()) {
        user_control_task.resume();
        test_movements_task.stop();
        chassis.stop_drive(vex::coast);
        task::sleep(200);
      }
      task::sleep(20);
    }
    return 0;
  });
}

static std::vector<mik::motor> motors_;

void config_add_motors(std::vector<std::vector<mik::motor>> motor_groups) {
  for (auto& motors : motor_groups) {
    for (auto& motor : motors) {
      motors_.push_back(motor);
    }
  }
}

void config_add_motors(std::vector<mik::motor> motors) {
  for (auto& motor : motors) {
    motors_.push_back(motor);
  }
}

int run_diagnostic() {
  error_data.clear();
  int errors = 0;
  if (!Brain.SDcard.isInserted()) {
    error_data.push_back("SD is not inserted");
    errors++;
  }
  if (!chassis.inertial.installed()) {
    std::string port = to_string(chassis.inertial.index() + 1);
    error_data.push_back("Inertial [PORT" + port + "] is disconnected");
    errors++;
  }
  if (!chassis.forward_tracker.installed()) {
    std::string port = to_string(chassis.forward_tracker.index() + 1);
    error_data.push_back("Forward Tracker [PORT" + port + "] is disconnected");
    errors++;
  }
  if (!chassis.sideways_tracker.installed()) {
    std::string port = to_string(chassis.sideways_tracker.index() + 1);
    error_data.push_back("Sideways Tracker [PORT" + port + "] is disconnected");
    errors++;
  }
  for (auto& motor : motors_) {
    if (!motor.installed()) {
      error_data.push_back(motor.name() + " [" + motor.port() +  "] is disconnected");
      errors++;
    }   
  }
  if (errors <= 0) {
    error_data.push_back("No issues found");
  }

  return errors;
}

void config_add_pid_output_SD_console() {
  if (!Brain.SDcard.isInserted()) { return; }
  UI_select_scr(console_scr->get_console_screen());
  console_scr->reset();
  vex::task e([](){
    task::sleep(500);

    std::vector<std::string> data_arr = get_SD_file_txt("auton.txt");
    for (const auto& line : data_arr) {
      console_scr->add(line, false);
    }
    return 0;
  });
}

void config_spin_all_motors() {
  UI_select_scr(console_scr->get_console_screen()); 
  console_scr->reset();
  disable_user_control = true;
  vex::task spin_mtrs([](){
    task::sleep(500);
    for (mik::motor& motor : motors_) { 
      std::string data = (motor.name() + ": " + motor.port() + ", fwd, 6 volt");
      console_scr->add(std::string(data), [](){ return ""; });
      motor.spin(fwd, 6, volt);
      vex::task::sleep(1000);
      motor.stop();
      vex::task::sleep(1000);
    }
    disable_user_control = false;
    return 0;
  });
}

void config_motor_wattage() {
  console_scr->reset();
  UI_select_scr(console_scr->get_console_screen()); 

  vex::task watt([](){
    task::sleep(500);
    console_scr->add("right_drive: ", []() { return chassis.right_drive.averagePower(); });
    console_scr->add("left_drive: ", []() { return chassis.left_drive.averagePower(); });
  
    for (auto& motor : motors_) {
      console_scr->add(motor.name() + ": ", [&motor]() { return motor.power(); });
    }
    return 0;
  });
}

void config_motor_temp() {
  console_scr->reset();
  UI_select_scr(console_scr->get_console_screen()); 

  
  vex::task temp([](){
    task::sleep(500);
    
    console_scr->add("right_drive: ", []() { return to_string_float(chassis.right_drive.averageTemperature(), 0, true) + "%% overheated"; });
    console_scr->add("left_drive: ", []() { return to_string_float(chassis.left_drive.averageTemperature(), 0, true) + "%% overheated"; });
    for (auto& motor : motors_) {
      console_scr->add(motor.name() + ": ", [&motor]() { return to_string_float(motor.temperature(), 0, true) + "%% overheated"; });
    }
    return 0;
  });

}

void config_odom_data() {
  if (!chassis.position_tracking) {
    chassis.set_coordinates(0, 0, 0);
  }

  console_scr->add("X: ", [](){ return chassis.get_X_position(); });
  console_scr->add("Y: ", [](){ return chassis.get_Y_position(); });
  console_scr->add("Heading: ", [](){ return chassis.get_absolute_heading(); });
  console_scr->add("Forward_Tracker: ", [](){ return chassis.get_ForwardTracker_position(); });
  console_scr->add("Sideways_Tracker: ", [](){ return chassis.get_SidewaysTracker_position(); });

  UI_select_scr(console_scr->get_console_screen()); 
}

void config_error_data() {
  console_scr->reset();
  UI_select_scr(console_scr->get_console_screen()); 
  
  vex::task add_errors([](){
    task::sleep(500);
    for (const auto& error : error_data) {
      console_scr->add(error);  
    }
    return 0;
  });
}

void config_skills_driver_run() {
  auton_scr->disable_controller_overlay();
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("SKILLS DRIVER RUN               ");
  task::sleep(1000);
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("             3                 ");
  Controller.rumble(".");
  task::sleep(1000);
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("             2                 ");
  Controller.rumble(".");
  task::sleep(1000);
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("             1                 ");
  Controller.rumble(".");
  task::sleep(1000);
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("            GO                 ");
  Controller.rumble("-");
  Controller.Screen.setCursor(1, 1);
  Controller.Screen.print("                               ");

  vex::task timer([](){
    float start_time = Brain.Timer.time(vex::timeUnits::sec);
    float current_time = start_time;
    float max_time = 60;
    float elapsed_time = 0;
    int time_remaining = 0;
    while(1) {
      current_time = Brain.Timer.time(vex::timeUnits::sec);
      elapsed_time = current_time - start_time;
      time_remaining = max_time - elapsed_time;

      switch (time_remaining)
      {
      case 30:
        Controller.rumble((".-"));
      case 15:
        Controller.rumble(("."));
        break;
      case 5:
        Controller.rumble((".-"));
        break;
      case 0:
        Controller.rumble(("."));
        chassis.stop_drive(vex::coast);
        disable_user_control = true;
        std::abort();
        break;
      default:
        break;
      }
      
      Controller.Screen.setCursor(1, 1);
      Controller.Screen.print("           ");
      Controller.Screen.print(time_remaining);
      Controller.Screen.print("  ");
    }
    return 0;
  });
}

void config_test_three_wire_port(port port) {
  vex::digital_out dig_out = Brain.ThreeWirePort.Port[port];
  dig_out.set(!dig_out.value());
}
