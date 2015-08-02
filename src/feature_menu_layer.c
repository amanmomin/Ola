#define APP_LOG_LEVEL_HELLO APP_LOG_LEVEL_INFO
#include "pebble.h"
#include "feature_menu_layer.h"
#include "langauges.h"
#include "translation.h"
#include "data_framework.h"

static Window *s_main_window;
static MenuLayer *s_menu_layer;

Phrase phrases[15];
uint8_t phrase_stack_count = 0;

void main_window_insert_phrase(char *phrase, uint8_t stack_location){
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Got %s %d %d", phrase, stack_location, phrase_stack_count);
    strcpy(phrases[stack_location].phrase[0], phrase);
    phrase_stack_count++;
    menu_layer_reload_data(s_menu_layer);
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return 1;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return phrase_stack_count;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
    menu_cell_basic_header_draw(ctx, cell_layer, "Phrases");
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  menu_cell_basic_draw(ctx, cell_layer, phrases[cell_index->row].phrase[0], NULL, NULL);
}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  int row_called = cell_index->row;
  languages_set_phrase(phrases[row_called]);
  window_stack_push(get_language_window(), true);
}

static void main_window_load(Window *window) {

  //read persistent data

  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  for(int i = 0; i < 15; i++){
    int value = persist_read_data(100+i, &phrases[i], sizeof(Phrase));
    if(strlen(phrases[i].phrase[0]) > 1){

    //  APP_LOG(APP_LOG_LEVEL_INFO, "here ", NULL, NULL);
      phrase_stack_count++;
      menu_layer_reload_data(s_menu_layer);
    }
  }

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {

  // Persist data that was selected

  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
  for(int i = 0; i < 15; i++){
    int value = persist_write_data(100+i, &phrases[i], sizeof(Phrase));
  }
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });

  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  APP_LOG(APP_LOG_LEVEL_HELLO, "Hello Javascript");
  data_framework_init();
  language_init();
  translation_init();
  app_event_loop();
  deinit();
  language_deinit();
  translation_deinit();
}
