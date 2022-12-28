/****************************************************************************
 *
 * Copyright © 2006-2008 Ciprico Inc. All rights reserved.
 * Copyright © 2008-2013 Dot Hill Systems Corp. All rights reserved.
 *
 * Use of this software is subject to the terms and conditions of the written
 * software license agreement between you and DHS (the "License"),
 * including, without limitation, the following (as further elaborated in the
 * License):  (i) THIS SOFTWARE IS PROVIDED "AS IS", AND DHS DISCLAIMS
 * ANY AND ALL WARRANTIES OF ANY KIND, WHETHER EXPRESS, IMPLIED, STATUTORY,
 * BY CONDUCT, OR OTHERWISE; (ii) this software may be used only in connection
 * with the integrated circuit product and storage software with which it was
 * designed to be used; (iii) this source code is the confidential information
 * of DHS and may not be disclosed to any third party; and (iv) you may not
 * make any modification or take any action that would cause this software,
 * or any other Dot Hill software, to fall under any GPL license or any other
 * open source license.
 *
 ****************************************************************************/

/*
 * The maximum number of SCSI targets
 * 16 arrays (8 primary + 8 imported) + 8 pass-through devices + 1 config dev.
 */
#define RC_MAX_SCSI_TARGETS 25

/*
 * SCSI Commands (that we treat special and need a "generic" platform independent
 * define for)
 */
#define RC_INQUIRY 0x12
#define RC_READ_6 0x08
#define RC_READ_10 0x28
#define RC_READ_16 0x88
#define RC_WRITE_6 0x0A
#define RC_WRITE_10 0x2A
#define RC_WRITE_16 0x8a

#define BYTE0(x) (unsigned char)(x)
#define BYTE1(x) (unsigned char)(x >> 8)
#define BYTE2(x) (unsigned char)(x >> 16)
#define BYTE3(x) (unsigned char)(x >> 24)

/*
 * group 6 commands
 *
 * the 3 most signficant bits of addr[0] define the LUN
 *  Mask the lun by using "addr[0] & 1f"
 */
typedef struct rc_cmd6 {
	unsigned char opcode; /* opcode             */
	unsigned char addr[3]; /* block address      */
	unsigned char len; /* transfer length    */
	unsigned char control; /* control field      */
} rc_cmd6_t;

/*
 * group 10 commands.
 */

typedef struct rc_cmd10 {
	unsigned char opcode; /* opcode                  */
	unsigned char lun; /* logical unit number:    */
	unsigned char addr[4]; /* block address           */
	unsigned char reserved; /* reserved field          */
	unsigned char len[2]; /* transfer length         */
	unsigned char control; /* control byte            */
} rc_cmd10_t;

/*
 * group 16 commands.
 */

typedef struct rc_cmd16 {
	unsigned char opcode; /* opcode                  */
	unsigned char lun; /* logical unit number:    */
	unsigned char addr[8]; /* block address           */
	unsigned char len[4]; /* transfer length         */
	unsigned char group; /* reserved field          */
	unsigned char control; /* control byte            */
} rc_cmd16_t;

typedef union rc_scb {
	struct rc_cmd6 scsi6;
	struct rc_cmd10 scsi10;
	struct rc_cmd16 scsi16;
} rc_scb_t;

typedef struct inq_cmd {
	u8 opcode;
	u8 lun;
	u8 page;
	u8 resv;
	u8 len;
	u8 ctrl;
} inq_cmd_t;

/*
 * private data.
 */
struct rc_scsi_cmd {
	rc_srb_t *srb;
};

/*
 * SCSI inquiry data
 */
typedef struct rc_inq_data {
	u8 dev_type; // Peripheral qualifier | Peripheral Device type
	u8 dev_qual; // RMB | Device type Qualifier
	u8 version; // ISO version | ECMA version | ANSI-approved version
	u8 data_fmt; // AENC | TrmIOP | Response data format
	u8 add_len; // Additional length (n-4)
	u8 pad1[2]; // reserved - must be zero
	u8 flags; // RelAdr | WBus32 | WBus16 | Sync | Linked | reserved |
	// CmdQue | SftRe
	u8 vendor_id[8]; // vendor ID
	u8 product_id[16]; // Product ID
	u8 rev_level[4]; // Product Revision Level
	u8 vendor_specific[20];
} rc_inq_data_t;

/*
 * values for dev_type: Peripheral device type in plain English
 */
#define INQD_PDT_DA 0x00 /* Direct-access (DISK) device */
#define INQD_PDT_PROC 0x03 /* Processor device */
#define INQD_PDT_CHNGR 0x08 /* Changer (jukebox, scsi2) */
#define INQD_PDT_COMM 0x09 /* Communication device (scsi2) */
#define INQD_PDT_NOLUN2 0x1f /* Unknown Device (scsi2) */
#define INQD_PDT_NOLUN 0x7f /* Logical Unit Not Present */

#define INQD_PDT_DMASK 0x1F /* Peripheral Device type Mask */
#define INQD_PDT_QMASK 0xE0 /* Peripheral Device Qualifer Mask */

