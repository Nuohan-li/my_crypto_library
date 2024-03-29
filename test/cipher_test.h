#ifndef CIPHER_TEST_H
#define CIPHER_TEST_H

typedef struct {
    char *name;
    int (*unit_test_fn)();
    int enabled;
} cipher_tests;

int chacha20_test();
int poly1305_test();

#endif /* CIPHER_TEST_H */