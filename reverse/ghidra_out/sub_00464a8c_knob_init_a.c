// knob_init_a  addr=00464a8c  func=FUN_00464a8c


void FUN_00464a8c(void)

{
  undefined4 *in_EAX;
  
  *in_EAX = 0;
  in_EAX[1] = 0;
  *(undefined2 *)(in_EAX + 2) = 0;
  *(undefined4 *)((int)in_EAX + 10) = 0;
  *(undefined4 *)((int)in_EAX + 0xe) = 0;
  *(undefined2 *)((int)in_EAX + 0x12) = 0;
  in_EAX[5] = 0;
  in_EAX[6] = 0;
  *(undefined2 *)(in_EAX + 7) = 0;
  *(undefined4 *)((int)in_EAX + 0x1e) = 0;
  *(undefined4 *)((int)in_EAX + 0x22) = 0;
  *(undefined2 *)((int)in_EAX + 0x26) = 0;
  in_EAX[10] = 0;
  in_EAX[0xb] = 0;
  *(undefined2 *)(in_EAX + 0xc) = 0;
  *(undefined4 *)((int)in_EAX + 0x32) = 0;
  *(undefined4 *)((int)in_EAX + 0x36) = 0;
  *(undefined2 *)((int)in_EAX + 0x3a) = 0;
  return;
}

