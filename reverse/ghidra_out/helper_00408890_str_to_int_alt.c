// str_to_int_alt  addr=00408890  func=FUN_00408890


void FUN_00408890(void)

{
  undefined4 uVar1;
  undefined4 extraout_ECX;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  undefined1 local_1c [4];
  undefined1 local_18;
  undefined4 local_14;
  float10 local_e;
  
  puStack_24 = &stack0xfffffffc;
  local_14 = 0;
  puStack_28 = &LAB_004088fa;
  uStack_2c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_2c;
  FUN_00403d8c();
  uVar1 = FUN_0040ac97('\0',&local_e);
  if ((char)uVar1 == '\0') {
    FUN_0040504c(extraout_ECX);
    local_18 = 0xb;
    FUN_00407a6c(0,local_1c);
  }
  *unaff_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_00408901;
  puStack_28 = (undefined1 *)0x4088f9;
  FUN_0040394c();
  return;
}

