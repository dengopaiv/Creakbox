// output_helper  addr=00464900  func=FUN_00464900


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1  [10] FUN_00464900(float10 param_1)

{
  undefined1 auVar1 [10];
  float10 fVar2;
  undefined4 local_e;
  undefined4 uStack_a;
  undefined2 uStack_6;
  
  if (param_1 <= _DAT_004649d8) {
    if (param_1 <= _DAT_004649e4) {
      param_1 = (float10)CONCAT28(0xbffe,0xcf5c28f5c28f5c29);
    }
  }
  else {
    param_1 = (float10)CONCAT28(0x3ffe,0xdeb851eb851eb852);
  }
  fVar2 = FUN_00464dd8(param_1,(float10)CONCAT28(0x3ff5,0x83126e978d4fdf3b));
  param_1._0_4_ = SUB104(fVar2,0);
  local_e = param_1._0_4_;
  param_1._4_4_ = (undefined4)((unkuint10)fVar2 >> 0x20);
  uStack_a = param_1._4_4_;
  param_1._8_2_ = (undefined2)((unkuint10)fVar2 >> 0x40);
  uStack_6 = param_1._8_2_;
  if ((float10)_DAT_004649f0 <= fVar2) {
    if ((float10)_DAT_004649f4 < fVar2) {
      local_e = 0;
      uStack_a = 0x80000000;
      uStack_6 = 0x3fff;
    }
  }
  else {
    local_e = 0;
    uStack_a = 0x80000000;
    uStack_6 = 0xbfff;
  }
  auVar1._4_4_ = uStack_a;
  auVar1._0_4_ = local_e;
  auVar1._8_2_ = uStack_6;
  return auVar1;
}

