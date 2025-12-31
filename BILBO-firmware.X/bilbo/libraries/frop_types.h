#ifndef BILBO_FROP_TYPES_H
#define BILBO_FROP_TYPES_H

// response messages

struct short_error_message_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t error_code;
    uint8_t end_of_header;
};

// short error message (error code required)
typedef union {
    uint8_t data[5];
    uint8_t short_error_message_s;
} short_error_message;


struct ok_response_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t end_of_header;
};

// ok response (no intput required)
typedef union {
    uint8_t data[4];
    uint8_t ok_response_s;
} ok_response ;

// data messages


struct tuning_data_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t number_of_fields;
    uint8_t end_of_header;
    uint8_t block_length_setting;
    uint8_t block_data_setting;
    uint8_t block_length_note_oct;
    uint8_t block_data_note_oct;
    uint8_t block_length_note_pos;
    uint8_t block_data_note_pos;
    uint8_t block_length_freq;
    uint16_t block_data_freq;
    uint8_t block_length_diff;
    uint8_t block_data_diff;
};

// tuning data (required: NoF = 5, "T" [1], note octive [1], note position in octive [1], frequency [2], tune offset [1])
typedef union {
    uint8_t data[16];
    uint8_t tuning_data_s;
} tuning_data ;


struct range_change_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t number_of_fields;
    uint8_t end_of_header;
    uint8_t block_length_setting;
    uint8_t block_data_setting;
    uint8_t block_length_range;
    uint8_t block_data_range;
};

// range change (required: NoF = 2, "R" [1], range [1])
typedef union {
    uint8_t data[9];
    uint8_t tuning_data_s;
} range_change ;


struct change_profile_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t number_of_fields;
    uint16_t length_of_data;
    uint16_t header_checksum;
    uint8_t end_of_header;
    uint8_t block_length_setting;
    uint8_t block_data_setting;
    uint8_t block_length_ref;
    uint16_t block_data_ref;
    uint8_t block_length_ref_oct;
    uint8_t block_data_ref_oct;
};

// change profile (required: NoF = 2, "P" [1], reference note [2], reference note octive [1] )
typedef union {
    uint8_t data[16];
    uint8_t change_profile_s;
} change_profile ;

#define FROP_MSG_NULL 0
#define FROP_MSG_D_NULL 0x10
#define FROP_MSG_R_NULL 0x20
#define FROP_MSG_D_PROFILE_CHANGE 1
#define FROP_MSG_D_RANGE_CHANGE 2
#define FROP_MSG_D_TUNING_DATA 3
#define FROP_MSG_R_OK 4
#define FROP_MSG_R_SHORT_ERROR 5

#endif