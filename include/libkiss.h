#ifndef LIBKISS_H
#define LIBKISS_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_DATA_SIZE (1024 * 1024)

enum special_chars {
    FEND = 0xC0,
    FESC = 0xDB,
    TFEND = 0xDC,
    TFESC = 0xDD
};

enum command_code {
    DATA_FRAME = 0x00,
    TX_DELAY = 0x01,
    P = 0x02,
    SLOT_TIME = 0x03,
    TX_TAIL = 0x04,
    FULL_DUPLEX = 0x05,
    SET_HARDWARE = 0x06,
    RET = 0xff,
};

enum kiss_parse_state {
    DONE,
    DONE_HAS_MORE,
    CONTINUE,
    ERROR,
};

enum error_code {
    NONE,
    DATA_TOO_LARGE
};

struct command {
    unsigned char port : 4;
    unsigned char command : 4;
};

struct kiss_frame {
    uint8_t data[MAX_DATA_SIZE];
    uint32_t data_size;
    union Command {
        struct command details;
        uint8_t byte;
    } command;

    uint32_t _last_read_offset;
    enum kiss_parse_state _state;
    enum error_code error;
    uint32_t _init_offset;
    bool _init_found;
    bool _escape;
};

void kiss_init(struct kiss_frame* frame);
enum kiss_parse_state kiss_parse(struct kiss_frame* frame, uint8_t *data, uint32_t data_size);

#endif
