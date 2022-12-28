/****************************************************************************
 *
 * Copyright © 2006-2008 Ciprico Inc. All rights reserved.
 * Copyright © 2008-2015 Dot Hill Systems Corp. All rights reserved.
 * Copyright © 2019-2020, Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef RC_MSG_PLATFORM_H
#define RC_MSG_PLATFORM_H

#include <linux/stdarg.h>
#include <linux/types.h>

#include "rc_srb.h"

#define RC_COOKIE_VALUE_LO 0x2A1DC02E
#define RC_COOKIE_VALUE_HI 0x38887900

#define RC_INTERFACE_VERSION 0x100

#define RC_INTERFACE_CHECKSUM \
	(RC_COOKIE_VALUE_LO + RC_COOKIE_VALUE_HI + RC_INTERFACE_VERSION)

typedef void(rc_function_t)(void);
typedef void(rc_function_cpuid_t)(u32, u32 *, u32 *, u32 *, u32 *);

#define RC_NUM_TUNABLE_PARAMS 32
typedef enum {
	rc_DoSorted = 0,
	rc_R1SeekMethod,
	rc_AffinityReadRateLimit,
	rc_HonestQueueLimit,
	rc_DoHonestQueue,
	rc_AccountedSizeOutstandingLimit,
	rc_R1_StreamAccessThreshold,
	rc_FlushDirtyDataDelay,
	rc_StreamActivateThreshold,
	rc_FlushBurstLimit,
	rc_FlushResourceQueueEntriesLimit,
	rc_ActiveRaid5FlushesLimit,
	rc_ActiveFlushesLimit,
	rc_DoStream,
	rc_TwoTripWriteHitRate,
	rc_TwoTripReadHitRate,
	rc_TwoTripReadRate,
	rc_TwoTripWriteRate,
	rc_OneTripWriteHitRate,
	rc_OneTripReadHitRate,
	rc_RandomWriteHitRate,
	rc_RandomReadHitRate,
	rc_OneTripReadRate,
	rc_OneTripWriteRate,
	rc_RAID1_AffinityLockSize,
	rc_WriteBypassThreshold,
	rc_DoSmart,
	rc_SmartPollInterval,
} rc_tunable_param_t;

#define RC_SINGLE_IO_ADDRESS_COUNT 17 /* must match what the OSIC sends. */
#define RC_MAX_MEM_OPERANDS 16

/*
 * memory IDs
 */
#define RC_MEM_UNSET 0
#define RC_MEM_VADDR 0x1000
#define RC_MEM_PADDR 0x2000
#define RC_MEM_MAPPED 0x4000
#define RC_MEM_DMA 0x8000

#define MEM_TYPE 0xf000
#define MEM_OP_MASK 0x0f00
#define MEM_SUB_TYPE 0x00ff

/*
 * force the OSIC to do operations externally
 */
#define RC_MEM_USE_EXTERNAL_LIST_XOR 0x0100
#define RC_MEM_USE_EXTERNAL_LIST_COPY 0x0200
#define RC_MEM_USE_EXTERNAL_LIST_CMP 0x0400
#define RC_MEM_USE_EXTERNAL_CP_CLR 0x0800

typedef enum {
	RC_OP_MEM_LIST_XOR = 1,
	RC_OP_MEM_LIST_COPY,
	RC_OP_MEM_LIST_CMP,
	RC_OP_MEM_CLEAR,
	RC_OP_MEM_COPY,
	RC_OP_MEM_USER_COPY
} rc_mem_opcode_t;

/*
 * this memory operation id is opaque to the driver
 */
typedef union rc_mem_op_id_s {
	void *addr;
	u64 u64;
} rc_mem_op_id_t;

/*
 * a chained list of memory address lists
 * each list can not exceed RC_SINGLE_IO_ADDRESS_COUNT number of elements
 * addresses can be virtual or physical
 * all addresses in chain must be the same type
 *
 * this is a very messy way to pass the parameters, but we're stuck with it.
 */

typedef struct rc_addr_list_s {
	void *reserved_priv1; // Reserved for external OSIC use only
	void *reserved_priv2; // Reserved for external OSIC use only
	u8 mem_elem_count; // elemnt count for this list
	u8 reserved_priv3; // Reserved for external OSIC use only
	u16 mem_id; // Memory space indentifier
	struct rc_addr_list_s *next; // If more than SINGLE_IO_ADDRESS_COUNT
	// elements, more lists.
	rc_sg_elem_t sg_list[RC_SINGLE_IO_ADDRESS_COUNT];
	u32 RequirePhysical; // Notification for Hibernation /
	// Crashdump and for future
	// ... reference to what actually
	// needs physical memory
} rc_addr_list_t;

