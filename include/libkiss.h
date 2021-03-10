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
    data_frame = 0x00,
    tx_delay = 0x01,
    p = 0x02,
    slot_time = 0x03,
    tx_tail = 0x04,
    full_duplex = 0x05,
    set_hardware = 0x06,
    ret = 0xff,
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

    uint16_t _last_read_offset;
    enum kiss_parse_state _state;
    bool _init;
};

void kiss_init(struct kiss_frame* frame);
enum kiss_parse_state kiss_parse(struct kiss_frame* frame, uint8_t *data, uint16_t data_size);

#endif
