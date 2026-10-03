// pow2_helper  addr=0041befc  func=FUN_0041befc


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1  [10]
FUN_0041befc(float10 param_1,undefined4 param_2,undefined4 param_3,undefined2 param_4)

{
  float10 fVar1;
  undefined1 local_e [10];
  
  if (param_1 == (float10)_DAT_0041bfb4) {
    local_e._8_2_ = 0x3fff;
    local_e._0_8_ = 0x8000000000000000;
  }
  else if (((float10)CONCAT28(param_4,CONCAT44(param_3,param_2)) != (float10)_DAT_0041bfb4) ||
          (param_1 <= (float10)_DAT_0041bfb4)) {
    fVar1 = FUN_004028bc();
    if ((fVar1 != (float10)_DAT_0041bfb4) || (_DAT_0041bfb8 < ABS(param_1))) {
      local_e = FUN_00402894();
    }
    else {
      FUN_004028e4();
      local_e = (undefined1  [10])FUN_0041bebc((float10)CONCAT28(param_4,CONCAT44(param_3,param_2)))
      ;
    }
  }
  else {
    local_e = (undefined1  [10])(unkuint10)0;
  }
  return local_e;
}

