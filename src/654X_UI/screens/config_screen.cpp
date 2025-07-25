#include "vex.h"

using namespace mik;

#define test_slot_color 0x00323232
#define test_slot_border_color 0x00666666

#define data_slot_color 0x00232323
#define data_slot_border_color 0x00666666

#define macro_slot_color 0
#define macro_slot_border_color 0x00666666

#define text_alignment text_align::CENTER

UI_config_screen::UI_config_screen() {
    UI_crt_config_scr();
    UI_crt_pnematics_scr();
}

std::shared_ptr<screen> UI_config_screen::get_config_screen() {
    return(UI_config_scr);
}

void UI_config_screen::UI_crt_config_scr() {
    UI_config_scr = UI_crt_scr(0, 45, SCREEN_WIDTH, SCREEN_HEIGHT + 5);
    UI_config_scr->add_scroll_bar(UI_crt_rec(0, 0, 3, 40, 0x00434343, UI_distance_units::pixels), screen::alignment::RIGHT);
    auto bg = UI_crt_bg(UI_crt_rec(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, vex::color::black, UI_distance_units::pixels));


    auto macro_1 = UI_crt_txtbox("Run Auto", text_alignment, UI_crt_rec(6, 54, 150, 31, macro_slot_color, UI_distance_units::pixels));
    auto macro_1_bg = UI_crt_btn(UI_crt_rec(4, 52, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [this, macro_1](){ countdown(macro_1, [](){ auton_scr->start_auton(); }); });
        macro_1_bg->set_states(UI_crt_rec(4, 52, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(4, 52, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));

    auto macro_2_bg = UI_crt_btn(UI_crt_rec(163, 52, 154, 35, data_slot_border_color, UI_distance_units::pixels), [](){ run_diagnostic(); config_error_data(); });
        macro_2_bg->set_states(UI_crt_rec(163, 52, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(163, 52, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_2 = UI_crt_txtbox("Error Data", text_alignment, UI_crt_rec(165, 54, 150, 31, data_slot_color, UI_distance_units::pixels));
    

    auto macro_3_bg = UI_crt_btn(UI_crt_rec(322, 52, 154, 35, test_slot_border_color, UI_distance_units::pixels), [](){ config_test_drive(); });
        macro_3_bg->set_states(UI_crt_rec(322, 52, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(322, 52, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_3 = UI_crt_txtbox("Test Drive", text_alignment, UI_crt_rec(324, 54, 150, 31, test_slot_color, UI_distance_units::pixels));


    macro_4_bg_tgl = UI_crt_tgl(UI_crt_rec(4, 91, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [](){});
    auto macro_4_bg = UI_crt_btn(UI_crt_rec(4, 91, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [](){ auton_scr->enable_time_limit(); auton_scr->flip_toggle_controller({3, 0}, auton_scr->time_limit); auton_scr->save_auton_SD(); });
        macro_4_bg_tgl->set_states(UI_crt_rec(4, 91, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(4, 91, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_4 = UI_crt_txtbox("Time Cap Auto", text_alignment, UI_crt_rec(6, 93, 150, 31, macro_slot_color, UI_distance_units::pixels));


    auto macro_5_bg = UI_crt_btn(UI_crt_rec(163, 91, 154, 35, data_slot_border_color, UI_distance_units::pixels), [](){ config_motor_temp(); });
        macro_5_bg->set_states(UI_crt_rec(163, 91, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(163, 91, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_5 = UI_crt_txtbox("Motor Temps", text_alignment, UI_crt_rec(165, 93, 150, 31, data_slot_color, UI_distance_units::pixels));


    auto macro_6_bg = UI_crt_btn(UI_crt_rec(322, 91, 154, 35, test_slot_border_color, UI_distance_units::pixels), [](){ config_test_turn(); });
        macro_6_bg->set_states(UI_crt_rec(322, 91, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(322, 91, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_6 = UI_crt_txtbox("Test Turn", text_alignment, UI_crt_rec(324, 93, 150, 31, test_slot_color, UI_distance_units::pixels));


    auto macro_7_bg = UI_crt_btn(UI_crt_rec(4, 91+39, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [](){ config_skills_driver_run(); });
        macro_7_bg->set_states(UI_crt_rec(4, 91+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(4, 91+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_7 = UI_crt_txtbox("Driver Skills", text_alignment, UI_crt_rec(6, 93+39, 150, 31, macro_slot_color, UI_distance_units::pixels));


    auto macro_8_bg = UI_crt_btn(UI_crt_rec(163, 91+39, 154, 35, data_slot_border_color, UI_distance_units::pixels), [](){ config_motor_wattage(); });
    macro_8_bg->set_states(UI_crt_rec(163, 91+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(163, 91+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_8 = UI_crt_txtbox("Motor Wattage", text_alignment, UI_crt_rec(165, 93+39, 150, 31, data_slot_color, UI_distance_units::pixels));
    
    
    auto macro_9_bg = UI_crt_btn(UI_crt_rec(322, 91+39, 154, 35, test_slot_border_color, UI_distance_units::pixels), [](){ config_test_swing(); });
    macro_9_bg->set_states(UI_crt_rec(322, 91+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(322, 91+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_9 = UI_crt_txtbox("Test Swing", text_alignment, UI_crt_rec(324, 93+39, 150, 31, test_slot_color, UI_distance_units::pixels));
    
    
    macro_10_bg_tgl = UI_crt_tgl(UI_crt_rec(4, 91+39+39, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [](){});
    auto macro_10_bg = UI_crt_btn(UI_crt_rec(4, 91+39+39, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [](){ auton_scr->UI_select_auton(autons::OFF_SKILLS); auton_scr->flip_toggle_controller({1, 1}, auton_scr->off_skills); auton_scr->save_auton_SD(); });
    macro_10_bg_tgl->set_states(UI_crt_rec(4, 91+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(4, 91+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_10 = UI_crt_txtbox("Auto Skills", text_alignment, UI_crt_rec(6, 93+39+39, 150, 31, macro_slot_color, UI_distance_units::pixels));
    
    
    auto macro_11_bg = UI_crt_btn(UI_crt_rec(163, 91+39+39, 154, 35, data_slot_border_color, UI_distance_units::pixels), [](){ config_odom_data(); });
        macro_11_bg->set_states(UI_crt_rec(163, 91+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(163, 91+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_11 = UI_crt_txtbox("Odom Data", text_alignment, UI_crt_rec(165, 93+39+39, 150, 31, data_slot_color, UI_distance_units::pixels));

    auto macro_12_bg = UI_crt_btn(UI_crt_rec(322, 91+39+39, 154, 35, test_slot_border_color, UI_distance_units::pixels), [](){ test_full(); });
    macro_12_bg->set_states(UI_crt_rec(322, 91+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(322, 91+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_12 = UI_crt_txtbox("Test Full", text_alignment, UI_crt_rec(324, 93+39+39, 150, 31, test_slot_color, UI_distance_units::pixels));
    
    
    auto macro_13_bg = UI_crt_btn(UI_crt_rec(4, 91+39+39+39, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [](){ config_spin_all_motors(); });
    macro_13_bg->set_states(UI_crt_rec(4, 91+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(4, 91+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_13 = UI_crt_txtbox("Spin Motors", text_alignment, UI_crt_rec(6, 93+39+39+39, 150, 31, macro_slot_color, UI_distance_units::pixels));
    
    auto macro_14_bg = UI_crt_btn(UI_crt_rec(163, 91+39+39+39, 154, 35, data_slot_border_color, UI_distance_units::pixels), [](){ config_add_pid_output_SD_console(); });
        macro_14_bg->set_states(UI_crt_rec(163, 91+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(163, 91+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_14 = UI_crt_txtbox("PID Data", text_alignment, UI_crt_rec(165, 93+39+39+39, 150, 31, data_slot_color, UI_distance_units::pixels));
    
    auto macro_15_bg = UI_crt_btn(UI_crt_rec(322, 91+39+39+39, 154, 35, test_slot_border_color, UI_distance_units::pixels), [](){ test_odom(); });
    macro_15_bg->set_states(UI_crt_rec(322, 91+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(322, 91+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_15 = UI_crt_txtbox("Test Odom", text_alignment, UI_crt_rec(324, 93+39+39+39, 150, 31, test_slot_color, UI_distance_units::pixels));
    
    
    auto macro_16_bg = UI_crt_btn(UI_crt_rec(4, 91+39+39+39+39, 154, 35, macro_slot_border_color, UI_distance_units::pixels), [this](){ UI_swap_screens({UI_pnematics_scr}); disable_user_control = true; });
    macro_16_bg->set_states(UI_crt_rec(4, 91+39+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(4, 91+39+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_16 = UI_crt_txtbox("Pnematic Menu", text_alignment, UI_crt_rec(6, 93+39+39+39+39, 150, 31, macro_slot_color, UI_distance_units::pixels));
    
    auto macro_17_bg = UI_crt_btn(UI_crt_rec(163, 91+39+39+39+39, 154, 35, data_slot_border_color, UI_distance_units::pixels), [](){ wipe_SD_file("PID_data.txt"); });
        macro_17_bg->set_states(UI_crt_rec(163, 91+39+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(163, 91+39+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_17 = UI_crt_txtbox("Wipe PID Data", text_alignment, UI_crt_rec(165, 93+39+39+39+39, 150, 31, data_slot_color, UI_distance_units::pixels));


    auto macro_18_bg = UI_crt_btn(UI_crt_rec(322, 91+39+39+39+39, 154, 35, test_slot_border_color, UI_distance_units::pixels), [](){ config_test_heading(); });
        macro_18_bg->set_states(UI_crt_rec(322, 91+39+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(322, 91+39+39+39+39, 154, 35, 0x00B6B6B6, UI_distance_units::pixels));
    auto macro_18 = UI_crt_txtbox("test_slot_6", text_alignment, UI_crt_rec(324, 93+39+39+39+39, 150, 31, test_slot_color, UI_distance_units::pixels));

    UI_config_scr->add_UI_components({bg, 
        macro_1_bg, macro_1, macro_2_bg, macro_2, macro_3_bg, macro_3,
        macro_4_bg, macro_4_bg_tgl, macro_4, macro_5_bg, macro_5, macro_6_bg, macro_6,
        macro_7_bg, macro_7, macro_8_bg, macro_8, macro_9_bg, macro_9,
        macro_10_bg, macro_10_bg_tgl, macro_10, macro_11_bg, macro_11, macro_12_bg, macro_12,
        macro_13_bg, macro_13, macro_14_bg, macro_14, macro_15_bg, macro_15,
        macro_16_bg, macro_16, macro_17_bg, macro_17, macro_18_bg, macro_18,
    });

    for (const auto& component : UI_config_scr->get_UI_components()) {
        component->set_y_pos(component->get_y_pos() - 45);
    }
}

void UI_config_screen::UI_crt_pnematics_scr() {
    UI_pnematics_scr = UI_crt_scr(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    auto bg = UI_crt_bg(UI_crt_rec(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, vex::color::black, UI_distance_units::pixels));

    auto title_txt = UI_crt_gfx(UI_crt_txt("Pneumatics Menu", 169, 30, UI_distance_units::pixels));

    auto port_A_btn = UI_crt_btn(UI_crt_img("", .12*96, .51*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_A); });
    auto port_A = UI_crt_tgl(UI_crt_grp({UI_crt_rec(.12*96, .51*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_A_triangle.png", .27, .64, 1, .83, UI_distance_units::inches)}), [](){});
        port_A->set_states(UI_crt_img("port_A_triangle_bg.png", .12, .51, 1, .83, UI_distance_units::inches), UI_crt_img("port_A_triangle_bg.png", .12, .51, 1, .83, UI_distance_units::inches));

    auto port_B_btn = UI_crt_btn(UI_crt_img("", 1.33*96, .51*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_B); });
    auto port_B = UI_crt_tgl(UI_crt_grp({UI_crt_rec(1.33*96, .51*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_B_triangle.png", 1.48, .64, 1, .83, UI_distance_units::inches)}), [](){});
        port_B->set_states(UI_crt_img("port_B_triangle_bg.png", 1.33, .51, 1, .83, UI_distance_units::inches), UI_crt_img("port_B_triangle_bg.png", 1.33, .51, 1, .83, UI_distance_units::inches));

    auto port_C_btn = UI_crt_btn(UI_crt_img("", 2.59*96, .51*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_C); });
    auto port_C = UI_crt_tgl(UI_crt_grp({UI_crt_rec(2.59*96, .51*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_C_triangle.png", 2.74, .64, 1, .83, UI_distance_units::inches)}), [](){});
        port_C->set_states(UI_crt_img("port_C_triangle_bg.png", 2.59, .51, 1, .83, UI_distance_units::inches), UI_crt_img("port_C_triangle_bg.png", 2.59, .51, 1, .83, UI_distance_units::inches));

    auto port_D_btn = UI_crt_btn(UI_crt_img("", 3.87*96, .51*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_D); });
    auto port_D = UI_crt_tgl(UI_crt_grp({UI_crt_rec(3.87*96, .51*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_D_triangle.png", 4.01, .64, 1, .83, UI_distance_units::inches)}), [](){});
        port_D->set_states(UI_crt_img("port_D_triangle_bg.png", 3.87, .51, 1, .83, UI_distance_units::inches), UI_crt_img("port_D_triangle_bg.png", 3.87, .51, 1, .83, UI_distance_units::inches));

    auto port_E_btn = UI_crt_btn(UI_crt_img("", .12*96, 1.38*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_E); });
    auto port_E = UI_crt_tgl(UI_crt_grp({UI_crt_rec(.12*96, 1.38*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_E_triangle.png", .27, 1.47, 1, .83, UI_distance_units::inches)}), [](){});
        port_E->set_states(UI_crt_img("port_E_triangle_bg.png", .12, 1.38, 1, .83, UI_distance_units::inches), UI_crt_img("port_E_triangle_bg.png", .12, 1.38, 1, .83, UI_distance_units::inches));

    auto port_F_btn = UI_crt_btn(UI_crt_img("", 1.33*96, 1.38*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_F); });
    auto port_F = UI_crt_tgl(UI_crt_grp({UI_crt_rec(1.33*96, 1.38*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_F_triangle.png", 1.48, 1.47, 1, .83, UI_distance_units::inches)}), [](){});
        port_F->set_states(UI_crt_img("port_F_triangle_bg.png", 1.33, 1.38, 1, .83, UI_distance_units::inches), UI_crt_img("port_F_triangle_bg.png", 1.33, 1.38, 1, .83, UI_distance_units::inches));
        
    auto port_G_btn = UI_crt_btn(UI_crt_img("", 2.59*96, 1.38*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_G); });
    auto port_G = UI_crt_tgl(UI_crt_grp({UI_crt_rec(2.59*96, 1.38*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_G_triangle.png", 2.74, 1.47, 1, .83, UI_distance_units::inches)}), [](){});
        port_G->set_states(UI_crt_img("port_G_triangle_bg.png", 2.59, 1.38, 1, .83, UI_distance_units::inches), UI_crt_img("port_G_triangle_bg.png", 2.59, 1.38, 1, .83, UI_distance_units::inches));

    auto port_H_btn = UI_crt_btn(UI_crt_img("", 3.87*96, 1.38*96, 1*96, .83*96, UI_distance_units::pixels), [](){ config_test_three_wire_port(PORT_H); });
    auto port_H = UI_crt_tgl(UI_crt_grp({UI_crt_rec(3.87*96, 1.38*96, 1*96, .83*96, vex::black, UI_distance_units::pixels), UI_crt_img("port_H_triangle.png", 4.01, 1.47, 1, .83, UI_distance_units::inches)}), [](){});
        port_H->set_states(UI_crt_img("port_H_triangle_bg.png", 3.87, 1.38, 1, .83, UI_distance_units::inches), UI_crt_img("port_H_triangle_bg.png", 3.87, 1.38, 1, .83, UI_distance_units::inches));
    
    auto exit_bg = UI_crt_btn(UI_crt_rec(0, 0, 40, 40, 0x00666666, UI_distance_units::pixels), [](){ UI_select_scr(config_scr->get_config_screen()); disable_user_control = false; });
        exit_bg->set_states(UI_crt_rec(0, 0, 40, 40, 0x00B6B6B6, UI_distance_units::pixels), UI_crt_rec(0, 0, 40, 40, 0x00B6B6B6, UI_distance_units::pixels));
    auto exit_txt = UI_crt_gfx({UI_crt_rec(2, 2, 36, 36, vex::color::black, UI_distance_units::pixels), UI_crt_txt("X", 15, 25, UI_distance_units::pixels)});
    
    UI_pnematics_scr->add_UI_components({bg, title_txt, exit_bg, exit_txt, port_A_btn, port_A, port_B_btn, port_B, port_C_btn,  port_C, port_D_btn, port_D, port_E_btn,port_E, port_F_btn, port_F, port_G_btn, port_G, port_H_btn, port_H});
}

void UI_config_screen::countdown(std::shared_ptr<UI_component> txtbox, std::function<void()> func) {
    auto* txtbx = static_cast<textbox*>(txtbox.get());
    txtbox_task_data = txtbx;
    func_task = func;
    vex::task count([](){
        int count = 3;
        std::string original_txt = config_scr->txtbox_task_data->get_text();
        while(count >= 0) {
            config_scr->txtbox_task_data->set_text(to_string(count));
            vex::task::sleep(1000);
            count--;
        }
        
        config_scr->txtbox_task_data->set_text(original_txt);
        config_scr->func_task();
        return 0;
    });

}
