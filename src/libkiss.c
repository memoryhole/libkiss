#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libkiss.h"

void kiss_init(struct kiss_frame* frame) {
    frame->data_size = 0;
    frame->command.byte = 0;

    frame->_init_offset = 0;
    frame->_init_found = 0;
    frame->_last_read_offset = 0;
    frame->_state = CONTINUE;
    frame->error = NONE;
}

void frame_set_data(struct kiss_frame* frame, uint8_t data) {
    if (frame->data_size+1 == MAX_DATA_SIZE) {
        frame->_state = ERROR;
        frame->error = DATA_TOO_LARGE;
        return;
    }
    frame->data[frame->data_size] = data;
    frame->data_size += 1;
}

enum kiss_parse_state kiss_parse(struct kiss_frame* frame, uint8_t* data, uint32_t data_size) {
    uint32_t i = 0;

    if (frame->_state == DONE_HAS_MORE) {
        i = frame->_last_read_offset + 1;
        frame->_last_read_offset = 0;
        frame->_init_found = false;
        frame->_init_offset = 0;
        frame->data_size = 0;
    } else if (frame->_state == DONE) {
        kiss_init(frame);
    }

    frame->_state = CONTINUE;

    while (i < data_size && frame->error == NONE) {
        frame->_last_read_offset = i;

        if (!frame->_init_found) {
            if (data[i] == FEND) {
                frame->_init_found = true;
                frame->_init_offset = i;
                frame->data_size = 0;
            }
        } else if (data[i] == FEND) {
            if (frame->data_size) {
                if (i == data_size - 1) {
                    frame->_state = DONE;
                    break;
                } else {
                    frame->_state = DONE_HAS_MORE;
                    break;
                }
            } else {
                frame->_init_found = true;
                frame->_init_offset = i;
            }
        } else {
            if (frame->_init_offset == i-1 && !frame->data_size) {
                frame->command.byte = data[i];
            } else {

                if (data[i] == TFESC) {
                    if (!frame->_escape) {
                        frame->_escape = true;
                    } else {
                        frame->_escape = false;
                        frame_set_data(frame, FESC);
                    }

                } else if(frame->_escape && data[i] == TFEND) {
                    frame->_escape = false;
                    frame_set_data(frame, FEND);

                } else {
                    frame->_escape = false;
                    frame_set_data(frame, data[i]);
                }

            }
        }

        i += 1;
    }

    return frame->_state;
}
