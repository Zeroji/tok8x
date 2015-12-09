#ifndef TOK8X_HEADER_H
#define TOK8X_HEADER_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "buf.h"

buf_t* header_pack_buf(buf_t *bin, char *name, bool archived);

#endif
