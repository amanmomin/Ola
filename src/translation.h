#pragma once
#include "data_framework.h"
void translation_init();
void translation_deinit();
void translation_set_data(Phrase phrase, const char *language);
Window *translation_window_get_window();
void translation_set_new_phrase(char *new_phrase);
