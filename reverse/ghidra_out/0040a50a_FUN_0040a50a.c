// FUN_0040a50a @ 0040a50a
// hits: ['immediate_9999']


int __thiscall FUN_0040a50a(char param_1,undefined4 param_2,int param_3,byte param_4)

{
  undefined1 *in_EAX;
  int iVar1;
  uint uVar2;
  undefined *puVar3;
  undefined1 *puVar4;
  ushort local_20;
  
  iVar1 = 0x13;
  if (param_1 == '\0') {
    iVar1 = param_3;
    if (param_3 < 2) {
      iVar1 = 2;
    }
    if (0x12 < iVar1) {
      iVar1 = 0x12;
    }
  }
  FUN_0040aaa6(param_1);
  puVar4 = in_EAX;
  if (local_20 - 0x7fff < 2) {
    puVar3 = &DAT_0040a504 + (local_20 - 0x7fff) * 3;
    for (iVar1 = 3; iVar1 != 0; iVar1 = iVar1 + -1) {
      *puVar4 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    }
  }
  else {
    uVar2 = (uint)param_4;
    if ((param_4 != 1) && ((4 < param_4 || (iVar1 < (short)local_20)))) {
      uVar2 = 0;
    }
    (*(code *)(&PTR_LAB_0040a5a4)[uVar2])();
  }
  return (int)puVar4 - (int)in_EAX;
}

