#include "vex.h"

std::vector<std::vector<float>> graph_buffer;

void test_palette() {
  UI_init();
  UI_render();
}

void test() {
  Brain.Screen.printAt(30, 30, "test");
  exit(1);
}

void test_spin_all_motors(std::vector<hzn::motor_group> motor_chains) {
    for (hzn::motor_group& motor_chain : motor_chains) {
      for (vex::motor& motor : motor_chain.motors) {
          Brain.Screen.clearLine();
          Brain.Screen.print("Spinning motor at PORT %d", (motor.index() + 1));
          Brain.Screen.newLine();
          motor.spinFor(1, sec);
          task::sleep(2000);
      }
      task::sleep(2000);
    }
}

void test_print_motor_torque(std::vector<hzn::motor_group> motor_chains) {
  while (true) {
    int y_pos = 50;
    for (hzn::motor_group& motor_chain : motor_chains) {
      for (vex::motor& motor : motor_chain.motors) {
        Brain.Screen.printAt(0, y_pos, "PORT %d TORQUE: %d", motor.index() + 1, motor.torque());
        y_pos += 20;
      }
    }
    task::sleep(2000);
  }
}

void test_serial_output() {
    double count = 0;
    while (true) {
        printf("%f\n", count);
        fflush(stdout);
        wait(100, msec);
        count++;
    }
}

void print_coordinates(){
  chassis.set_coordinates(0, 0, 0);

  UI_console_scr_add("X: ", [](){ return chassis.get_X_position(); });
  UI_console_scr_add("Y: ", [](){ return chassis.get_Y_position(); });
  UI_console_scr_add("Heading: ", [](){ return chassis.get_absolute_heading(); });
  UI_console_scr_add("ForwardTracker: ", [](){ return chassis.get_ForwardTracker_position(); });
  UI_console_scr_add("SidewaysTracker: ", [](){ return chassis.get_SidewaysTracker_position(); });
}

void add_to_graph_buffer(std::vector<float> data) {
  graph_buffer.push_back(data);
}

void clear_graph_buffer() {
  graph_buffer.clear();
}

void graph() {
  for (const auto &row : graph_buffer) {
    for (size_t i = 0; i < row.size(); ++i) {
      printf("%f", row[i]);
      if (i != row.size() - 1) {
        printf(",");
      } 
    }
    printf("\n");
    fflush(stdout);
    wait(100, msec);
  }
  clear_graph_buffer();
}