typedef struct sense_data_s {
	u8 error_code; // 70h (current errors), 71h(deferred errors)
	u8 valid : 1; // A valid bit of one indicates that the information
	// field contains valid information as defined in
	// the SCSI-2 Standard.

	u8 segment_number; // Only used for COPY, COMPARE, or COPY AND VERIFY

	u8 sense_key : 4; // Sense Key
	u8 reserved : 1;
	u8 ili : 1; // Incorrect length Indicator
	u8 eom : 1; // End Of Medium - reserved for random access
	// devices
	u8 file_mark : 1; // file_mark - reserved for random access devices

	u8 information[4]; // for direct-access devices, contains the unsigned
	// logical block address or Residue associated with
	// the sense key
	u8 add_sense_len; // number of additional sense bytes to follow
	u8 cmnd_info[4]; // not used
	u8 asc; // Additional Sense Code
	u8 ascq; // Additional Sense Code Qualifier
	u8 fruc; // Field Replaceable Unit Code - not used

	u8 bit_ptr : 3; // indicates which byte of the CDB or parameter data
	// was in error
	u8 bpv : 1; // bit pointer valid (BPV): 1- indicates that
	// the bit_ptr field has valid value
	u8 reserved2 : 2;
	u8 cd : 1; // command data bit: 1- illegal parameter in CDB.
	//                   0- illegal parameter in data.
	u8 sksv : 1;

	u8 field_ptr[2]; // byte of the CDB or parameter data in error
} sense_data_t;

/*
 * Sense keys
 */
#define SENKEY_NO_SENSE 0x00
#define SENKEY_UNDEFINED 0x01
#define SENKEY_NOT_READY 0x02
#define SENKEY_MEDIUM_ERR 0x03
#define SENKEY_HW_ERR 0x04
#define SENKEY_ILLEGAL 0x05
#define SENKEY_ATTENTION 0x06
#define SENKEY_PROTECTED 0x07
#define SENKEY_BLANK 0x08
#define SENKEY_V_UNIQUE 0x09
#define SENKEY_CPY_ABORT 0x0A
#define SENKEY_ABORT 0x0B
#define SENKEY_EQUAL 0x0C
#define SENKEY_VOL_OVERFLOW 0x0D
#define SENKEY_MISCOMP 0x0E
#define SENKEY_RESERVED 0x0F

/*
 * Sense codes
 */
#define SENCODE_NO_SENSE 0x00
#define SENCODE_END_OF_DATA 0x00
#define SENCODE_BECOMING_READY 0x04
#define SENCODE_INIT_CMD_REQUIRED 0x04
#define SENCODE_PARAM_LIST_LENGTH_ERROR 0x1A
#define SENCODE_INVALID_COMMAND 0x20
#define SENCODE_LBA_OUT_OF_RANGE 0x21
#define SENCODE_INVALID_CDB_FIELD 0x24
#define SENCODE_LUN_NOT_SUPPORTED 0x25
#define SENCODE_INVALID_PARAM_FIELD 0x26
#define SENCODE_PARAM_NOT_SUPPORTED 0x26
#define SENCODE_PARAM_VALUE_INVALID 0x26
#define SENCODE_RESET_OCCURRED 0x29
#define SENCODE_LUN_NOT_SELF_CONFIGURED_YET 0x3E
#define SENCODE_INQUIRY_DATA_CHANGED 0x3F
#define SENCODE_SAVING_PARAMS_NOT_SUPPORTED 0x39
#define SENCODE_DIAGNOSTIC_FAILURE 0x40
#define SENCODE_INTERNAL_TARGET_FAILURE 0x44
#define SENCODE_INVALID_MESSAGE_ERROR 0x49
#define SENCODE_LUN_FAILED_SELF_CONFIG 0x4c
#define SENCODE_OVERLAPPED_COMMAND 0x4E

/*
 * Additional sense codes
 */
#define ASENCODE_NO_SENSE 0x00
#define ASENCODE_END_OF_DATA 0x05
#define ASENCODE_BECOMING_READY 0x01
#define ASENCODE_INIT_CMD_REQUIRED 0x02
#define ASENCODE_PARAM_LIST_LENGTH_ERROR 0x00
#define ASENCODE_INVALID_COMMAND 0x00
#define ASENCODE_LBA_OUT_OF_RANGE 0x00
#define ASENCODE_INVALID_CDB_FIELD 0x00
#define ASENCODE_LUN_NOT_SUPPORTED 0x00
#define ASENCODE_INVALID_PARAM_FIELD 0x00
#define ASENCODE_PARAM_NOT_SUPPORTED 0x01
#define ASENCODE_PARAM_VALUE_INVALID 0x02
#define ASENCODE_RESET_OCCURRED 0x00
#define ASENCODE_LUN_NOT_SELF_CONFIGURED_YET 0x00
#define ASENCODE_INQUIRY_DATA_CHANGED 0x03
#define ASENCODE_SAVING_PARAMS_NOT_SUPPORTED 0x00
#define ASENCODE_DIAGNOSTIC_FAILURE 0x80
#define ASENCODE_INTERNAL_TARGET_FAILURE 0x00
#define ASENCODE_INVALID_MESSAGE_ERROR 0x00
#define ASENCODE_LUN_FAILED_SELF_CONFIG 0x00
#define ASENCODE_OVERLAPPED_COMMAND 0x00
