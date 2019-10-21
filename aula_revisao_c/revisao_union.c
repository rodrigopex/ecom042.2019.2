#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct packet_s {
    uint8_t ab;
    uint8_t c;
    uint16_t d;
};
struct packet_simple_s {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint16_t d;
};

union int_array4 {
    int a;
    uint8_t data[4];
};


union int_array4 x = {0};

struct packet_ninja_s {
    unsigned int a : 3;
    unsigned int b : 5;
    union {
        struct {
            unsigned int reserved : 8;
            unsigned int c : 8;
            unsigned int d : 16;
        };
        uint8_t data[4];
    };
};

typedef struct packet_s packet_t;
typedef struct packet_ninja_s packet_ninja_t;


packet_t packet_create(uint8_t a, uint8_t b, uint8_t c, uint16_t d)
{
    packet_t pkt = {.ab = a | b << 3, .c = c, .d = d};
    return pkt;
}

int packet_ninja_init(packet_ninja_t *pkt, uint8_t a, uint8_t b, uint8_t c, uint16_t d)
{
    pkt->a = a;
    pkt->b = b;
    pkt->c = c;
    pkt->d = d;
    return 0;
}

int packet_ninja_set(packet_ninja_t *pkt, uint8_t *data, size_t size)
{
    if (size > 3) {
        return -1;
    }
    memcpy(pkt->data + 1, data, size);
    // pkt->c = data[0];
    // pkt->d = data[1] | data[2] << 8;
    return 0;
}
int packet_ninja_set_bytes(packet_ninja_t *pkt, uint8_t *data, size_t size)
{
    if (size > 3) {
        return -1;
    }

    pkt->c = data[0];
    pkt->d = data[1] | data[2] << 8;
    return 0;
}

void packet_ninja_print(packet_ninja_t *pkt)
{
    if (!pkt) {
        return;
    }
    printf("Packet ninja: a %d/%02Xh, b %d/0x%02X, c %d/0x%02X, d %d/0x%02X\n", pkt->a,
           pkt->a, pkt->b, pkt->b, pkt->c, pkt->c, pkt->d, pkt->d);
}

int main(int argc, char const *argv[])
{
    packet_ninja_t my_pkt = {0};
    packet_ninja_init(&my_pkt, 6, 2, 0x0f, 0x10e4);
    packet_ninja_print(&my_pkt);
    uint8_t data[] = {0x01, 0x02, 0x03};
    packet_ninja_set(&my_pkt, data, 3);
    packet_ninja_print(&my_pkt);
    uint8_t data2[] = {0x04, 0x05, 0x06};
    packet_ninja_set_bytes(&my_pkt, data2, 3);
    packet_ninja_print(&my_pkt);
    printf("Size of normal %ld, simple %ld ,ninja %ld\n", sizeof(struct packet_s),
           sizeof(struct packet_simple_s), sizeof(struct packet_ninja_s));
    return 0;
}
