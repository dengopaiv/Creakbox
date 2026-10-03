// getDecay @ 0045d9c4


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0045d9c4(float10 param_1)

{
  undefined1 *puVar1;
  ushort in_AX;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *unaff_FS_OFFSET;
  float10 fVar5;
  float10 fVar6;
  undefined4 uStack_40;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  
  puStack_38 = &stack0xfffffffc;
  puStack_3c = &LAB_0045db38;
  uStack_40 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_40;
  puVar1 = &stack0xfffffffc;
  if (_DAT_0046896c < param_1 / (float10)_DAT_0045db4c + (float10)in_AX) {
    puVar3 = &DAT_0046896c;
    puVar4 = &DAT_00468958;
    for (iVar2 = 5; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    }
    DAT_004689bc = DAT_004689bc + 1;
    FUN_00454ffc(DAT_004689bc);
    fVar5 = (float10)FUN_00408890();
    FUN_00454ffc(DAT_004689bc);
    fVar6 = (float10)FUN_00408890();
    _DAT_0046896c = fVar5 + fVar6 / (float10)_DAT_0045db4c;
    FUN_00454ffc(DAT_004689bc);
    _DAT_00468976 = FUN_00408890();
    puVar1 = puStack_38;
  }
  puStack_38 = puVar1;
  FUN_004028ac();
  *unaff_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_0045db3f;
  puStack_3c = (undefined1 *)0x45db37;
  FUN_0040394c();
  return;
}

