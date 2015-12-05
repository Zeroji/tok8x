#ifndef TOK8X_HEADER_H
#define TOK8X_HEADER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "buf.h"

#define HEADER_SIZE 0x4A

buf_t* header_pack_buf(buf_t *bin, uint8_t name[8], bool archived);

buf_t* header_unpack_buf(buf_t *bin);

#endif
