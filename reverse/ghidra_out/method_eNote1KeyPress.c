// eNote1KeyPress  entry=00460a04  containing=00460a04


void __fastcall FUN_00460a04(undefined1 *param_1)

{
  undefined1 *puVar1;
  int iVar2;
  int *piVar3;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 *unaff_FS_OFFSET;
  undefined4 uStack_24;
  undefined1 *puStack_20;
  undefined1 *puStack_1c;
  
  puStack_1c = &stack0xfffffffc;
  puStack_20 = &LAB_00460ab9;
  uStack_24 = *unaff_FS_OFFSET;
  *unaff_FS_OFFSET = &uStack_24;
  *param_1 = 0;
  iVar2 = FUN_00402f3c(param_1,(int)PTR_PTR_0041ce9c);
  if (*(int *)(iVar2 + 0xc) == 0x10) {
    FUN_004123c8(extraout_ECX,"eNote1");
    piVar3 = (int *)FUN_00402f3c(extraout_ECX_00,(int)PTR_PTR_0041ce9c);
    (**(code **)(*piVar3 + 0xb4))();
  }
  else {
    FUN_00402f3c(extraout_ECX,(int)PTR_PTR_0041ce9c);
    FUN_00407cac();
    FUN_00403c14((undefined4 *)0x0,(undefined4 *)"eNote");
    FUN_004123c8(extraout_ECX_01,(char *)0x0);
    piVar3 = (int *)FUN_00402f3c(extraout_ECX_02,(int)PTR_PTR_0041ce9c);
    (**(code **)(*piVar3 + 0xb4))();
  }
  puVar1 = puStack_1c;
  *unaff_FS_OFFSET = uStack_24;
  puStack_1c = &LAB_00460ac0;
  puStack_20 = (undefined1 *)0x460ab8;
  FUN_00403970(puVar1,2);
  return;
}

