// getRezo @ 0045d710


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0045d710(float10 param_1)

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
  puStack_3c = &LAB_0045d850;
  uStack_40 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_40;
  if (_DAT_0046891c < param_1 / (float10)_DAT_0045d864 + (float10)in_AX) {
    puVar2 = &DAT_0046891c;
    puVar3 = &DAT_00468908;
    for (iVar1 = 5; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar2;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
    }
    DAT_004689b4 = DAT_004689b4 + 1;
    FUN_00454ffc(DAT_004689b4);
    fVar4 = (float10)FUN_00408890();
    FUN_00454ffc(DAT_004689b4);
    fVar5 = (float10)FUN_00408890();
    _DAT_0046891c = fVar4 + fVar5 / (float10)_DAT_0045d864;
    FUN_00454ffc(DAT_004689b4);
    _DAT_00468926 = FUN_00408890();
  }
  *unaff_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_0045d857;
  puStack_3c = (undefined1 *)0x45d84f;
  FUN_0040394c();
  return;
}

