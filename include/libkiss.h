#ifndef LIBKISS_H
#define LIBKISS_H

#include <stdint.h>
#include <stdbool.h>

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
    CONTINUE,
    PARTIAL_DONE,
    DONE
};

struct command {
    unsigned char port : 4;
    unsigned char command : 4;
};

struct kiss_frame {
    uint8_t data[330];
    uint16_t data_size;
    union Command {
        struct command details;
        uint8_t byte;
    } command;

    uint16_t _last_read_offset;
    enum kiss_parse_state _state;
    int16_t _init_offset;
    bool _init_found;
};

void kiss_init(struct kiss_frame* frame);
enum kiss_parse_state kiss_parse(struct kiss_frame* frame, uint8_t *data, uint16_t data_size);

#endif
