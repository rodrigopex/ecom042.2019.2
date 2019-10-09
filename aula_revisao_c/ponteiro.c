#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int a = 10;
    // {
    char b = 5;
    // }
    b = a;
    printf("Value a:0x%02X b:0x%02X\n", a, b);
    a = 0x101;  // a = 257;
    b = a;
    printf("Value a:0x%02X b:0x%02X\n", a, b);
    long int c      = 0;
    long long int d = 0;
    printf("Value sizeof c:%lu d:%lu\n", sizeof(c), sizeof(d));

    uint32_t e[6];
    char f = 100;
    // = {0};  // 0 '\0' NULL
    memset(e, 0, sizeof(e));
    for (int i = 0; i < sizeof(e); ++i) {
        printf("Value sizeof e[%d]:0x%02X\n", i,
               e[i]);  //-> *(e + i*sizeof(type e aponta))
    }
    e[6] = 5;
    printf("Value f:0x%02X\n", f);

    // for (int i = 0; i < sizeof(e); ++i, printf("Value sizeof e[%d]:0x%02X\n", i,
    // e[i]));
    char *g = NULL;
    g       = (char *) &a;
    *g      = 10;

    return 0;
}
