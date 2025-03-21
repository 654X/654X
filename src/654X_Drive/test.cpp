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
          motor.spinFor(1, vex::sec);
          vex::task::sleep(2000);
      }
      vex::task::sleep(2000);
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
    vex::task::sleep(2000);
  }
}

void test_serial_output() {
    double count = 0;
    while (true) {
        printf("%f\n", count);
        fflush(stdout);
        vex::task::sleep(100);
        count++;
    }
}

int print_vector_to_serial(const std::string& name, const std::vector<float>& vector) {
    printf("%s\n", name.c_str());
    fflush(stdout);
    vex::task::sleep(100);

    for(size_t i = 0; i < vector.size(); ++i) {
      printf("%f\n", vector[i]);
      fflush(stdout);
      vex::task::sleep(50);
    }
    printf("%f\n\n");
    fflush(stdout);
    vex::task::sleep(50);
    return 0;
}

void print_coordinates(){
  // chassis.set_coordinates(0, 0, 0);
  // vex::distance dis = vex::distance(vex::PORT21);
  while(true){
    Brain.Screen.clearScreen();
    Brain.Screen.printAt(0, 50, "X: %f", chassis.get_X_position());
    Brain.Screen.printAt(0, 70, "Y: %f", chassis.get_Y_position());
    Brain.Screen.printAt(0, 90, "Heading: %f", chassis.get_absolute_heading());
    Brain.Screen.printAt(0, 110, "ForwardTracker: %f", chassis.get_ForwardTracker_position());
    Brain.Screen.printAt(0, 130, "SidewaysTracker: %f", chassis.get_SidewaysTracker_position());
    // float hypotnuse = dis.objectDistance(vex::inches);
    // Brain.Screen.printAt(0, 150, "Distance sensor: %f", hypotnuse);
    // float theta = to_rad(chassis.get_absolute_heading());

    // Brain.Screen.printAt(0, 170, "X adjusted: %f", -hypotnuse * sin(theta));
    // Brain.Screen.printAt(0, 190, "Y adjusted: %f", 62 + (hypotnuse * cos(theta) - 7.7));
    vex::task::sleep(20);
  }
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
    vex::task::sleep(100);
  }
  clear_graph_buffer();
}