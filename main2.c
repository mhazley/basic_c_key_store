#include <stdio.h>
#include "key_store.h"

int main() {
    key_store_init();

    uint8_t test_guid_one[KEY_GUID_LENGTH] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };

    if( key_store_does_key_exist( test_guid_one ) ) {
        printf( "Key Exists in Store\n" );

        uint8_t test_ltk_two[KEY_LTK_LENGTH] = { 0 };
        if( key_store_get_key(test_guid_one, test_ltk_two) ) {
            printf( "Got Key\n" );
            for( int i=0; i<KEY_LTK_LENGTH; i++ ) {
                printf("%02hhX ", test_ltk_two[i]);
            }
        }
    } else {
        printf( "Key Not Found\n" );
    }

    return 0;
}