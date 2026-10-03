// cell_writer  addr=0045506c  func=FUN_0045506c


void __fastcall FUN_0045506c(uint param_1,uint param_2)

{
  int *piVar1;
  undefined4 extraout_ECX;
  
  piVar1 = (int *)FUN_00454fa8(param_1,param_1);
  (**(code **)(*piVar1 + 0x20))();
  FUN_00454f44(CONCAT31((int3)((uint)extraout_ECX >> 8),1),param_2);
  FUN_00454f44(0,param_1);
  FUN_00454ef8(param_1,param_2);
  return;
}

