// getnextnote_helper  addr=0045f524  func=FUN_0045f524


void __fastcall FUN_0045f524(int param_1)

{
  undefined1 *puVar1;
  int in_EAX;
  undefined4 extraout_ECX;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  
  puStack_24 = &stack0xfffffffc;
  puStack_28 = &LAB_0045f5d7;
  uStack_2c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_2c;
  puVar1 = &stack0xfffffffc;
  if (param_1 == 0x10) {
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0x14))();
    puVar1 = puStack_24;
  }
  puStack_24 = puVar1;
  (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
  FUN_00407cdc(extraout_ECX);
  *unaff_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_0045f5de;
  puStack_28 = (undefined1 *)0x45f5d6;
  FUN_0040394c();
  return;
}

