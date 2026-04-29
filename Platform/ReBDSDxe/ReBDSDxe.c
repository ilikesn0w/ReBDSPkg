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
// all possible boot paths for now
// will be expanded tomorrow and will be customizable through config
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR16  *BootPaths[] = {
  L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi",     // Windows

  L"\\EFI\\Arch\\shimx64.efi",                // Arch shim
  L"\\EFI\\Arch\\grubx64.efi",                // Arch grub

  L"\\EFI\\Astra\\shimx64.efi",               // Astra shim
  L"\\EFI\\Astra\\grubx64.efi",               // Astra grub

  L"\\EFI\\CentOS\\shimx64.efi",              // CentOS shim
  L"\\EFI\\CentOS\\grubx64.efi",              // CentOS grub

  L"\\EFI\\Debian\\shimx64.efi",              // Debian shim
  L"\\EFI\\Debian\\grubx64.efi",              // Debian grub

  L"\\EFI\\Deepin\\shimx64.efi",              // Deepin shim
  L"\\EFI\\Deepin\\grubx64.efi",              // Deepin grub

  L"\\EFI\\elementaryOS\\shimx64.efi",        // elementaryOS shim
  L"\\EFI\\elementaryOS\\grubx64.efi",        // elementaryOS grub

  L"\\EFI\\Endless\\shimx64.efi",             // Endless shim
  L"\\EFI\\Endless\\grubx64.efi",             // Endless grub

  L"\\EFI\\Gentoo\\shimx64.efi",              // Gentoo shim
  L"\\EFI\\Gentoo\\grubx64.efi",              // Gentoo grub

  L"\\EFI\\Fedora\\shimx64.efi",              // Fedora shim
  L"\\EFI\\Fedora\\grubx64.efi",              // Fedora grub

  L"\\EFI\\KDEneon\\shimx64.efi",             // KDE neon shim
  L"\\EFI\\KDEneon\\grubx64.efi",             // KDE neon grub

  L"\\EFI\\Kali\\shimx64.efi",                // Kali shim
  L"\\EFI\\Kali\\grubx64.efi",                // Kali grub

  L"\\EFI\\Mageia\\shimx64.efi",              // Mageia shim
  L"\\EFI\\Mageia\\grubx64.efi",              // Mageia grub

  L"\\EFI\\Manjaro\\shimx64.efi",             // Manjaro shim
  L"\\EFI\\Manjaro\\grubx64.efi",             // Manjaro grub

  L"\\EFI\\Mint\\shimx64.efi",                // Mint shim
  L"\\EFI\\Mint\\grubx64.efi",                // Mint grub

  L"\\EFI\\openSUSE\\shimx64.efi",            // openSUSE shim
  L"\\EFI\\openSUSE\\grubx64.efi",            // openSUSE grub

  L"\\EFI\\Oracle\\shimx64.efi",              // Oracle Linux shim
  L"\\EFI\\Oracle\\grubx64.efi",              // Oracle Linux grub

  L"\\EFI\\PopOS\\shimx64.efi",               // Pop!_OS shim
  L"\\EFI\\PopOS\\grubx64.efi",               // Pop!_OS grub

  L"\\EFI\\redhat\\shimx64.efi",              // RedHat shim
  L"\\EFI\\redhat\\grubx64.efi",              // RedHat grub

  L"\\EFI\\RHEL\\shimx64.efi",                // RHEL shim
  L"\\EFI\\RHEL\\grubx64.efi",                // RHEL grub

  L"\\EFI\\Rocky\\shimx64.efi",               // Rocky shim
  L"\\EFI\\Rocky\\grubx64.efi",               // Rocky grub

  L"\\EFI\\Solus\\shimx64.efi",               // Solus shim
  L"\\EFI\\Solus\\grubx64.efi",               // Solus grub

  L"\\EFI\\Lubuntu\\shimx64.efi",             // Lubuntu shim
  L"\\EFI\\Lubuntu\\grubx64.efi",             // Lubuntu grub

  L"\\EFI\\UbuntuMATE\\shimx64.efi",          // Ubuntu MATE shim
  L"\\EFI\\UbuntuMATE\\grubx64.efi",          // Ubuntu MATE grub

  L"\\EFI\\Xubuntu\\shimx64.efi",             // Xubuntu shim
  L"\\EFI\\Xubuntu\\grubx64.efi",             // Xubuntu grub

  L"\\EFI\\Ubuntu\\shimx64.efi",              // Ubuntu shim
  L"\\EFI\\Ubuntu\\grubx64.efi",              // Ubuntu grub

  L"\\EFI\\Void\\shimx64.efi",                // Void shim
  L"\\EFI\\Void\\grubx64.efi",                // Void grub

  L"\\EFI\\Zorin\\shimx64.efi",               // Zorin shim
  L"\\EFI\\Zorin\\grubx64.efi",               // Zorin grub

  L"\\EFI\\MX\\shimx64.efi",                  // MX shim
  L"\\EFI\\MX\\grubx64.efi",                  // MX grub

  L"\\EFI\\BOOT\\BOOTX64.EFI"                 // default
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINTN  PathNum =
  ARRAY_SIZE (BootPaths);

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
IsBbsDevicePath (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath
  )
{
  if (DevicePath == NULL) {
    return FALSE;
  }

  return (DevicePathType (DevicePath) == BBS_DEVICE_PATH);
}

STATIC
BOOLEAN
IsDevicePathSane (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  IN UINTN                           DevicePathSize
  )
{
  CONST UINT8                     *Ptr;
  CONST UINT8                     *End;
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node;
  UINTN                           Len;
  UINTN                           Index;

  if (DevicePath == NULL) {
    return FALSE;
  }

  if (DevicePathSize < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
    return FALSE;
  }

  Ptr = (CONST UINT8 *)DevicePath;
  End = Ptr + DevicePathSize;

  for (Index = 0; Index < MAX_DEVICE_PATH_NODES; Index++) {
    if ((UINTN)(End - Ptr) < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      return FALSE;
    }

    Node = (CONST EFI_DEVICE_PATH_PROTOCOL *)Ptr;
    Len  = DevicePathNodeLength (Node);

    if (Len < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
      return FALSE;
    }

    if (Len > MAX_DEVICE_PATH_NODE_SIZE) {
      return FALSE;
    }

    if (Len > (UINTN)(End - Ptr)) {
      return FALSE;
    }

    if (IsDevicePathEnd (Node)) {
      if (DevicePathSubType (Node) != END_ENTIRE_DEVICE_PATH_SUBTYPE) {
        return FALSE;
      }

      return (Len == sizeof (EFI_DEVICE_PATH_PROTOCOL)) &&
             ((UINTN)(End - Ptr) == Len);
    }

    Ptr += Len;
  }

  return FALSE;
}

STATIC
BOOLEAN
IsDevicePathEqual (
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *Path1,
  IN UINTN                           Size1,
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *Path2,
  IN UINTN                           Size2
  )
{
  CONST UINT8  *Ptr1 = (CONST UINT8 *)Path1;
  CONST UINT8  *End1 = Ptr1 + Size1;

  CONST UINT8  *Ptr2 = (CONST UINT8 *)Path2;
  CONST UINT8  *End2 = Ptr2 + Size2;

  UINTN                           Len1, Len2;
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node1;
  CONST EFI_DEVICE_PATH_PROTOCOL  *Node2;

  while (TRUE) {
    if (((UINTN)(End1 - Ptr1) < sizeof (EFI_DEVICE_PATH_PROTOCOL)) ||
        ((UINTN)(End2 - Ptr2) < sizeof (EFI_DEVICE_PATH_PROTOCOL)))
    {
      return FALSE;
    }

    Node1 = (CONST EFI_DEVICE_PATH_PROTOCOL *)Ptr1;
    Node2 = (CONST EFI_DEVICE_PATH_PROTOCOL *)Ptr2;

    Len1 = DevicePathNodeLength (Node1);
    Len2 = DevicePathNodeLength (Node2);

    if ((Len1 != Len2) ||
        (Len1 < sizeof (EFI_DEVICE_PATH_PROTOCOL)) ||
        (Len1 > MAX_DEVICE_PATH_NODE_SIZE) ||
        (Len1 > (UINTN)(End1 - Ptr1)) ||
        (Len2 > (UINTN)(End2 - Ptr2)))
    {
      return FALSE;
    }

    if (CompareMem (Node1, Node2, Len1) != 0) {
      return FALSE;
    }

    if (IsDevicePathEnd (Node1)) {
      return IsDevicePathEnd (Node2) &&
             (DevicePathSubType (Node1) == END_ENTIRE_DEVICE_PATH_SUBTYPE) &&
             (DevicePathSubType (Node2) == END_ENTIRE_DEVICE_PATH_SUBTYPE) &&
             ((UINTN)(End1 - Ptr1) == Len1) &&
             ((UINTN)(End2 - Ptr2) == Len2);
    }

    Ptr1 += Len1;
    Ptr2 += Len2;
  }
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
    DEBUG ((DEBUG_ERROR, "REBDS: GetVolumeLabel: FileSystem is NULL\n"));
    return EFI_INVALID_PARAMETER;
  }

  Status = FileSystem->OpenVolume (FileSystem, &Volume);
  if (EFI_ERROR (Status) || (Volume == NULL)) {
    DEBUG ((DEBUG_ERROR, "REBDS: GetVolumeLabel: OpenVolume failed: %r\n", Status));
    return EFI_NOT_FOUND;
  }

  Status = Volume->GetInfo (
                     Volume,
                     &gEfiFileSystemVolumeLabelInfoIdGuid,
                     &InfoSize,
                     NULL
                     );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    DEBUG ((DEBUG_INFO, "REBDS: GetVolumeLabel: volume label info not supported (%r)\n", Status));
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
    DEBUG ((DEBUG_INFO, "REBDS: GetVolumeLabel: empty label or read failed (%r)\n", Status));
    FreePool (VolumeInfo);
    return EFI_NOT_FOUND;
  }

  *Label = AllocateCopyPool (StrSize (VolumeInfo->VolumeLabel), VolumeInfo->VolumeLabel);
  FreePool (VolumeInfo);

  if (*Label == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "REBDS: Volume label: %s\n", *Label));
  return EFI_SUCCESS;
}

