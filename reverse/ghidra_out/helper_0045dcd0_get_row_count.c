// get_row_count  addr=0045dcd0  func=FUN_0045dcd0


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0045dcd0(void)

{
  int in_EAX;
  int iVar1;
  undefined4 *unaff_FS_OFFSET;
  float10 fVar2;
  float10 fVar3;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffffc;
  puStack_2c = &LAB_0045dd5b;
  uStack_30 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_30;
  iVar1 = 1;
  while( true ) {
    FUN_00454ffc(iVar1);
    fVar2 = (float10)FUN_00408890();
    FUN_00454ffc(iVar1);
    fVar3 = (float10)FUN_00408890();
    if ((float10)in_EAX < fVar2 + fVar3 / (float10)_DAT_0045dd6c) break;
    iVar1 = iVar1 + 1;
  }
  *unaff_FS_OFFSET = uStack_30;
  puStack_28 = &LAB_0045dd62;
  puStack_2c = (undefined1 *)0x45dd5a;
  FUN_0040394c();
  return;
}

