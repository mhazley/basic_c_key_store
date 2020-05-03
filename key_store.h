
#ifndef KEY_STORE_KEY_STORE_H
#define KEY_STORE_KEY_STORE_H

#include <stdint.h>
#include <stdbool.h>

#define KEY_GUID_LENGTH         ( 16 )
#define KEY_LTK_LENGTH          ( 16 )

bool key_store_init( void );
bool key_store_add_key( uint8_t *guid, uint8_t *ltk );
bool key_store_get_key( uint8_t *guid, uint8_t *ltk );
void key_store_remove_key( uint8_t *guid );
bool key_store_does_key_exist( uint8_t *guid );

#endif //KEY_STORE_KEY_STORE_H
