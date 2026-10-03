// file_write  addr=00403f00  func=FUN_00403f00


void __fastcall FUN_00403f00(undefined4 param_1,undefined1 *param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = 0;
  if (param_2 != (undefined1 *)0x0) {
    iVar1 = *(int *)(param_2 + -4);
    iVar2 = -iVar1;
  }
  FUN_004057fb(iVar1,iVar2);
  FUN_00405790(iVar1,param_2);
  return;
}

