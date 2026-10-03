// getCutoff @ 0045d52c


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0045d52c(float10 param_1)

{
  undefined1 *puVar1;
  ushort in_AX;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined4 *unaff_FS_OFFSET;
  float10 fVar5;
  float10 fVar6;
  undefined1 auVar7 [10];
  float10 fVar8;
  undefined4 uStack_40;
  undefined1 *puStack_3c;
  undefined1 *puStack_38;
  
  puStack_38 = &stack0xfffffffc;
  puStack_3c = &LAB_0045d6da;
  uStack_40 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_40;
  fVar8 = param_1 / (float10)_DAT_0045d6ec + (float10)in_AX;
  puVar1 = &stack0xfffffffc;
  if (_DAT_004688f4 < fVar8) {
    puVar3 = &DAT_004688f4;
    puVar4 = &DAT_004688e0;
    for (iVar2 = 5; iVar2 != 0; iVar2 = iVar2 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    }
    DAT_004689b0 = DAT_004689b0 + 1;
    FUN_00454ffc(DAT_004689b0);
    fVar5 = (float10)FUN_00408890();
    FUN_00454ffc(DAT_004689b0);
    fVar6 = (float10)FUN_00408890();
    _DAT_004688f4 = fVar5 + fVar6 / (float10)_DAT_0045d6ec;
    FUN_00454ffc(DAT_004689b0);
    _DAT_004688fe = (float10)FUN_00408890();
    puVar1 = puStack_38;
  }
  puStack_38 = puVar1;
  auVar7 = FUN_0041befc(((_DAT_004688ea +
                         (_DAT_004688fe - _DAT_004688ea) *
                         ((fVar8 - _DAT_004688e0) / (_DAT_004688f4 - _DAT_004688e0))) /
                        (float10)_DAT_0045d6ec) * (float10)_DAT_0045d6f0,0,0x80000000,0x4000);
  FUN_0041befc(_DAT_0045d6f8 * ((float10)auVar7 - (float10)_DAT_0045d6f4),0,0x80000000,0x4000);
  *unaff_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_0045d6e1;
  puStack_3c = (undefined1 *)0x45d6d9;
  FUN_0040394c();
  return;
}

