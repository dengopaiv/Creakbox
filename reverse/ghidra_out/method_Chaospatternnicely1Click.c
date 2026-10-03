// Chaospatternnicely1Click  entry=004644a4  containing=004644a4


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_004644a4(void)

{
  byte bVar1;
  int in_EAX;
  uint uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  undefined *puVar5;
  undefined4 *unaff_FS_OFFSET;
  undefined1 auVar6 [10];
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  char local_6;
  char local_5;
  
  puStack_1c = &stack0xfffffffc;
  puStack_20 = &LAB_004646c8;
  uStack_24 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_24;
  FUN_00402838();
  local_5 = '\x10';
  puVar5 = PTR_DAT_004673bc;
  do {
    uVar2 = FUN_00402aac();
    local_6 = (char)uVar2;
    switch(uVar2 & 0xff) {
    case 0:
    case 7:
    case 8:
    case 9:
    case 10:
      uVar3 = FUN_00402aac();
      local_6 = ((char)uVar3 + '\x01') * '\f' + '\x01';
      break;
    case 1:
      uVar3 = FUN_00402aac();
      local_6 = ((char)uVar3 + '\x01') * '\f' + '\x04';
      break;
    case 2:
      uVar3 = FUN_00402aac();
      local_6 = ((char)uVar3 + '\x01') * '\f' + '\x05';
      break;
    case 3:
      uVar3 = FUN_00402aac();
      local_6 = ((char)uVar3 + '\x01') * '\f' + '\x06';
      break;
    case 4:
      uVar3 = FUN_00402aac();
      local_6 = ((char)uVar3 + '\x01') * '\f' + '\b';
      break;
    case 5:
      uVar3 = FUN_00402aac();
      local_6 = ((char)uVar3 + '\x01') * '\f' + '\v';
      break;
    case 6:
      local_6 = '\0';
    }
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    iVar4 = FUN_00407cdc(extraout_ECX);
    puVar5[iVar4 * 0x30 + -0x30] = local_6;
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    iVar4 = FUN_00407cdc(extraout_ECX_00);
    auVar6 = FUN_00402ac4();
    puVar5[iVar4 * 0x30 + -0x2f] = (float10)_DAT_004646d8 < (float10)auVar6;
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    iVar4 = FUN_00407cdc(extraout_ECX_01);
    auVar6 = FUN_00402ac4();
    puVar5[iVar4 * 0x30 + -0x2e] = (float10)_DAT_004646d8 < (float10)auVar6;
    local_5 = local_5 + -1;
    puVar5 = puVar5 + 3;
  } while (local_5 != '\0');
  iVar4 = *(int *)(in_EAX + 0x454);
  FUN_004226a0();
  (**(code **)(**(int **)(iVar4 + 0x1ec) + 0xc))();
  bVar1 = FUN_00407cdc(extraout_ECX_02);
  FUN_0046027c(extraout_ECX_03,bVar1);
  *unaff_FS_OFFSET = uStack_24;
  puStack_1c = &LAB_004646cf;
  puStack_20 = (undefined1 *)0x4646c7;
  FUN_0040394c();
  return;
}

