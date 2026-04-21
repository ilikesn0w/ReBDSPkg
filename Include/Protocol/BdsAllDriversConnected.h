/** @file
  Copyright (c) 2026, ilikesn0w. All rights reserved.
  SPDX-License-Identifier: BSD-3-Clause
**/

#ifndef BDS_ALL_DRIVERS_CONNECTED_H
#define BDS_ALL_DRIVERS_CONNECTED_H

#include <Uefi.h>

/**
  BDS_ALL_DRIVERS_CONNECTED_PROTOCOL_GUID

  This protocol was found in AMI Aptio 4 and 5 BDS.

  Once it appears in the system BDS stops modifying the boot variables,
  so this is the ideal time to start our boot management.

  DBC9FD21-FAD8-45B0-9E78-27158867CC93
 **/
#define BDS_ALL_DRIVERS_CONNECTED_PROTOCOL_GUID \
{ 0xDBC9FD21, 0xFAD8, 0x45B0,                   \
  { 0x9E, 0x78, 0x27, 0x15, 0x88, 0x67, 0xCC, 0x93 } }

extern EFI_GUID  gBdsAllDriversConnectedProtocolGuid;

#endif // BDS_ALL_DRIVERS_CONNECTED_H
