#include "vex.h"

std::vector<std::shared_ptr<screen>> UI_render_queue;
std::vector<std::shared_ptr<screen>> UI_render_buffer;
bool is_screen_swapping = false;

void UI_init() {

  auto main_bg = UI_crt_gfx(UI_crt_img("background_main.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, UI_distance_units::pixels));
  
  auto main_bg_scr = UI_crt_scr(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  main_bg_scr->add_UI_component(main_bg);

  auto selector_panel_scr = UI_crt_scr(0, 0, SCREEN_WIDTH + 100, 45);
  selector_panel_scr->add_scroll_bar(UI_crt_rec(0, 0, 40, 3, 0x00434343, UI_distance_units::pixels), screen::alignment::BOTTOM);

  UI_crt_console_scr();

  auto console_tgl = UI_crt_tgl(
    UI_crt_img("console_button.png", 0, 0, 160, 45, UI_distance_units::pixels),
    nullptr, 1
  );
  console_tgl->set_states(nullptr, UI_crt_img("console_button_pressed.png", 0, 0, 160, 45, UI_distance_units::pixels));
  console_tgl->set_callback([=](){ 
      UI_execute_selector_toggles(console_tgl, selector_panel_scr, true);
      UI_swap_screens({UI_console_scr, selector_panel_scr}); 
    } 
  );

  auto auton_tgl = UI_crt_tgl(
    UI_crt_img("console_button.png", 160, 0, 160, 45, UI_distance_units::pixels),
    nullptr, 1
  );
  auton_tgl->set_states(nullptr, UI_crt_img("console_button_pressed.png", 160, 0, 160, 45, UI_distance_units::pixels));
  auton_tgl->set_callback([=](){ 
      UI_execute_selector_toggles(auton_tgl, selector_panel_scr, true);
      UI_swap_screens({main_bg_scr, selector_panel_scr}); 
    } 
  );
  auto config_tgl = UI_crt_tgl(
    UI_crt_img("console_button.png", 320, 0, 160, 45, UI_distance_units::pixels),
    nullptr, 1
  );
  config_tgl->set_states(nullptr, UI_crt_img("console_button_pressed.png", 320, 0, 160, 45, UI_distance_units::pixels));
  config_tgl->set_callback([=](){ 
      UI_execute_selector_toggles(config_tgl, selector_panel_scr, true);
      UI_swap_screens({main_bg_scr, selector_panel_scr}); 
      UI_console_scr_add(chassis.turn_ki);
    } 
  );

  selector_panel_scr->add_UI_components({console_tgl, auton_tgl, config_tgl});
  UI_render_queue = {UI_console_scr, selector_panel_scr};
}

void UI_swap_screens(const std::vector<std::shared_ptr<screen>>& scr) {
  UI_render_buffer = scr;
  is_screen_swapping = true;
}

void UI_render() {
  bool local_needs_render_update = false;

  while (true) {
    if (is_screen_swapping) {
      local_needs_render_update = true;
      is_screen_swapping = false;

      UI_render_queue = UI_render_buffer;
      UI_render_buffer.clear();
    }

    for (const auto& screen : UI_render_queue) {
      if (screen->needs_update()) {
        local_needs_render_update = true;
        break;
      }
    }

    if (local_needs_render_update) {
      for (const auto& screen : UI_render_queue) {
        screen->render();
      }
      Brain.Screen.render(true, true);
      local_needs_render_update = false;
    }

    vex::this_thread::sleep_for(10);
  }
}

void UI_execute_selector_toggles(std::shared_ptr<UI_component> tgl, std::shared_ptr<screen> scr, bool lock_toggles) {
  int tgl_id = UI_decode_toggle_group(tgl->get_ID());

  if (tgl_id <= 0) { return; }

  auto* toggle_component = static_cast<toggle*>(tgl.get());

  if (toggle_component->get_toggle_state() == true && lock_toggles) {
    toggle_component->lock_toggle();
  }

  for (const auto& component : scr->get_UI_components()) {
    if (component == tgl) continue;
    if (tgl_id == UI_decode_toggle_group(component->get_ID())) {
      auto* toggle_component = static_cast<toggle*>(component.get());
      toggle_component->unpress();
    }
  }
}
