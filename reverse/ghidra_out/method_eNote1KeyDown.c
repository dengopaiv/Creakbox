// eNote1KeyDown  entry=004605d0  containing=004605d0


void __fastcall FUN_004605d0(short *param_1)

{
  short sVar1;
  undefined *puVar2;
  undefined1 *puVar3;
  byte bVar4;
  int in_EAX;
  int iVar5;
  int iVar6;
  int *piVar7;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  undefined4 extraout_ECX_04;
  undefined4 extraout_ECX_05;
  undefined4 extraout_ECX_06;
  undefined4 extraout_ECX_07;
  undefined4 extraout_ECX_08;
  undefined4 extraout_ECX_09;
  undefined4 extraout_ECX_10;
  undefined4 extraout_ECX_11;
  undefined4 extraout_ECX_12;
  undefined4 extraout_ECX_13;
  undefined4 extraout_ECX_14;
  undefined4 extraout_ECX_15;
  undefined4 extraout_ECX_16;
  undefined4 extraout_ECX_17;
  undefined4 extraout_ECX_18;
  undefined4 extraout_ECX_19;
  undefined4 extraout_ECX_20;
  undefined4 extraout_ECX_21;
  undefined4 extraout_ECX_22;
  undefined4 extraout_ECX_23;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_2c;
  undefined1 *puStack_28;
  undefined1 *puStack_24;
  
  puStack_24 = &stack0xfffffffc;
  puStack_28 = &LAB_004609c3;
  uStack_2c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_2c;
  bVar4 = 0;
  sVar1 = *param_1;
  if (sVar1 == 0x26) {
    iVar5 = FUN_00402f3c(param_1,(int)PTR_PTR_0041ce9c);
    if (*(int *)(iVar5 + 0xc) == 1) {
      FUN_004123c8(extraout_ECX_16,"eNote16");
      piVar7 = (int *)FUN_00402f3c(extraout_ECX_17,(int)PTR_PTR_0041ce9c);
      (**(code **)(*piVar7 + 0xb4))();
    }
    else {
      FUN_00402f3c(extraout_ECX_16,(int)PTR_PTR_0041ce9c);
      FUN_00407cac();
      FUN_00403c14((undefined4 *)0x0,(undefined4 *)"eNote");
      FUN_004123c8(extraout_ECX_18,(char *)0x0);
      piVar7 = (int *)FUN_00402f3c(extraout_ECX_19,(int)PTR_PTR_0041ce9c);
      (**(code **)(*piVar7 + 0xb4))();
    }
  }
  else if (sVar1 == 0x28) {
    puStack_24 = &stack0xfffffffc;
    iVar5 = FUN_00402f3c(param_1,(int)PTR_PTR_0041ce9c);
    if (*(int *)(iVar5 + 0xc) == 0x10) {
      FUN_004123c8(extraout_ECX_12,"eNote1");
      piVar7 = (int *)FUN_00402f3c(extraout_ECX_13,(int)PTR_PTR_0041ce9c);
      (**(code **)(*piVar7 + 0xb4))();
    }
    else {
      FUN_00402f3c(extraout_ECX_12,(int)PTR_PTR_0041ce9c);
      FUN_00407cac();
      FUN_00403c14((undefined4 *)0x0,(undefined4 *)"eNote");
      FUN_004123c8(extraout_ECX_14,(char *)0x0);
      piVar7 = (int *)FUN_00402f3c(extraout_ECX_15,(int)PTR_PTR_0041ce9c);
      (**(code **)(*piVar7 + 0xb4))();
    }
  }
  else if (sVar1 == 0x2e) {
    puStack_24 = &stack0xfffffffc;
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    iVar5 = FUN_00407cdc(extraout_ECX);
    puVar2 = PTR_DAT_004673bc;
    iVar6 = FUN_00402f3c(extraout_ECX_00,(int)PTR_PTR_0041ce9c);
    puVar2[*(int *)(iVar6 + 0xc) * 3 + iVar5 * 0x30 + -0x33] = 0;
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    iVar5 = FUN_00407cdc(extraout_ECX_01);
    puVar2 = PTR_DAT_004673bc;
    FUN_00402f3c(extraout_ECX_02,(int)PTR_PTR_0041ce9c);
    puVar2[*(int *)(iVar6 + 0xc) * 3 + iVar5 * 0x30 + -0x32] = 0;
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    iVar5 = FUN_00407cdc(extraout_ECX_03);
    puVar2 = PTR_DAT_004673bc;
    FUN_00402f3c(extraout_ECX_04,(int)PTR_PTR_0041ce9c);
    puVar2[*(int *)(iVar6 + 0xc) * 3 + iVar5 * 0x30 + -0x31] = 0;
    FUN_004226a0();
    (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
    bVar4 = FUN_00407cdc(extraout_ECX_05);
    FUN_0046027c(extraout_ECX_06,bVar4);
    FUN_00402f3c(extraout_ECX_07,(int)PTR_PTR_0041ce9c);
    if (*(int *)(iVar6 + 0xc) == 0x10) {
      FUN_004123c8(extraout_ECX_08,"eNote1");
      piVar7 = (int *)FUN_00402f3c(extraout_ECX_09,(int)PTR_PTR_0041ce9c);
      (**(code **)(*piVar7 + 0xb4))();
    }
    else {
      FUN_00402f3c(extraout_ECX_08,(int)PTR_PTR_0041ce9c);
      FUN_00407cac();
      FUN_00403c14((undefined4 *)0x0,(undefined4 *)"eNote");
      FUN_004123c8(extraout_ECX_10,(char *)0x0);
      piVar7 = (int *)FUN_00402f3c(extraout_ECX_11,(int)PTR_PTR_0041ce9c);
      (**(code **)(*piVar7 + 0xb4))();
    }
  }
  else {
    do {
      bVar4 = bVar4 + 1;
      if (bVar4 == 0xc) break;
    } while (*(short *)(PTR_DAT_00467050 + (uint)bVar4 * 2 + -2) != *param_1);
    puStack_24 = &stack0xfffffffc;
    if (*(short *)(PTR_DAT_00467050 + (uint)bVar4 * 2 + -2) == *param_1) {
      puStack_24 = &stack0xfffffffc;
      FUN_004226a0();
      (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0xc))();
      iVar5 = FUN_00407cdc(extraout_ECX_20);
      puVar2 = PTR_DAT_004673bc;
      iVar6 = FUN_00402f3c(extraout_ECX_21,(int)PTR_PTR_0041ce9c);
      puVar2[*(int *)(iVar6 + 0xc) * 3 + iVar5 * 0x30 + -0x33] = *PTR_DAT_00467014 * '\f' + bVar4;
      FUN_00403b6c(PTR_DAT_00467294,
                   PTR_DAT_00467294 + ((uint)bVar4 + (uint)(byte)*PTR_DAT_00467014 * 0xc) * 4 + -4);
      FUN_00402f3c(extraout_ECX_22,(int)PTR_PTR_0041ce9c);
      FUN_00428aa0(extraout_ECX_23,(int *)0x0);
    }
    *param_1 = 0;
  }
  puVar3 = puStack_24;
  *unaff_FS_OFFSET = uStack_2c;
  puStack_24 = &LAB_004609ca;
  puStack_28 = (undefined1 *)0x4609c2;
  FUN_00403970(puVar3,2);
  return;
}

