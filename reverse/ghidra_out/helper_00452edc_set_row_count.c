// set_row_count  addr=00452edc  func=FUN_00452edc


void __fastcall FUN_00452edc(undefined4 param_1,int param_2)

{
  int in_EAX;
  
  if (param_2 != *(int *)(in_EAX + 0x22c)) {
    if (param_2 < 1) {
      param_2 = 1;
    }
    if (param_2 <= *(int *)(in_EAX + 0x21c)) {
      FUN_00452d80(param_1,param_2 + -1);
    }
    FUN_00450338(param_2,*(int *)(in_EAX + 0x1fc));
  }
  return;
}

