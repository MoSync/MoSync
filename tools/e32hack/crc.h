#include "e32base.h"
#include "helpers/types.h"

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void calcUidCrc(void* buffer);
uint32_t crc_itu32_calc(const char *cbuf, int len, uint32_t crc);
