#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "key_store.h"

#define NUM_KEYS                ( 5 )
#define KEY_STORE_FILE_PATH     ( "./.key_store" )

/* Module Types */
typedef enum {
    KEY_ONE     = 0,
    KEY_TWO     = 1,
    KEY_THREE   = 2,
    KEY_FOUR    = 3,
    KEY_FIVE    = 4,

    KEY_MAX     = 5
} key_index_et;

typedef struct key_pair {
    uint8_t guid[KEY_GUID_LENGTH];
    uint8_t ltk[KEY_LTK_LENGTH];
    bool    valid;
} key_pair_t;

typedef struct key_store {
    key_pair_t key_pairs[NUM_KEYS];
    key_index_et next_valid_key;
} key_store_t;


/* Module Functions */
static bool check_for_key_store_and_create(void );
static bool init_empty_key_store( void );
static bool read_key_store( void );
static bool write_key_store( void );
static bool add_key( uint8_t *guid, uint8_t *ltk );


/* Module Data */
static key_store_t m_key_store;


bool key_store_init( void ) {
    return check_for_key_store_and_create();
}

bool key_store_add_key( uint8_t *guid, uint8_t *ltk ) {
    bool ret = true;

    if( !key_store_does_key_exist( guid ) ) {
        ret = add_key( guid, ltk );
    }

    return ret;
}

bool key_store_does_key_exist( uint8_t *guid ) {
    bool exists = false;

    for( int i=0; i<NUM_KEYS; i++ )
    {
        if( m_key_store.key_pairs[i].valid ) {
            if( memcmp( guid, m_key_store.key_pairs[i].guid, KEY_GUID_LENGTH ) == 0 ) {
                exists = true;
                break;
            }
        }
    }

    return exists;
}

extern bool key_store_get_key( uint8_t *guid, uint8_t *ltk ) {
    bool found = false;

    for( int i=0; i<NUM_KEYS; i++ )
    {
        if( m_key_store.key_pairs[i].valid ) {
            if( memcmp( guid, m_key_store.key_pairs[i].guid, KEY_GUID_LENGTH ) == 0 ) {
                // assumes you have memory for ltk as it doesnt want to expose internal pointer
                found = true;
                memcpy( ltk, m_key_store.key_pairs[i].ltk, KEY_LTK_LENGTH );
                break;
            }
        }
    }

    return found;
}

void key_store_remove_key( uint8_t *guid ) {
    for( int i=0; i<NUM_KEYS; i++ )
    {
        if( m_key_store.key_pairs[i].valid ) {
            if( memcmp( guid, m_key_store.key_pairs[i].guid, KEY_GUID_LENGTH ) == 0 ) {
                m_key_store.key_pairs[i].valid = false;
                break;
            }
        }
    }
}

static bool add_key( uint8_t *guid, uint8_t *ltk ) {
    memcpy( m_key_store.key_pairs[m_key_store.next_valid_key].guid, guid, KEY_GUID_LENGTH );
    memcpy( m_key_store.key_pairs[m_key_store.next_valid_key].ltk, ltk, KEY_LTK_LENGTH );
    m_key_store.key_pairs[m_key_store.next_valid_key].valid = true;

    if( ++m_key_store.next_valid_key == KEY_MAX ) {
        m_key_store.next_valid_key = KEY_ONE;
    }

    return write_key_store();
}

static bool check_for_key_store_and_create(void ) {
    bool ret = true;

    if( access( KEY_STORE_FILE_PATH, F_OK ) != -1 ) {
        // key file exists
        ret = read_key_store();
    } else {
        // key file does not exist
        ret = init_empty_key_store();
    }
}

static bool init_empty_key_store( void ) {
    for( int i=0; i<NUM_KEYS; i++ )
    {
        m_key_store.key_pairs[i].valid = false;
    }

    m_key_store.next_valid_key = KEY_ONE;

    return write_key_store();
}

static bool read_key_store( void ) {
    bool ret = true;
    FILE *key_file = fopen( KEY_STORE_FILE_PATH, "r" );

    if( key_file == NULL ) {
        printf("Error opening key store file for reading\n");
        ret = false;
    }

    if( fread( &m_key_store, sizeof( struct key_store ), 1, key_file ) <= 0 ) {
        printf("Error reading key store file\n");
        ret = false;
    }
    fclose (key_file);
    return ret;
}

static bool write_key_store( void ) {
    bool ret = true;
    FILE *key_file = fopen( KEY_STORE_FILE_PATH, "w" );

    if( key_file == NULL ) {
        printf("Error opening key store file for writing\n");
        ret = false;
    }

    if( fwrite( &m_key_store, sizeof( struct key_store ), 1, key_file ) <= 0 ) {
        printf("Error writing to key store file\n");
        ret = false;
    }
    fclose (key_file);
    return ret;
}