typedef struct rc_mem_list_s {
	rc_addr_list_t *addr;
	u32 start_element;
	u32 start_offset;
} rc_mem_list_t;

/*
 * this is a very messy way to pass the parameters, but we're stuck with it
 */
typedef struct rc_mem_list_op_s {
	struct rc_addr_list_s *dst_addr_list; // addr list for destination XOR DMA
	u32 dst_element; // starting dest element DMA
	u32 dst_offset; // starting dest offset XOR DMA CMP
	union {
		struct rc_addr_list_s *
			*addr_list_array; // Array of address lists for
		// source(s) XOR CMP
		struct rc_addr_list_s *addr_list; // source address list DMA
	} src;
	u32 src_element; // starting src element DMA
	u32 src_offset; // startint src offset DMA
	u32 array_count; // number of sources in array XOR CMP
	u32 sector_count; // Number of sectors XOR DMA CMP
	u32 flags; // engine flags XOR
} rc_mem_list_op_t;

// Ignore destination as part of XOR
// operation
#define RC_MEMOP_FLAG_XOR_MOVE_FIRST 0x01
// Return success only if final
// result is zero
#define RC_MEMOP_FLAG_XOR_CHECK_ZERO 0x02

typedef struct rc_access_ok_s {
	u64 write;
	void *access_location;
	u64 access_size;
	u64 returnStatus;

} rc_access_ok_t;

typedef struct rc_usr_mem_s {
	u64 dstLocation;
	u64 srcLocation;
	u64 size;
} rc_usr_mem_t;

typedef struct rc_mem_op_s {
	struct rc_mem_op_s *next;
	rc_mem_op_id_t id; // opaque
	rc_mem_opcode_t opcode;
	u32 status; // immediate status returned
	union {
		rc_mem_list_op_t *list;
		struct {
			u64 dst;
			u64 src;
			u32 dst_id;
			u32 src_id;
			u32 byte_count;
		} cp;
		struct {
			u64 dst;
			u32 dst_id;
			u32 byte_count;
		} clr;
	} mem;
} rc_mem_op_t;

typedef struct rc_mem_op_resp_s {
	rc_mem_op_id_t id; // opaque
	u32 status;
} rc_mem_op_resp_t;

typedef struct rc_pci_op_s {
	u32 adapter;
	u32 offset;
	u32 val;
	u32 status;
} rc_pci_op_t;

typedef struct rc_pci_io_s {
	u32 adapter;
	u32 offset;
	u32 width;
	u32 val;
	u32 status;
} rc_pci_io_t;

/*
 * Memory Operation Request Block
 */
typedef struct rc_morb_s {
	rc_mem_op_id_t id; // opaque
	rc_mem_opcode_t opcode;
	u32 status; // immediate status returned
	u32 num_arrays;
	u32 byte_count;
	rc_sg_list_t *sg_list[16]; // start of lists, variable size
} rc_morb_t;

//
//  Enumerate <C>all <T>ype for <S>end
//
#define RC_CTS_TEST 1
#define RC_CTS_GET_INFO 2
#define RC_CTS_INIT_CONTROLLER 3
#define RC_CTS_FINAL_INIT 4
#define RC_CTS_TIMER_CALL 5
#define RC_CTS_INTERRUPT_CALL 6
#define RC_CTS_SEND_SRB 7
#define RC_CTS_PAUSE 8
#define RC_CTS_RESUME 9
#define RC_CTS_MEMORY_OP_RESP 11
#define RC_CTS_SET_MSG_LEVEL 12
#define RC_CTS_PERFORM_DPC 13
#define RC_CTS_RESART_ADAPTER 14
#define RC_CTS_STOP_ADAPTER 15
#define RC_CTS_CHANGE_PARAM 16
#define RC_CTS_PARAM_AN 1
#define RC_CTS_PARAM_ZPODD 2
#define RC_CTS_SCSI_INFO 17

