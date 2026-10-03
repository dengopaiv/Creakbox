// getTune @ 0045d3d4


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_0045d3d4(float10 param_1)

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
  puStack_3c = &LAB_0045d514;
  uStack_40 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_40;
  if (_DAT_004688cc < param_1 / (float10)_DAT_0045d528 + (float10)in_AX) {
    puVar2 = &DAT_004688cc;
    puVar3 = &DAT_004688b8;
    for (iVar1 = 5; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar3 = *puVar2;
      puVar2 = puVar2 + 1;
      puVar3 = puVar3 + 1;
    }
    DAT_004689ac = DAT_004689ac + 1;
    FUN_00454ffc(0);
    fVar4 = (float10)FUN_00408890();
    FUN_00454ffc(1);
    fVar5 = (float10)FUN_00408890();
    _DAT_004688cc = fVar4 + fVar5 / (float10)_DAT_0045d528;
    FUN_00454ffc(2);
    _DAT_004688d6 = FUN_00408890();
  }
  *unaff_FS_OFFSET = uStack_40;
  puStack_38 = &LAB_0045d51b;
  puStack_3c = (undefined1 *)0x45d513;
  FUN_0040394c();
  return;
}

