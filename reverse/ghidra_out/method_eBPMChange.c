// eBPMChange  entry=0046366c  containing=0046366c


void FUN_0046366c(void)

{
  int iVar1;
  undefined4 extraout_ECX;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  undefined4 local_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  
  puStack_1c = &stack0xfffffffc;
  puStack_28 = &stack0xfffffffc;
  puStack_20 = &LAB_004636fc;
  local_24 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &local_24;
  puStack_2c = &LAB_004636c6;
  uStack_30 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_30;
  FUN_00428a70();
  iVar1 = FUN_00407cdc(extraout_ECX);
  *(float10 *)PTR_DAT_00467100 = (float10)iVar1;
  *unaff_FS_OFFSET = uStack_30;
  *unaff_FS_OFFSET = local_24;
  puStack_1c = &LAB_00463703;
  puStack_20 = (undefined1 *)0x4636fb;
  FUN_0040394c();
  return;
}

