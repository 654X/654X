#pragma once

#include "vex.h"


void goalside_red(bool set_pos = false);
void ring_side_red(bool set_pos = false);
void win_point_red(bool set_pos = false);
void solo_win_point_red(bool set_pos = false);

void goalside_blue(bool set_pos = false);
void ring_side_blue(bool set_pos = false);
void win_point_blue(bool set_pos = false);
void solo_win_point_blue(bool set_pos = false);

void skills(void);

void drive_until_settled(float left_voltage, float right_voltage);
void distance_reset();
void LB_task(int angle);
void start_intake(bool unjam_intake = true);
void stop_intake();
void intake_ring_halfway();
void score_LB();
void color_sort_auton(color_sort color);