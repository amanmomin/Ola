#include "pebble.h"
#include "langauges.h"
#include "feature_menu_layer.h"
#include "translation.h"
#include "data_framework.h"

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 8

Window *s_language_window;
static MenuLayer *s_language_layer;
const char *languages[8]={
  "Croatian",
  "French",
  "German",
  "Italian",
  "Polish",
  "Portuguese",
  "Romanian",
  "Spanish",
};

Phrase language_phrase;

void languages_set_phrase(Phrase phrase){
  language_phrase = phrase;
};


Window *get_language_window(){
  return s_language_window;
}

// static int s_current_icon = 0;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return NUM_FIRST_MENU_ITEMS;
}


static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
 menu_cell_basic_header_draw(ctx, cell_layer, "Languages");
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  menu_cell_basic_draw(ctx, cell_layer, languages[cell_index->row], NULL, NULL);
}

static void language_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
    translation_set_data(language_phrase, languages[cell_index->row]);
    window_stack_push(translation_window_get_window(), true);
}

static void language_window_load(Window *window) {

  // And also load the background

  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  s_language_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_language_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = language_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_language_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_language_layer));
}

static void language_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_language_layer);

}

void language_init() {
  s_language_window = window_create();
  window_set_window_handlers(s_language_window, (WindowHandlers) {
    .load = language_window_load,
    .unload = language_window_unload,
  });
}

void language_deinit() {
  window_destroy(s_language_window);
}