typedef struct rc_init_controller_s {
	//
	//    Sent to STH from driver
	//
	void *controller_memory; // cpu memory per controller
	void *controller_handle;
	void *pci_config_space;
	u32 pci_config_space_length;
	void *bar_memory[6]; // Virtually mapped memory space
	u32 bar_length[6]; // Length of each space
	u16 bar_port[6]; // Port bars
	u16 controller_memory_id; // memory type
	u16 reserved;
	u16 orig_vendor_id; // Original VendorID before trapping
	u16 orig_device_id; // Original DeviceID before trapping
	u32 pci_location; // PCI bus Bus/Slot/Function information

	//
	//    Input from STH
	//
	u32 max_split_virtual_memory_size_needed;
	u32 max_split_cache_memory_size_needed;
#define virtual_memory_size_needed min_virtual_memory_size_needed
#define cache_memory_size_needed min_cache_memory_size_needed
	u32 min_virtual_memory_size_needed;
	u32 min_cache_memory_size_needed;

	// The following two fields are said to be required by the software
	// which is bizarre to say the least.
	// The software should not have the ability to manipulate hardware
	// registers, but just in case this is actually really used, keeping
	// this here.
	// These two calls will log an unsupported function call and won't
	// perform any actuall register reads/writes.
	u32 (*regread_unsupported)(void *, u32);
	void (*regwrite_unsupported)(void *, u32, u32);

	void *context;
} rc_init_controller_t;

typedef struct rc_final_init_s {
	//
	//    Sent to STH from driver
	//
	void *virtual_memory;
	void *cache_memory;
	u32 size_of_virtual_memory_allocated;
	u32 size_of_cache_memory_allocated;
	u16 virtual_memory_id; // memory type
	u16 cache_memory_id; // memory type
	u32 timer_interval;
} rc_final_init_t;

typedef struct rc_scsi_info_s {
	u32 bus;
	u32 target;
	u32 lun;
	u32 value;
} rc_scsi_info_t;

//
//    Structure which contains the send argument information
//
typedef struct rc_send_arg_s {
	u32 call_type;
	union {
		struct {
			//
			//    Sent to STH from driver
			//
			u32 max_total_memory;
			u32 max_cache_memory;
			u32 max_virtual_memory;
			u32 controller_count;
			u32 max_sg_map_elements; // maximun number of sg elements
			// allocated on each srb by the sender
			u32 max_print_severity; // Sets the max severity for getting
			// back print messages

			//
			//    Input from the STH
			//
			u32 memory_size_per_controller;
			u32 memory_size_per_srb;
			u32 timer_interval;
			u32 random_seed;
			u64 max_lba; // in blocks, we use LBA
			u32 parameter_mask;
			u32 tunable_parameters[RC_NUM_TUNABLE_PARAMS];
			u32 support4kNativeDisks;
		} get_info __attribute__((aligned(8)));
		struct rc_init_controller_s init_controller;
		struct rc_final_init_s final_init __attribute__((aligned(8)));
		struct {
			//
			//    Sent to STH from driver
			//
			void *controller_handle;
		} interrupt_call __attribute__((aligned(8)));
		struct {
			struct rc_srb_s *srb;
			u32 queued; // 1 if queued
		} send_srb __attribute__((aligned(8)));
		struct {
			u32 param;
			u32 value;
		} change_param __attribute__((aligned(8)));
		rc_mem_op_resp_t mem_op_resp __attribute__((aligned(8)));
		u32 max_print_severity __attribute__((aligned(8)));
		void *adapterMemory __attribute__((aligned(8)));
		rc_scsi_info_t rc_scsi_info __attribute__((aligned(8)));
	} u;
} rc_send_arg_t;

//
//  Enumerate <C>all <T>ype for <R>eceive
//
#define RC_CTR_TEST 1000
#define RC_CTR_PRINT_VA 1001
#define RC_CTR_SRB_DONE 1002
#define RC_CTR_INIT_DONE 1003
#define RC_CTR_ASSERTION_FAILURE 1004
#define RC_CTR_EVENT 1005
#define RC_CTR_MAP_MEMORY 1006
#define RC_CTR_UNMAP_MEMORY 1007
#define RC_CTR_WAIT_MICROSECONDS 1008
#define RC_CTR_MEMORY_OP 1009
#define RC_PCI_READ_CONFIG_DWORD 1011
#define RC_PCI_WRITE_CONFIG_DWORD 1012
#define RC_PCI_READ_IO 1013
#define RC_PCI_WRITE_IO 1014
#define RC_PCI_READ_CONFIG_BYTE 1015
#define RC_PCI_WRITE_CONFIG_BYTE 1016
#define RC_CTR_VMAP_MEMORY 1017
#define RC_CTR_SCHEDULE_DPC 1018
#define RC_CTR_GET_DMA_ADDRESS 1019
#define RC_CTR_ACCESS_OK 1020
#define RC_ACPI_INVOKE 1021
#define RC_ACPI_REGISTER 1022
#define RC_PCI_READ_CONFIG_WORD 1023

