// str_to_int  addr=00407cdc  func=FUN_00407cdc


void __fastcall FUN_00407cdc(undefined4 param_1)

{
  undefined4 extraout_ECX;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_28;
  undefined1 *puStack_24;
  undefined1 *puStack_20;
  undefined1 local_14 [4];
  undefined1 local_10;
  undefined4 local_c;
  int local_8;
  
  puStack_20 = &stack0xfffffffc;
  local_c = 0;
  puStack_24 = &LAB_00407d44;
  uStack_28 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_28;
  FUN_00402aec(param_1,&local_8);
  if (local_8 != 0) {
    FUN_0040504c(extraout_ECX);
    local_10 = 0xb;
    FUN_00407a6c(0,local_14);
  }
  *unaff_FS_OFFSET = uStack_28;
  puStack_20 = &LAB_00407d4b;
  puStack_24 = (undefined1 *)0x407d43;
  FUN_0040394c();
  return;
}

