// cell_reader  addr=00454ffc  func=FUN_00454ffc


void __fastcall FUN_00454ffc(undefined4 param_1)

{
  int *piVar1;
  
  piVar1 = (int *)FUN_00454274(param_1,param_1);
  if (piVar1 == (int *)0x0) {
    FUN_0040394c();
  }
  else {
    (**(code **)(*piVar1 + 0xc))();
  }
  return;
}

