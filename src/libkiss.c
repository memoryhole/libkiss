#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libkiss.h"

void kiss_init(struct kiss_frame* frame) {
    frame->_init = false;
    frame->data_size = 0;
    frame->_last_read_offset = 0;
    frame->_state = CONTINUE;
}

enum kiss_parse_state kiss_parse(struct kiss_frame* frame, uint8_t *data, uint16_t data_size) {
    int i = 0;

    if (frame->_state == PARTIAL_DONE) {
        i = frame->_last_read_offset + 1;
    } else if (frame->_state == DONE) {
        kiss_init(frame);
        frame->_state = CONTINUE;
    } else {
        frame->_state = CONTINUE;
    }

    while (i < data_size) {
        frame->_last_read_offset = i;

        if (!frame->_init) {
            if (data[i] == FEND) {
                if (!frame->_init) {
                    frame->_init = true;
                    frame->data_size = 0;
                }
            }
        } else if (data[i] == FEND) {
            if (frame->data_size) {
                if (i == data_size - 1) {
                    frame->_state = DONE;
                    break;
                } else {
                    frame->_init = false;
                    frame->_state = PARTIAL_DONE;
                    break;
                }
            }
        } else if (frame->_init) {
            frame->data[frame->data_size] = data[i];
            frame->data_size += 1;
        }

        i += 1;
    }

    return frame->_state;
}
