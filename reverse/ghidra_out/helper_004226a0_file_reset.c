// file_reset  addr=004226a0  func=FUN_004226a0


void FUN_004226a0(void)

{
  int in_EAX;
  HWND pHVar1;
  UINT UVar2;
  WPARAM WVar3;
  LPARAM LVar4;
  
  if (*(char *)(in_EAX + 0x202) == '\0') {
    LVar4 = 0;
    WVar3 = 0;
    UVar2 = 0x19f;
    pHVar1 = (HWND)FUN_0042e7ec();
    SendMessageA(pHVar1,UVar2,WVar3,LVar4);
    return;
  }
  LVar4 = 0;
  WVar3 = 0;
  UVar2 = 0x188;
  pHVar1 = (HWND)FUN_0042e7ec();
  SendMessageA(pHVar1,UVar2,WVar3,LVar4);
  return;
}

