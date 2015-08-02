#include <pebble.h>
#include "translation.h"
#include "data_framework.h"
#include "feature_menu_layer.h"

Phrase current_phrase;

void process_tuple(Tuple *t){
	int key = t->key;
	if(key != 100){
    	main_window_insert_phrase(t->value->cstring, key);
	}
	else{
		translation_set_new_phrase(t->value->cstring);
	}
}

void inbox(DictionaryIterator *iter, void *context){
	Tuple *t = dict_read_first(iter);
	if(t){
		process_tuple(t);
	}
	while(t != NULL){
		t = dict_read_next(iter);
		if(t){
			process_tuple(t);
		}
	}
}

void data_framework_init(){
    app_message_register_inbox_received(inbox);
    app_message_open(512, 512);
}
