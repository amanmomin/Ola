#pragma once
#include "pebble.h"
#include "feature_menu_layer.h"
Window *get_language_window(); 
void language_init();
void language_deinit();
void languages_set_phrase(Phrase phrase);