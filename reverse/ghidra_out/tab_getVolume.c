// getVolume @ 0045d260


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0045d260(float10 param_1)

{
  ushort in_AX;
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *unaff_FS_OFFSET;
  float10 fVar4;
  float10 fVar5;
  undefined4 uStack_40;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  
  puStack_38 = &stack0xfffffffc;
  puStack_3c = &LAB_0045d3af;
  uStack_40 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_40;
  if (_DAT_004688cc < param_1 / (float10)_DAT_0045d3c4 + (float10)in_AX) {
    puVar2 = &DAT_004688a4;
    puVar3 = &DAT_00468890;
    for (iVar1 = 5; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar2;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
    }
    DAT_004689a8 = DAT_004689a8 + 1;
    FUN_00454ffc(0);
    fVar4 = (float10)FUN_00408890();
    FUN_00454ffc(1);
    fVar5 = (float10)FUN_00408890();
    _DAT_004688a4 = fVar4 + fVar5 / (float10)_DAT_0045d3c4;
    FUN_00454ffc(2);
    _DAT_004688ae = FUN_00408890();
  }
  *unaff_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_0045d3b6;
  puStack_3c = (undefined1 *)0x45d3ae;
  FUN_0040394c();
  return;
}

