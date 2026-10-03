// FUN_0045f5e8 @ 0045f5e8  hits=['9999']


void FUN_0045f5e8(void)

{
  byte bVar1;
  int in_EAX;
  undefined1 *puVar2;
  WPARAM WVar3;
  undefined4 *puVar4;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined1 *puVar5;
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
  undefined4 uVar6;
  int iVar7;
  undefined2 extraout_var;
  int iVar8;
  uint uVar9;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_3c;
  undefined1 *puStack_38;
  char *pcStack_34;
  undefined4 uStack_30;
  undefined1 *puStack_2c;
  undefined1 *puStack_28;
  
  puStack_28 = &stack0xfffffffc;
  puStack_2c = &LAB_004601cb;
  uStack_30 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_30;
  pcStack_34 = (char *)0x45f61a;
  FUN_0045cbe4(0,1);
  iVar7 = *(int *)(in_EAX + 0x47c);
  *(undefined4 *)(iVar7 + 0x204) = 1;
  *(undefined4 *)(iVar7 + 0x208) = 1;
  pcStack_34 = (char *)0x45f64a;
  FUN_0045cbe4(extraout_ECX,1);
  iVar7 = *(int *)(in_EAX + 0x480);
  *(undefined4 *)(iVar7 + 0x204) = 1;
  *(undefined4 *)(iVar7 + 0x208) = 1;
  puStack_38 = &LAB_0045f692;
  uStack_3c = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_3c;
  pcStack_34 = &stack0xfffffffc;
  FUN_00408890();
  DAT_004689d0 = 0x2e;
  *unaff_FS_OFFSET = uStack_3c;
  pcStack_34 = (char *)0x45f6b7;
  (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0x40))();
  pcStack_34 = (char *)0x45f6d0;
  (**(code **)(**(int **)(*(int *)(in_EAX + 0x454) + 0x1ec) + 0x34))();
  pcStack_34 = (char *)0x45f6e3;
  FUN_004226dc(extraout_ECX_00,1);
  pcStack_34 = (char *)0x45f6f7;
  (**(code **)(**(int **)(*(int *)(in_EAX + 0x450) + 0x1ec) + 0x40))();
  pcStack_34 = (char *)0x45f710;
  (**(code **)(**(int **)(*(int *)(in_EAX + 0x450) + 0x1ec) + 0x34))();
  iVar7 = 0x100;
  puVar5 = PTR_DAT_004673bc + -0x30;
  do {
    iVar8 = 0x10;
    puVar2 = puVar5;
    do {
      *puVar2 = 0;
      puVar2[2] = 0;
      puVar2[1] = 0;
      puVar2 = puVar2 + 3;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    puVar5 = puVar5 + 0x30;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  iVar7 = *(int *)(in_EAX + 0x454);
  pcStack_34 = (char *)0x45f74c;
  FUN_004226a0();
  pcStack_34 = (char *)0x45f75c;
  (**(code **)(**(int **)(iVar7 + 0x1ec) + 0xc))();
  pcStack_34 = (char *)0x45f764;
  bVar1 = FUN_00407cdc(extraout_ECX_01);
  pcStack_34 = (char *)0x45f76e;
  FUN_0046027c(extraout_ECX_02,bVar1);
  pcStack_34 = (char *)0x45f77c;
  WVar3 = FUN_00422898();
  pcStack_34 = (char *)0x45f78c;
  FUN_004228c8(extraout_ECX_03,WVar3);
  pcStack_34 = (char *)0x45f79a;
  WVar3 = FUN_00422898();
  pcStack_34 = (char *)0x45f7aa;
  FUN_004228c8(extraout_ECX_04,WVar3);
  iVar7 = 0xfa1;
  puVar4 = (undefined4 *)PTR_DAT_00467274;
  do {
    *puVar4 = 0;
    puVar4[1] = 0;
    *(undefined2 *)(puVar4 + 2) = 0;
    puVar4 = (undefined4 *)((int)puVar4 + 10);
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  pcStack_34 = (char *)0x45f7d8;
  FUN_004226dc(extraout_ECX_05,1);
  pcStack_34 = (char *)CONCAT22(extraout_var,*(undefined2 *)(PTR_DAT_00467100 + 8));
  puStack_38 = *(undefined1 **)(PTR_DAT_00467100 + 4);
  uStack_3c = *(undefined4 *)PTR_DAT_00467100;
  FUN_00408860();
  pcStack_34 = (char *)0x45f800;
  FUN_00428aa0(extraout_ECX_06,(int *)0x0);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x460) + 0x22c);
  uVar6 = extraout_ECX_07;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45f82b;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45f842;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45f859;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_08;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x45f870;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x45f887;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x45f8a1;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x45f8bb;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x45f8d5;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45f8f2;
  FUN_0045506c(1,1);
  pcStack_34 = "100";
  puStack_38 = (undefined1 *)0x45f90f;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x45f929;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45f946;
  FUN_0045506c(2,1);
  pcStack_34 = "100";
  puStack_38 = (undefined1 *)0x45f963;
  FUN_0045506c(2,2);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x3ec) + 0x22c);
  uVar6 = extraout_ECX_09;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45f98e;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45f9a5;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45f9bc;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_10;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x45f9d3;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x45f9ea;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x45fa04;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x45fa1e;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x45fa38;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fa55;
  FUN_0045506c(1,1);
  pcStack_34 = "50";
  puStack_38 = (undefined1 *)0x45fa72;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x45fa8c;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45faa9;
  FUN_0045506c(2,1);
  pcStack_34 = "50";
  puStack_38 = (undefined1 *)0x45fac6;
  FUN_0045506c(2,2);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x400) + 0x22c);
  uVar6 = extraout_ECX_11;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45faf1;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fb08;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fb1f;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_12;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x45fb36;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x45fb4d;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x45fb67;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x45fb81;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x45fb9b;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fbb8;
  FUN_0045506c(1,1);
  pcStack_34 = "100";
  puStack_38 = (undefined1 *)0x45fbd5;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x45fbef;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fc0c;
  FUN_0045506c(2,1);
  pcStack_34 = "100";
  puStack_38 = (undefined1 *)0x45fc29;
  FUN_0045506c(2,2);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x3f4) + 0x22c);
  uVar6 = extraout_ECX_13;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fc54;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fc6b;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fc82;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_14;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x45fc99;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x45fcb0;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x45fcca;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x45fce4;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x45fcfe;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fd1b;
  FUN_0045506c(1,1);
  pcStack_34 = "100";
  puStack_38 = (undefined1 *)0x45fd38;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x45fd52;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fd6f;
  FUN_0045506c(2,1);
  pcStack_34 = "100";
  puStack_38 = (undefined1 *)0x45fd8c;
  FUN_0045506c(2,2);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x3f0) + 0x22c);
  uVar6 = extraout_ECX_15;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fdb7;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fdce;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45fde5;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_16;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x45fdfc;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x45fe13;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x45fe2d;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x45fe47;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x45fe61;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fe7e;
  FUN_0045506c(1,1);
  pcStack_34 = "15";
  puStack_38 = (undefined1 *)0x45fe9b;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x45feb5;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45fed2;
  FUN_0045506c(2,1);
  pcStack_34 = "15";
  puStack_38 = (undefined1 *)0x45feef;
  FUN_0045506c(2,2);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x3f8) + 0x22c);
  uVar6 = extraout_ECX_17;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45ff1a;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45ff31;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x45ff48;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_18;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x45ff5f;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x45ff76;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x45ff90;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x45ffaa;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x45ffc4;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x45ffe1;
  FUN_0045506c(1,1);
  pcStack_34 = "10";
  puStack_38 = (undefined1 *)0x45fffe;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x460018;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x460035;
  FUN_0045506c(2,1);
  pcStack_34 = "10";
  puStack_38 = (undefined1 *)0x460052;
  FUN_0045506c(2,2);
  iVar7 = *(int *)(*(int *)(in_EAX + 0x400) + 0x22c);
  uVar6 = extraout_ECX_19;
  if (-1 < iVar7 + -1) {
    uVar9 = 0;
    do {
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x46007d;
      FUN_0045506c(uVar9,0);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x460094;
      FUN_0045506c(uVar9,1);
      pcStack_34 = (char *)0x0;
      puStack_38 = (undefined1 *)0x4600ab;
      FUN_0045506c(uVar9,2);
      uVar9 = uVar9 + 1;
      iVar7 = iVar7 + -1;
      uVar6 = extraout_ECX_20;
    } while (iVar7 != 0);
  }
  pcStack_34 = (char *)0x4600c2;
  FUN_00452edc(uVar6,3);
  pcStack_34 = "Order";
  puStack_38 = (undefined1 *)0x4600d9;
  FUN_0045506c(0,0);
  pcStack_34 = "Offset%";
  puStack_38 = (undefined1 *)0x4600f3;
  FUN_0045506c(0,1);
  pcStack_34 = "Value%";
  puStack_38 = (undefined1 *)0x46010d;
  FUN_0045506c(0,2);
  pcStack_34 = "1";
  puStack_38 = (undefined1 *)0x460127;
  FUN_0045506c(1,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x460144;
  FUN_0045506c(1,1);
  pcStack_34 = "90";
  puStack_38 = (undefined1 *)0x460161;
  FUN_0045506c(1,2);
  pcStack_34 = "9999";
  puStack_38 = (undefined1 *)0x46017b;
  FUN_0045506c(2,0);
  pcStack_34 = "0";
  puStack_38 = (undefined1 *)0x460198;
  FUN_0045506c(2,1);
  pcStack_34 = "90";
  puStack_38 = (undefined1 *)0x4601b5;
  FUN_0045506c(2,2);
  *unaff_FS_OFFSET = uStack_30;
  puStack_28 = &LAB_004601d2;
  puStack_2c = (undefined1 *)0x4601ca;
  FUN_0040394c();
  return;
}

