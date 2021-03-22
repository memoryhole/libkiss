# libkiss
[KISS Protocol](https://en.wikipedia.org/wiki/KISS_(TNC)) (Keep It Simple Stupid) parsing library for Amateur Radio

 * no dynamic memory allocations
 * supports parsing with partial data

Getting Started
===============

### Example API Usage

```c
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "libkiss.h"

void print_frame(struct kiss_frame* frame) {
    printf("Frame command=%d port=%d data=", frame->command.details.command, frame->command.details.port);
    for (uint16_t i = 0; i < frame->data_size; i++) {
        printf("%02x ", frame->data[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    struct kiss_frame frame;

    // Parse in a single pass
    printf("SINGLE PASS\n");
    uint8_t data1[] = {FEND, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, FEND};
    uint16_t data1_size = sizeof(data1) / sizeof(uint8_t);

    kiss_init(&frame);
    assert(DONE == kiss_parse(&frame, data1, data1_size));
    print_frame(&frame);

    // Parse in multiple passes (useful if reading partial data from a socket or file)
    // CONTINUE means that the buffer was consumed and no frame was parsed yet
    // DONE means a frame is parsed and the buffer can be re-used
    // DONE_HAS_MORE means a frame has been parsed, but there is remaining data in the buffer that needs to be fed. Keep calling until the state is DONE or CONTINUE.
    printf("\nMULTIPLE PASSES\n");
    uint8_t data2[] = {FEND, 0x00, 0x01, FEND, FEND, 0x00, 0x02, 0x03, 0x04, 0x05, 0x06, FEND};
    uint16_t data_size = sizeof(data2) / sizeof(uint8_t);

    kiss_init(&frame);

    enum kiss_parse_state state = CONTINUE;

    for (int i = i; i < data_size; i += 3) {
        state = kiss_parse(&frame, data2 + i, 3);

        if (DONE == state) {
            print_frame(&frame);
        } else if (DONE_HAS_MORE == state) {
            print_frame(&frame);

            while(DONE_HAS_MORE == (state = kiss_parse(&frame, data2 + i, 3))) {
                print_frame(&frame);
            }

            if (DONE == state) {
                print_frame(&frame);
            }
        }
    }

    return 0;
}
```

### Install Build Dependencies

```bash
sudo apt install build-essential meson ninja-build libcmocka-dev
```

### Build

```bash
make
```

### Test

```bash
make test
```

### Install

```bash
make install
```

### Uninstall

```bash
make uninstall
```

