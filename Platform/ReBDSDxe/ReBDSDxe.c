/** @file
  Copyright (c) 2026, ilikesn0w. All rights reserved.
  SPDX-License-Identifier: BSD-3-Clause

  NOTE: Some DriverBinding/ComponentName code was taken from PartitionDxe
  Copyright (c) 2018 Qualcomm Datacenter Technologies, Inc.
  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "ReBDSDxePrivate.h"

//
// ReBDSDxe Driver Global Variables.
//
EFI_DRIVER_BINDING_PROTOCOL  gReBDSDxeDriverBinding = {
  ReBDSDxeDriverBindingSupported,
  ReBDSDxeDriverBindingStart,
  ReBDSDxeDriverBindingStop,
  0xb,
  NULL,
  NULL
};

EFI_STATUS
EFIAPI
ReBDSDxeDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReBDSDxeDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
ReBDSDxeDriverBindingStop (
  IN  EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                   ControllerHandle,
  IN  UINTN                        NumberOfChildren,
  IN  EFI_HANDLE                   *ChildHandleBuffer
  )
{
  return EFI_SUCCESS;
}

STATIC
BOOLEAN
IsDevicePathSane (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node;
  UINTN                           Index;
  UINTN                           Len;

  if (DevicePath == NULL) {
    return FALSE;
  }

  Node = DevicePath;

  for (Index = 0; Index < MAX_DEVICE_PATH_NODES; Index++) {
    Len = DevicePathNodeLength (Node);

    if (Len < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      return FALSE;
    }

    if (Len > MAX_DEVICE_PATH_NODE_SIZE) {
      return FALSE;
    }

    if (IsDevicePathEnd (Node)) {
      return (DevicePathSubType (Node) == END_ENTIRE_DEVICE_PATH_SUBTYPE);
    }

    Node = NextDevicePathNode (Node);
  }

  return FALSE;
}

STATIC
BOOLEAN
EFIAPI
IsDevicePathEqual (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *Path1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *Path2
  )
{
  if (!IsDevicePathSane (Path1) || !IsDevicePathSane (Path2)) {
    return FALSE;
  }

  EFI_DEVICE_PATH_PROTOCOL  *Node1 = (EFI_DEVICE_PATH_PROTOCOL *)Path1;
  EFI_DEVICE_PATH_PROTOCOL  *Node2 = (EFI_DEVICE_PATH_PROTOCOL *)Path2;

  while (!IsDevicePathEnd (Node1) && !IsDevicePathEnd (Node2)) {
    UINTN  Len1 = DevicePathNodeLength (Node1);
    UINTN  Len2 = DevicePathNodeLength (Node2);

    if ((Len1 < sizeof (EFI_DEVICE_PATH_PROTOCOL)) ||
        (Len2 < sizeof (EFI_DEVICE_PATH_PROTOCOL)) ||
        (Len1 > MAX_DEVICE_PATH_NODE_SIZE) ||
        (Len2 > MAX_DEVICE_PATH_NODE_SIZE))
    {
      return FALSE;
    }

    if (Len1 != Len2) {
      return FALSE;
    }

    if (CompareMem (Node1, Node2, Len1) != 0) {
      return FALSE;
    }

    Node1 = NextDevicePathNode (Node1);
    Node2 = NextDevicePathNode (Node2);
  }

  return IsDevicePathEnd (Node1) && IsDevicePathEnd (Node2);
}

STATIC
CONST CHAR16 *
GetFileNameFromMediaFilePathNode (
  IN EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  UINTN  Len;

  if (DevicePath == NULL) {
    return NULL;
  }

  if ((DevicePathType (DevicePath) != MEDIA_DEVICE_PATH) ||
      (DevicePathSubType (DevicePath) != MEDIA_FILEPATH_DP))
  {
    return NULL;
  }

  Len = DevicePathNodeLength (DevicePath);
  if (Len <= sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
    return NULL;
  }

  if (((Len - sizeof (EFI_DEVICE_PATH_PROTOCOL)) % sizeof (CHAR16)) != 0) {
    return NULL;
  }

  return (CONST CHAR16 *)((UINT8 *)DevicePath + sizeof (EFI_DEVICE_PATH_PROTOCOL));
}

STATIC
EFI_STATUS
GetVolumeLabel (
  IN  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *FileSystem,
  OUT CHAR16                           **Label
  )
{
  EFI_STATUS                    Status;
  EFI_FILE_PROTOCOL             *Volume     = NULL;
  EFI_FILE_SYSTEM_VOLUME_LABEL  *VolumeInfo = NULL;
  UINTN                         InfoSize    = 0;

  if (Label == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Label = NULL;

  if (FileSystem == NULL) {
    DEBUG ((DEBUG_ERROR, "GetVolumeLabel: FileSystem is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = FileSystem->OpenVolume (FileSystem, &Volume);
  if (EFI_ERROR (Status) || (Volume == NULL)) {
    DEBUG ((DEBUG_ERROR, "GetVolumeLabel: OpenVolume failed: %r\n", Status));
    return EFI_NOT_FOUND;
  }

  Status = Volume->GetInfo (
                     Volume,
                     &gEfiFileSystemVolumeLabelInfoIdGuid,
                     &InfoSize,
                     NULL
                     );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_INFO, "GetVolumeLabel: volume label info not supported (%r)\n", Status));
    Volume->Close (Volume);
    return EFI_NOT_FOUND;
  }

  VolumeInfo = AllocatePool (InfoSize);
  if (VolumeInfo == NULL) {
    Volume->Close (Volume);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = Volume->GetInfo (
                     Volume,
                     &gEfiFileSystemVolumeLabelInfoIdGuid,
                     &InfoSize,
                     VolumeInfo
                     );
  Volume->Close (Volume);

  if (EFI_ERROR (Status) || (VolumeInfo->VolumeLabel[0] == L'\0')) {
    DEBUG ((DEBUG_INFO, "GetVolumeLabel: empty label or read failed (%r)\n", Status));
    FreePool (VolumeInfo);
    return EFI_NOT_FOUND;
  }

  *Label = AllocateCopyPool (StrSize (VolumeInfo->VolumeLabel), VolumeInfo->VolumeLabel);
  FreePool (VolumeInfo);

  if (*Label == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "Volume label: %s\n", *Label));
  return EFI_SUCCESS;
}

STATIC
BOOLEAN
IsBootOptionFilePathValid (
  IN EFI_BOOT_MANAGER_LOAD_OPTION  *LoadOption
  )
{
  EFI_STATUS                       Status;
  EFI_DEVICE_PATH_PROTOCOL         *OriginalPath  = NULL;
  EFI_DEVICE_PATH_PROTOCOL         *RemainingPath = NULL;
  EFI_HANDLE                       FsHandle       = NULL;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs            = NULL;
  EFI_FILE_PROTOCOL                *Root          = NULL;
  EFI_FILE_PROTOCOL                *File          = NULL;
  CONST CHAR16                     *FileName;

  if ((LoadOption == NULL) || (LoadOption->FilePath == NULL)) {
    return FALSE;
  }

  OriginalPath = DuplicateDevicePath (LoadOption->FilePath);
  if (OriginalPath == NULL) {
    return FALSE;
  }

  RemainingPath = OriginalPath;

  Status = gBS->LocateDevicePath (
                  &gEfiSimpleFileSystemProtocolGuid,
                  &RemainingPath,
                  &FsHandle
                  );
  if (EFI_ERROR (Status) || (FsHandle == NULL) || (RemainingPath == NULL)) {
    FreePool (OriginalPath);
    return FALSE;
  }

  Status = gBS->HandleProtocol (
                  FsHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID **)&Fs
                  );
  if (EFI_ERROR (Status) || (Fs == NULL)) {
    FreePool (OriginalPath);
    return FALSE;
  }

  Status = Fs->OpenVolume (Fs, &Root);
  if (EFI_ERROR (Status) || (Root == NULL)) {
    FreePool (OriginalPath);
    return FALSE;
  }

  FileName = GetFileNameFromMediaFilePathNode (RemainingPath);
  if ((FileName == NULL) || (FileName[0] == L'\0')) {
    Root->Close (Root);
    FreePool (OriginalPath);
    return FALSE;
  }

  Status = Root->Open (
                   Root,
                   &File,
                   (CHAR16 *)FileName,
                   EFI_FILE_MODE_READ,
                   0
                   );

  if (File != NULL) {
    File->Close (File);
  }

  Root->Close (Root);
  FreePool (OriginalPath);

  return !EFI_ERROR (Status);
}

STATIC
BOOLEAN
BootOptionAlreadyExistsForPath (
  IN EFI_DEVICE_PATH_PROTOCOL      *CandidatePath,
  IN EFI_BOOT_MANAGER_LOAD_OPTION  *BootOptions,
  IN UINTN                         Count
  )
{
  UINTN  Index;

  if ((CandidatePath == NULL) || (BootOptions == NULL)) {
    return FALSE;
  }

  for (Index = 0; Index < Count; Index++) {
    if (BootOptions[Index].OptionType != LoadOptionTypeBoot) {
      continue;
    }

    if ((BootOptions[Index].FilePath != NULL) &&
        IsDevicePathEqual (BootOptions[Index].FilePath, CandidatePath))
    {
      return TRUE;
    }
  }

  return FALSE;
}

STATIC
BOOLEAN
IsBootPathPresent (
  IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs,
  IN CONST CHAR16                     *BootPath
  )
{
  EFI_STATUS         Status;
  EFI_FILE_PROTOCOL  *Root = NULL;
  EFI_FILE_PROTOCOL  *File = NULL;

  if ((Fs == NULL) || (BootPath == NULL) || (BootPath[0] == L'\0')) {
    return FALSE;
  }

  Status = Fs->OpenVolume (Fs, &Root);
  if (EFI_ERROR (Status) || (Root == NULL)) {
    return FALSE;
  }

  Status = Root->Open (
                   Root,
                   &File,
                   (CHAR16 *)BootPath,
                   EFI_FILE_MODE_READ,
                   0
                   );

  if (File != NULL) {
    File->Close (File);
  }

  Root->Close (Root);

  return !EFI_ERROR (Status);
}

STATIC
EFI_STATUS
GetBootDescription (
  IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs,
  IN CONST CHAR16                     *BootPath,
  OUT CHAR16                          **Description
  )
{
  CONST CHAR16  *StaticDescription;

  if ((Fs == NULL) || (BootPath == NULL) || (Description == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *Description = NULL;

  if (StrCmp (BootPath, L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi") == 0) {
    StaticDescription = L"Windows Boot Manager";
  } else if (StrStr (BootPath, L"\\EFI\\debian\\") != NULL) {
    StaticDescription = L"Debian";
  } else if (StrStr (BootPath, L"\\EFI\\ubuntu\\") != NULL) {
    StaticDescription = L"Ubuntu";
  } else if (StrStr (BootPath, L"\\EFI\\fedora\\") != NULL) {
    StaticDescription = L"Fedora";
  } else if (StrStr (BootPath, L"\\EFI\\arch\\") != NULL) {
    StaticDescription = L"Arch";
  } else {
    StaticDescription = NULL;
  }

  if (StaticDescription != NULL) {
    *Description = AllocateCopyPool (StrSize (StaticDescription), StaticDescription);
    return (*Description == NULL) ? EFI_OUT_OF_RESOURCES : EFI_SUCCESS;
  }

  return GetVolumeLabel (Fs, Description);
}

STATIC
EFI_STATUS
AddBootOptionForHandleAndPath (
  IN EFI_HANDLE    FsHandle,
  IN CONST CHAR16  *BootPath
  )
{
  EFI_STATUS                       Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL  *Fs             = NULL;
  EFI_DEVICE_PATH_PROTOCOL         *CandidatePath  = NULL;
  EFI_BOOT_MANAGER_LOAD_OPTION     *BootOptions    = NULL;
  UINTN                            BootOptionCount = 0;
  CHAR16                           *Description    = NULL;
  EFI_BOOT_MANAGER_LOAD_OPTION     Option;

  Status = gBS->HandleProtocol (
                  FsHandle,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID **)&Fs
                  );
  if (EFI_ERROR (Status) || (Fs == NULL)) {
    return EFI_NOT_FOUND;
  }

  if (!IsBootPathPresent (Fs, BootPath)) {
    return EFI_NOT_FOUND;
  }

  CandidatePath = FileDevicePath (FsHandle, (CHAR16 *)BootPath);
  if (CandidatePath == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);
  if (BootOptions != NULL) {
    if (BootOptionAlreadyExistsForPath (CandidatePath, BootOptions, BootOptionCount)) {
      EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
      FreePool (CandidatePath);
      return EFI_ALREADY_STARTED;
    }

    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
  }

  Status = GetBootDescription (Fs, BootPath, &Description);
  if (EFI_ERROR (Status) || (Description == NULL)) {
    Description = AllocateCopyPool (StrSize (L"NO NAME"), L"NO NAME");
    if (Description == NULL) {
      FreePool (CandidatePath);
      return EFI_OUT_OF_RESOURCES;
    }
  }

  ZeroMem (&Option, sizeof (Option));
  Option.OptionNumber = LoadOptionNumberUnassigned;
  Option.OptionType   = LoadOptionTypeBoot;
  Option.Attributes   = LOAD_OPTION_ACTIVE;
  Option.Description  = Description;
  Option.FilePath     = CandidatePath;

  Status = EfiBootManagerAddLoadOptionVariable (&Option, 0);

  FreePool (Description);
  FreePool (CandidatePath);

  return Status;
}

STATIC
VOID
RemoveInvalidBootOptions (
  VOID
  )
{
  EFI_BOOT_MANAGER_LOAD_OPTION  *LoadOptions = NULL;
  UINTN                         Count        = 0;
  UINTN                         Index;

  LoadOptions = EfiBootManagerGetLoadOptions (&Count, LoadOptionTypeBoot);
  if (LoadOptions == NULL) {
    return;
  }

  for (Index = 0; Index < Count; Index++) {
    if (LoadOptions[Index].FilePath == NULL) {
      continue;
    }

    if (!IsDevicePathSane (LoadOptions[Index].FilePath)) {
      DEBUG ((DEBUG_WARN, "Skipping malformed Boot%04x\n", (UINT16)LoadOptions[Index].OptionNumber));
      continue;
    }

    if (!IsBootOptionFilePathValid (&LoadOptions[Index])) {
      DEBUG ((
        DEBUG_INFO,
        "Deleting Boot%04x (Attributes=%x, FilePath=%p)\n",
        (UINT16)LoadOptions[Index].OptionNumber,
        LoadOptions[Index].Attributes,
        LoadOptions[Index].FilePath
        ));
      EfiBootManagerDeleteLoadOptionVariable (
        LoadOptions[Index].OptionNumber,
        LoadOptionTypeBoot
        );
    }
  }

  EfiBootManagerFreeLoadOptions (LoadOptions, Count);
}

STATIC
VOID
SyncBootOptions (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *Handles    = NULL;
  UINTN       HandleCount = 0;
  UINTN       Index;
  UINTN       PathIndex;

  RemoveInvalidBootOptions ();

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  if (EFI_ERROR (Status) || (Handles == NULL)) {
    DEBUG ((DEBUG_INFO, "SyncBootOptions: no filesystem handles (%r)\n", Status));
    return;
  }

  for (Index = 0; Index < HandleCount; Index++) {
    for (PathIndex = 0; PathIndex < PathNum; PathIndex++) {
      Status = AddBootOptionForHandleAndPath (
                 Handles[Index],
                 BootPaths[PathIndex]
                 );

      if (!EFI_ERROR (Status) || (Status == EFI_ALREADY_STARTED)) {
        break;
      }

      if ((Status != EFI_NOT_FOUND) && EFI_ERROR (Status)) {
        DEBUG ((
          DEBUG_WARN,
          "SyncBootOptions: Failed on handle %u path %u: %r\n",
          Index,
          PathIndex,
          Status
          ));
      }
    }
  }

  FreePool (Handles);
}

STATIC
VOID
EFIAPI
MainInit (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  SyncBootOptions ();
}

STATIC
EFI_STATUS
EFIAPI
NotifyAllDriversConnected (
  EFI_EVENT_NOTIFY  NotifyFunction,
  VOID              *Context,
  EFI_EVENT         *Event,
  VOID              **Registration
  )
{
  EFI_STATUS  Status;

  if ((Event == NULL) || (Registration == NULL) || (NotifyFunction == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  NotifyFunction,
                  Context,
                  Event
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return gBS->RegisterProtocolNotify (
                &gBdsAllDriversConnectedProtocolGuid,
                *Event,
                Registration
                );
}

EFI_STATUS
EFIAPI
ReBDSDxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_EVENT   Event;
  VOID        *Registration;
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "ReBDSDxe initialized, version %s\n", REBDSDXE_VERSION));

  if (StrCmp (gST->FirmwareVendor, L"American Megatrends") != 0) {
    DEBUG ((DEBUG_ERROR, "Unsupported firmware: %s\n", gST->FirmwareVendor));
    DEBUG ((DEBUG_ERROR, "Driver is shutting down in:\n"));
    gBS->Stall (1000000);
    DEBUG ((DEBUG_ERROR, "3\n"));
    gBS->Stall (1000000);
    DEBUG ((DEBUG_ERROR, "2\n"));
    gBS->Stall (1000000);
    DEBUG ((DEBUG_ERROR, "1\n"));
    gST->ConOut->ClearScreen (gST->ConOut);

    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "Trying to install DB/CN/CN2 protocols\n"));
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gReBDSDxeDriverBinding,
             ImageHandle,
             &gReBDSDxeComponentName,
             &gReBDSDxeComponentName2
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to install DB/CN/CN2 protocols - %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Trying to install BdsAllDriversConnected callback\n"));
  Status = NotifyAllDriversConnected (MainInit, NULL, &Event, &Registration);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to register BdsAllDriversConnected callback: %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
