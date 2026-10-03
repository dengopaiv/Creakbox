// file_close  addr=00405468  func=FUN_00405468


void FUN_00405468(void)

{
  undefined *in_EAX;
  int iVar1;
  
  if (*(int *)(in_EAX + 4) != 0xd7b1) {
    if (2 < *(int *)(in_EAX + 4) - 0xd7b1U) {
      if (in_EAX == &DAT_0046804c) {
        return;
      }
      goto LAB_0040548d;
    }
    iVar1 = (**(code **)(in_EAX + 0x1c))();
    if (iVar1 != 0) goto LAB_0040548d;
  }
  iVar1 = (**(code **)(in_EAX + 0x24))();
  if (iVar1 == 0) {
    return;
  }
LAB_0040548d:
  FUN_004027c4();
  return;
}

