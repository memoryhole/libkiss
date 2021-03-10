#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "libkiss.h"

static void test_parse_aligned(void** state) {
    struct kiss_frame frame;
    uint8_t data[] = {FEND, 0x01, 0x02, 0x03, FEND};

    kiss_init(&frame);
    assert_int_equal(kiss_parse(&frame, data, sizeof(data) / sizeof(uint8_t)), DONE);
    assert_int_equal(frame.data_size, 3);
    assert_memory_equal(frame.data, data+1, 3);
}

static void test_parse_multiple_passes(void** state) {
    struct kiss_frame frame;
    uint8_t data[2] = {FEND, 0x01};
    uint8_t data2[9] = {0x02, 0x03, FEND, 0x99, 0x98, FEND, 0x04, 0x05, FEND};
    uint8_t expected1[] = {0x01, 0x02, 0x03};
    uint8_t expected2[] = {0x04, 0x05};

    kiss_init(&frame);
    assert_int_equal(kiss_parse(&frame, data, 2), CONTINUE);
    assert_int_equal(kiss_parse(&frame, data2, 9), PARTIAL_DONE);
    assert_int_equal(frame.data_size, 3);
    assert_memory_equal(frame.data, expected1, 3);

    assert_int_equal(kiss_parse(&frame, data2, 9), DONE);
    assert_int_equal(frame.data_size, 2);
    assert_memory_equal(frame.data, expected2, 2);
}

static void test_parse_with_header(void** state) {
    struct kiss_frame frame;
    uint8_t data[] = {0x99, 0x98, 0x97, FEND, 0x01, 0x02, 0x03, FEND};

    kiss_init(&frame);
    assert_int_equal(kiss_parse(&frame, data, sizeof(data) / sizeof(uint8_t)), DONE);
    assert_int_equal(frame.data_size, 3);
    assert_memory_equal(frame.data, data+4, 3);
}

static void test_parse_with_multiple_fend(void** state) {
    struct kiss_frame frame;
    uint8_t data[] = {FEND, FEND, FEND, 0x01, 0x02, 0x03, FEND};

    kiss_init(&frame);
    assert_int_equal(kiss_parse(&frame, data, sizeof(data) / sizeof(uint8_t)), DONE);
    assert_int_equal(frame.data_size, 3);
    assert_memory_equal(frame.data, data+3, 3);
}

int main(int argc, char **argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_aligned),
        cmocka_unit_test(test_parse_with_header),
        cmocka_unit_test(test_parse_with_multiple_fend),
        cmocka_unit_test(test_parse_multiple_passes),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}

