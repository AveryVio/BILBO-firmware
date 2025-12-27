// response messages

struct short_error_message_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t error_code;
    uint8_t end_of_header;
};

// short error message (error code required)
typedef union short_error_message_u {
    uint8_t data[6];
    uint8_t short_error_message_s;
} short_error_message;


struct ok_response_s {
    uint8_t start_of_message;
    uint8_t frop_message_type;
    uint8_t frop_message_format;
    uint8_t end_of_header;
};

// ok response (no intput required)
typedef union ok_response_u {
    uint8_t data[5];
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
    uint8_t block_length_note;
    uint16_t block_data_note;
    uint8_t block_length_freq;
    uint16_t block_data_freq;
    uint8_t block_length_diff;
    uint8_t block_data_diff;
};

// tuning data (required: NoF = 4, "T" [1], note [2], frequency [2], tune offset [1])
typedef union tuning_data_u {
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
typedef union range_change_u {
    uint8_t data[10];
    uint8_t tuning_data_s;
} range_change ;


/*
struct change_profile_s {
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

// range profile (required: NoF = , )
typedef union change_profile_u {
    uint8_t data[10];
    uint8_t change_profile_s;
} change_profile ;
*/