STATIC
BOOLEAN
GetDevicePathSizeSafe (
  IN  CONST EFI_DEVICE_PATH_PROTOCOL  *DevicePath,
  OUT UINTN                           *DevicePathSize
  )
{
  UINTN  Size;

  if ((DevicePath == NULL) || (DevicePathSize == NULL)) {
    return FALSE;
  }

  Size = GetDevicePathSize ((EFI_DEVICE_PATH_PROTOCOL *)DevicePath);
  if (Size < sizeof (EFI_DEVICE_PATH_PROTOCOL)) {
    return FALSE;
  }

  *DevicePathSize = Size;
  return TRUE;
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
  UINTN                            FilePathSize;

  if ((LoadOption == NULL) || (LoadOption->FilePath == NULL)) {
    return FALSE;
  }

  if (!GetDevicePathSizeSafe (LoadOption->FilePath, &FilePathSize)) {
    return FALSE;
  }

  if (!IsDevicePathSane (LoadOption->FilePath, FilePathSize)) {
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
  IN CONST EFI_DEVICE_PATH_PROTOCOL  *CandidatePath,
  IN EFI_BOOT_MANAGER_LOAD_OPTION    *BootOptions,
  IN UINTN                           Count
  )
{
  UINTN  Index;
  UINTN  CandidateSize;
  UINTN  ExistingSize;

  if ((CandidatePath == NULL) || (BootOptions == NULL)) {
    return FALSE;
  }

  if (!GetDevicePathSizeSafe (CandidatePath, &CandidateSize)) {
    return FALSE;
  }

  for (Index = 0; Index < Count; Index++) {
    if (BootOptions[Index].OptionType != LoadOptionTypeBoot) {
      continue;
    }

    if (BootOptions[Index].FilePath == NULL) {
      continue;
    }

    if (!GetDevicePathSizeSafe (BootOptions[Index].FilePath, &ExistingSize)) {
      continue;
    }

    if (IsDevicePathEqual (
          CandidatePath,
          CandidateSize,
          BootOptions[Index].FilePath,
          ExistingSize
          ))
    {
      return TRUE;
    }
  }

  return FALSE;
}

STATIC
UINTN
FindFirstFreeBootOptionNumberFromFirst (
  IN EFI_BOOT_MANAGER_LOAD_OPTION  *BootOptions,
  IN UINTN                         Count
  )
{
  UINTN    Candidate;
  UINTN    Index;
  BOOLEAN  Used;

  if (BootOptions == NULL) {
    return LoadOptionNumberUnassigned;
  }

  for (Candidate = 1; Candidate <= 0xFFFF; Candidate++) {
    Used = FALSE;

    for (Index = 0; Index < Count; Index++) {
      if ((BootOptions[Index].OptionType == LoadOptionTypeBoot) &&
          (BootOptions[Index].OptionNumber == Candidate))
      {
        Used = TRUE;
        break;
      }
    }

    if (!Used) {
      return Candidate;
    }
  }

  return LoadOptionNumberUnassigned;
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
  } else if (StrStr (BootPath, L"\\EFI\\Arch\\") != NULL) {
    StaticDescription = L"Arch";
  } else if (StrStr (BootPath, L"\\EFI\\Astra\\") != NULL) {
    StaticDescription = L"Astra";
  } else if (StrStr (BootPath, L"\\EFI\\CentOS\\") != NULL) {
    StaticDescription = L"CentOS";
  } else if (StrStr (BootPath, L"\\EFI\\Debian\\") != NULL) {
    StaticDescription = L"Debian";
  } else if (StrStr (BootPath, L"\\EFI\\Deepin\\") != NULL) {
    StaticDescription = L"Deepin";
  } else if (StrStr (BootPath, L"\\EFI\\elementaryOS\\") != NULL) {
    StaticDescription = L"elementaryOS";
  } else if (StrStr (BootPath, L"\\EFI\\Endless\\") != NULL) {
    StaticDescription = L"Endless";
  } else if (StrStr (BootPath, L"\\EFI\\Gentoo\\") != NULL) {
    StaticDescription = L"Gentoo";
  } else if (StrStr (BootPath, L"\\EFI\\Fedora\\") != NULL) {
    StaticDescription = L"Fedora";
  } else if (StrStr (BootPath, L"\\EFI\\KDEneon\\") != NULL) {
    StaticDescription = L"KDE neon";
  } else if (StrStr (BootPath, L"\\EFI\\Kali\\") != NULL) {
    StaticDescription = L"Kali";
  } else if (StrStr (BootPath, L"\\EFI\\Mageia\\") != NULL) {
    StaticDescription = L"Mageia";
  } else if (StrStr (BootPath, L"\\EFI\\Manjaro\\") != NULL) {
    StaticDescription = L"Manjaro";
  } else if (StrStr (BootPath, L"\\EFI\\Mint\\") != NULL) {
    StaticDescription = L"Mint";
  } else if (StrStr (BootPath, L"\\EFI\\openSUSE\\") != NULL) {
    StaticDescription = L"openSUSE";
  } else if (StrStr (BootPath, L"\\EFI\\Oracle\\") != NULL) {
    StaticDescription = L"Oracle Linux";
  } else if (StrStr (BootPath, L"\\EFI\\PopOS\\") != NULL) {
    StaticDescription = L"Pop!_OS";
  } else if (StrStr (BootPath, L"\\EFI\\redhat\\") != NULL) {
    StaticDescription = L"RedHat";
  } else if (StrStr (BootPath, L"\\EFI\\RHEL\\") != NULL) {
    StaticDescription = L"RHEL";
  } else if (StrStr (BootPath, L"\\EFI\\Rocky\\") != NULL) {
    StaticDescription = L"Rocky";
  } else if (StrStr (BootPath, L"\\EFI\\Solus\\") != NULL) {
    StaticDescription = L"Solus";
  } else if (StrStr (BootPath, L"\\EFI\\Lubuntu\\") != NULL) {
    StaticDescription = L"Lubuntu";
  } else if (StrStr (BootPath, L"\\EFI\\UbuntuMATE\\") != NULL) {
    StaticDescription = L"Ubuntu MATE";
  } else if (StrStr (BootPath, L"\\EFI\\Xubuntu\\") != NULL) {
    StaticDescription = L"Xubuntu";
  } else if (StrStr (BootPath, L"\\EFI\\Ubuntu\\") != NULL) {
    StaticDescription = L"Ubuntu";
  } else if (StrStr (BootPath, L"\\EFI\\Void\\") != NULL) {
    StaticDescription = L"Void";
  } else if (StrStr (BootPath, L"\\EFI\\Zorin\\") != NULL) {
    StaticDescription = L"Zorin";
  } else if (StrStr (BootPath, L"\\EFI\\MX\\") != NULL) {
    StaticDescription = L"MX";
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
  UINTN                            Position;
  UINTN                            OptionNumber;
  CHAR16                           *Description = NULL;
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

    OptionNumber = FindFirstFreeBootOptionNumberFromFirst (BootOptions, BootOptionCount);
    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
  } else {
    OptionNumber = 1;
  }

  if (OptionNumber == LoadOptionNumberUnassigned) {
    FreePool (CandidatePath);
    return EFI_OUT_OF_RESOURCES;
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
  Option.OptionNumber = OptionNumber;
  Option.OptionType   = LoadOptionTypeBoot;
  Option.Attributes   = LOAD_OPTION_ACTIVE;
  Option.Description  = Description;
  Option.FilePath     = CandidatePath;

  Position = (BootOptionCount > 0) ? 1 : 0;

  Status = EfiBootManagerAddLoadOptionVariable (&Option, Position);

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
  UINTN                         FilePathSize;

  LoadOptions = EfiBootManagerGetLoadOptions (&Count, LoadOptionTypeBoot);
  if (LoadOptions == NULL) {
    return;
  }

  for (Index = 0; Index < Count; Index++) {
    if (LoadOptions[Index].FilePath == NULL) {
      continue;
    }

    if (IsBbsDevicePath (LoadOptions[Index].FilePath)) {
      DEBUG ((
        DEBUG_INFO,
        "REBDS: Skipping legacy Boot%04x (BBS)\n",
        (UINT16)LoadOptions[Index].OptionNumber
        ));
      continue;
    }

    if (!GetDevicePathSizeSafe (LoadOptions[Index].FilePath, &FilePathSize) ||
        !IsDevicePathSane (LoadOptions[Index].FilePath, FilePathSize))
    {
      DEBUG ((
        DEBUG_WARN,
        "REBDS: Deleting malformed Boot%04x\n",
        (UINT16)LoadOptions[Index].OptionNumber
        ));

      EfiBootManagerDeleteLoadOptionVariable (
        LoadOptions[Index].OptionNumber,
        LoadOptionTypeBoot
        );
      continue;
    }

    if (!IsBootOptionFilePathValid (&LoadOptions[Index])) {
      DEBUG ((
        DEBUG_INFO,
        "REBDS: Deleting Boot%04x (Attributes=%x, FilePath=%p)\n",
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
    DEBUG ((DEBUG_INFO, "REBDS: SyncBootOptions: no filesystem handles (%r)\n", Status));
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
          "REBDS: SyncBootOptions: Failed on handle %u path %u: %r\n",
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
    DEBUG ((DEBUG_ERROR, "REBDS: Unsupported firmware: %s\n", gST->FirmwareVendor));
    DEBUG ((DEBUG_ERROR, "REBDS: Driver is shutting down in:\n"));
    gBS->Stall (1000000);
    DEBUG ((DEBUG_ERROR, "3\n"));
    gBS->Stall (1000000);
    DEBUG ((DEBUG_ERROR, "2\n"));
    gBS->Stall (1000000);
    DEBUG ((DEBUG_ERROR, "1\n"));
    gST->ConOut->ClearScreen (gST->ConOut);

    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "REBDS: Trying to install DB/CN/CN2 protocols\n"));
  Status = EfiLibInstallDriverBindingComponentName2 (
             ImageHandle,
             SystemTable,
             &gReBDSDxeDriverBinding,
             ImageHandle,
             &gReBDSDxeComponentName,
             &gReBDSDxeComponentName2
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "REBDS: Unable to install DB/CN/CN2 protocols - %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "REBDS: Trying to install BdsAllDriversConnected callback\n"));
  Status = NotifyAllDriversConnected (MainInit, NULL, &Event, &Registration);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "REBDS: Failed to register BdsAllDriversConnected callback: %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}
