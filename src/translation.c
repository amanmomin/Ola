#include <pebble.h>
#include "effectlayer/effect_layer.h"
#include "translation.h"
#include "data_framework.h"
#include "other.h"

Window *s_translation_window;
EffectLayer *effect_layer;
TextLayer *original_phrase_layer, *to_layer, *new_phrase_layer;
Phrase translation_phrase;
char translation_language[1][20], translation_phrase_text[1][50];

Window *translation_window_get_window(){
    return s_translation_window;
}

void send_translation_request(){
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);

	if (iter == NULL) {
	  return;
	}

	dict_write_uint16(iter, 200, 0);
    dict_write_cstring(iter, 201, translation_language[0]);
    dict_write_cstring(iter, 202, translation_phrase.phrase[0]);
	dict_write_end(iter);

	app_message_outbox_send();
}

void translation_set_data(Phrase phrase, const char *language){
    translation_phrase = phrase;
    strcpy(translation_language[0], language);
    send_translation_request();
}

void translation_set_new_phrase(char *new_phrase){
    strcpy(translation_phrase_text[0], new_phrase);
    text_layer_set_text(new_phrase_layer, translation_phrase_text[0]);
}

void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);

  original_phrase_layer = text_layer_create(GRect(0, 190, 144, 168));
  text_layer_set_font(original_phrase_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text(original_phrase_layer, translation_phrase.phrase[0]);
  text_layer_set_text_alignment(original_phrase_layer, GTextAlignmentCenter);
  text_layer_set_text_color(original_phrase_layer, GColorWhite);
  text_layer_set_background_color(original_phrase_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(original_phrase_layer));

  to_layer = text_layer_create(GRect(0, 190, 144, 100));
  text_layer_set_font(to_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  static char to_buffer[] = "the the then then then";
  snprintf(to_buffer, sizeof(to_buffer), "in %s is", translation_language[0]);
  text_layer_set_text(to_layer, to_buffer);
  text_layer_set_text_alignment(to_layer, GTextAlignmentCenter);
  text_layer_set_text_color(to_layer, GColorWhite);
  text_layer_set_background_color(to_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(to_layer));

  new_phrase_layer = text_layer_create(GRect(0, 190, 144, 100));
  text_layer_set_font(new_phrase_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text(new_phrase_layer, "loading...");
  text_layer_set_text_alignment(new_phrase_layer, GTextAlignmentCenter);
  text_layer_set_text_color(new_phrase_layer, GColorWhite);
  text_layer_set_background_color(new_phrase_layer, GColorBlack);
  layer_add_child(window_layer, text_layer_get_layer(new_phrase_layer));

  animate_layer(text_layer_get_layer(original_phrase_layer), &GRect(0, 190, 144, 168), &GRect(0, 0, 144, 168), 400, 0);
  animate_layer(text_layer_get_layer(to_layer), &GRect(0, 190, 144, 168), &GRect(0, 34, 144, 168), 400, 120);
  animate_layer(text_layer_get_layer(new_phrase_layer), &GRect(0, 190, 144, 168), &GRect(0, 58, 144, 168), 400, 300);
}

void main_window_unload(Window *window) {
  text_layer_destroy(original_phrase_layer);
  text_layer_destroy(to_layer);
  text_layer_destroy(new_phrase_layer);
}

void translation_init() {
  s_translation_window = window_create();
  window_set_background_color(s_translation_window, GColorBlack);
  window_set_window_handlers(s_translation_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
}

void translation_deinit() {
  window_destroy(s_translation_window);
}
