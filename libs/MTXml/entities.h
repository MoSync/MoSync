/** 
 * \file entities.h
 * \brief HTML entities look-up used by MTXml Parser
 * \author Fredrik Eldh
 */

#ifndef ENTITIES_H
#define ENTITIES_H

#include <mastring.h>

struct ENTITY { const char *name; int c; };

#ifdef __cplusplus
extern "C"
#endif
const struct ENTITY* entity_lookup(const char *str, unsigned int len);

#endif	//ENTITIES_H
