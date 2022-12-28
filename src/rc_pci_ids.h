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

/****************************************************************************\
*
*   File used to describe known RAIDCore PCI vendors and devices
*
\****************************************************************************/

#ifndef _RC_PCI_IDS_H
#define _RC_PCI_IDS_H

/****************************************************************************\
*
*   Vendor IDs (PD_VID - <P>roduct <D>escription <V>endor <ID>)
*
*       These are the vendor specific PCI IDs given to each company (vendor)
*       by the PCI SIG when a company joins it.
*
\****************************************************************************/
#define RC_PD_VID_RAIDCORE 0x182f
#define RC_PD_VID_INTEL 0x8086
#define RC_PD_VID_SERVERWORKS 0x1166
#define RC_PD_VID_ATI 0x1002
#define RC_PD_VID_AMD 0x1022
#define RC_PD_VID_LSI 0x1000
#define RC_PD_VID_DELL 0x1028
#define RC_PD_VID_SEAGATE 0x1BB1

#define RC_PD_VID_RAMDISK 0x8590
#define RC_PD_VID_HP 0x103C
#define RC_PD_VID_SAMSUNG 0x144D
#define RC_PD_VID_ASMEDIA 0x1B21

#define RC_PD_VID_GENERIC_NVME 0xFFFF

#define RC_PD_CLASS_RAID 0x4
#define RC_PD_CLASS_SATA 0x6
#define RC_PD_CLASS_NVME 0x8

/****************************************************************************\
*
*   Product IDs (PD_DID - <P>roduct <D>escription <D>evice <ID>)
*
*       These are the vendor specific Device IDs that the vendor assigns
*       to each unique device that they are producing. The standard
*       layout for these IDs is RC_PD_DID_XX_NNNN where XX is a two
*       letter symbol summarizing the Vendor that assigned the ID, and
*       NNNN is the name of their device represented by it.
*
\****************************************************************************/
//
// Definition by board type
//
// RAIDCore 8 channel SW241 based SATA RAID controllers
#define RC_PD_DID_RC_RC48XX_SW 0x0005
// RAIDCore 4 channel SW242 based SATA RAID controllers
#define RC_PD_DID_RC_RC44XX_SW 0x0006

//
// Generic definition or by default chip PCI Device ID
//
// don't forget to update the equivalent IDs in fulcrum\rc\platforms\rcbottom\rcbottom\device.h
//
// ServerWorks (Frodo SATA-1 8-port)
#define RC_PD_DID_SW_0241 0x0241
// ServerWorks (Frodo SATA-1 4-port)
#define RC_PD_DID_SW_0242 0x0242
// ICH10 RAID
#define RC_PD_DID_ICH10 0x3A25
// AMD's VSTOR Device
#define RC_PD_DID_VSTOR_DEVICE RC_PD_VID_AMD

// AMD BOLTON RAID SWL 10
#define RC_PD_DID_BOLTON_10 0x7802
// AMD BOLTON RAID SWL 5X
#define RC_PD_DID_BOLTON_5X 0x7803

// AMD ATI SP5100 SWL 52
#define RC_PD_DID_SP5100_52 0x4392
// AMD ATI SP5100 SWL 60
#define RC_PD_DID_SP5100_60 0x4393
// AMD BOLTON AHCI Driver
#define RC_PD_DID_AHCI_0A 0x7801
// AMD BOLTON AHCI Driver
#define RC_PD_DID_AHCI_0B 0x7804
// AMD Client Bolton    SWL 10
// AMD Server Riverside SWL 50
#define RC_PD_DID_CBSR 0x7805
// PCIe AMD Bristol RAID mode
#define RC_PD_DID_BRISTOL 0x7905
// PCIe AMD Promontory Chipset
#define RC_PD_DID_PROMONTORY 0x43BD
// PCIe AMD Summit AM4 RAID mode
#define RC_PD_DID_SUMMIT 0x7916
// PCIe AMD Summit SP3 RAID mode
#define RC_PD_DID_SUMMIT_SP3 0x7917
// PCIe LSI1068 Adapater
#define RC_PD_DID_LSI1068E 0x0058
// Motherboard LSI1068
#define RC_PD_DID_LSI1068 0x0059
// MPT2 Falcon Adapter
#define RC_PD_DID_FALCON 0x0072
// DELL's LSI 1068e HBA (S300)
#define RC_PD_DID_DELL1068E 0x0016
// DELL's LSI 2008e HBA
#define RC_PD_DID_DELL2008E 0x0072
// Samsung Turbo SSD
#define RC_PD_DID_TURBO_SSD 0xA800
// Samsung Turbo SSD
#define RC_PD_DID_TURBO_SSD1 0xA801
// Samsung Turbo SSD
#define RC_PD_DID_TURBO_SSD2 0xA802
#define RC_PD_DID_ASMEDIA_AHCI 0x0612
// HP Z820 AHCI MODE
#define RC_PD_DID_HPZ820 0x1D02
#define RC_PD_DID_SEAGATE_JAGUAR 0x0100

// HP Sub Device IDs
// HP KINGCOBRA - AMD BOLTON RAID Driver
#define RC_PD_SDID_KINGCOBRA 0x1850
// HP JASMINE - AMD BOLTON RAID Driver
#define RC_PD_SDID_JASMINE 0x2AE0
// HP BATGIRL - AMD KABINI RAID Driver
#define RC_PD_SDID_BATGIRL 0x2215
// HP ORCHID - AMD KABINI RAID Driver
#define RC_PD_SDID_ORCHID 0x2B17
// HP GLADIATOR - AMD KABINI RAID Driver
#define RC_PD_SDID_GLADIATOR 0x225F
// HP ORCHID2 - AMD KABINI RAID Driver
#define RC_PD_SDID_ORCHID2 0x2B35

#define AMD_NVME_DID 0xB000

#endif //_RC_PCI_IDS_H
