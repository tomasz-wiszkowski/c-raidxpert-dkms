/****************************************************************************\
*
*   MODULE: rc_build_number.h
*
*		Header file to define the build number variables.
*
*   Copyright (c) 2000-2004, RAIDCore, Inc.
*   Copyright (c) 2005-2006, Broadcom Corporation.  All rights reserved.
*   Copyright (c) 2006-2008, Ciprico Inc.  All rights reserved.
*   Copyright (c) 2008-2013, Dot Hill Systems Corp.  All rights reserved.
*
\****************************************************************************/

#ifndef _RC_BUILD_NUMBER_H
#define _RC_BUILD_NUMBER_H

#include <linux/types.h>

// maximum build number string length
// - this is actually 20 due to the genflash utility
// - see fulcrum/utils/genflash/genflash.cpp
// - unfortunately, other areas could be affected by this size, and
//   reducing it may be bad (alignment, structure size, etc.)
#define RC_MAX_BUILD_NUMBER_SIZE 32

// maximum mon/day/year string length
#define RC_MAX_BUILD_MONDAYYEAR 32

extern char *RC_BUILD_NUMBER;
extern u16 RC_BUILD_COUNT;
extern char *RC_BUILD_MONDAYYEAR;

// be sure these this length matches that of the strings
// RC_BUILD_NUMBER_MARK_[BEGIN,END] in rc_build_number.cpp
// (length does not include null terminator)
#define RC_BUILD_NUMBER_MARK_LENGTH 7

#endif //_RC_BUILD_NUMBER_H