#define RC_CTR_PFTYPE_32_BIT 1
#define RC_CTR_PFTYPE_64_BIT 2
#define RC_CTR_PFTYPE_POINTER 3

#define RC_CTR_EVENT_CONFIG_CHANGE_DETECTED 101
#define RC_CTR_EVENT_CONFIG_ARRAY_OFFLINE 102
#define RC_CTR_EVENT_CONFIG_DISK_OFFLINE 103

// After printing, system should be crashed
#define RC_CTR_PRINTF_SEV_PANIC 0
// Errors that must be printed
#define RC_CTR_PRINTF_SEV_ALERT 1
// Errors that must be printed
#define RC_CTR_PRINTF_SEV_CRITICAL 2
// Warnings that should be printed
#define RC_CTR_PRINTF_SEV_ERROR 3
// Warnings that should be printed
#define RC_CTR_PRINTF_SEV_WARNING 4
// Warnings that should be printed
#define RC_CTR_PRINTF_SEV_NOTICE 5
// Informational messages
#define RC_CTR_PRINTF_SEV_INFO 6
// Uncatagorized output, but by may be more than debug
#define RC_CTR_PRINTF_SEV_UNKNOWN 7
// Debug only starting point: Any severity at the level
#define RC_CTR_PRINTF_SEV_DEBUG_MIN 8
typedef struct alloc_dma_address_s {
	u32 bytes;
	void *cpu_addr;
	void *dev_handle;
	u64 dmaHandle;
} alloc_dma_address_t;

//
//    Structure which contains the receive argument information
//
typedef struct alloc_dma_map_s {
	u64 physical_address; // RETURNS: Physical address of translation
	u64 address_handle; // RETURNS: Handle associated with address
	u64 number_bytes; // IN/OUT: Number of contiguous bytes
	// translation is valid for
	void *dev_handle; // device structure pointer
	rc_srb_t *srb;
	u64 address;
	u16 memory_id; // Memory ID for what is being mapped.
} alloc_dma_map_t, map_memory_t;

#define map_memory_s alloc_dma_map_s
#define unmap_memory_s free_dma_map_s

typedef struct free_dma_map_s {
	void *dev_handle; // device structure pointer
	rc_srb_t *srb;
	u64 physical_address; // phys addr to unmap
	u64 address_handle; // Handle for address map
	u64 number_bytes; // number of bytes to unmap
	u16 memory_id; // Memory ID for what is being mapped.
} free_dma_map_t, unmap_memory_t;

struct rc_receive_arg_s;

typedef void(rc_acpi_callback_t)(struct rc_receive_arg_s *);

typedef struct rc_receive_arg_s {
	u32 call_type;
	u32 padding;
	union {
		struct {
			void *va_l;
			const char *format;
			u32 severity;
		} print_va;
		struct {
			struct rc_srb_s *srb;
		} srb_done;
		struct {
			char *file_name;
			u32 line_number;
		} assertion_failure;
		struct {
			u32 rc_notification_type;
			u8 rc_bus_changed;
		} event;
		map_memory_t map_memory;
		unmap_memory_t unmap_memory;
		rc_mem_op_t *mem_op;
		rc_access_ok_t isAccessOk;
		rc_usr_mem_t usrMem;
		rc_pci_op_t pci_op;
		rc_pci_io_t pci_io;
		struct {
			u32 microseconds;
		} wait_microseconds;
		struct {
			char *method; // ACPI method name -- based off _SB.PCI0.SATA if not "^\"
			rc_acpi_callback_t *
				callback; // function to call when ACPI call complete -- func(*context, *args);
			void *inPtr; // pointer to method input parameters
			u32 inSize; // size of input parameters
			void *outPtr; // pointer to method or value output parameters
			u32 outSize; // size of output parameters
			u32 status; // status of ACPI call
			void *context; // context for callback function
		} acpi;
		alloc_dma_address_t get_dma_memory;
	} u;
} rc_receive_arg_t;

//
//    Base structure that allows communication with hardware interface
//
typedef struct rc_interface_s {
	u32 cookie_lo;
	u32 cookie_hi;
	u32 version;
	u32 checksum;
	rc_function_t *check_interrupt_function;
	rc_function_t *send_function;
	struct rc_send_arg_s *send_arg;
	void *check_interrupt_arg;

	rc_function_t *receive_function;
	rc_function_t *schedule_dpc_function;
	rc_function_cpuid_t *cpuid_function;
	struct rc_receive_arg_s *receive_arg;
} rc_interface_t;

/*
 * function protos
 */
void rc_msg_mem_op(rc_mem_op_t *mop);

#endif //RC_MSG_H
