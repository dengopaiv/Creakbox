// lowpass_helper  addr=00464b18  func=FUN_00464b18


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 FUN_00464b18(float10 param_1,float10 param_2,float10 param_3,undefined4 param_4,
                    undefined4 param_5,undefined2 param_6)

{
  float10 *in_EAX;
  float10 fVar1;
  float10 fVar2;
  
  if ((float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) < _DAT_00464d48) {
    param_4 = 0x70a3d70a;
    param_5 = 0xa3d70a3d;
    param_6 = 0x3ff8;
  }
  if (_DAT_00464d54 < (float10)CONCAT28(param_6,CONCAT44(param_5,param_4))) {
    param_4 = 0x33333333;
    param_5 = 0xb3333333;
    param_6 = 0x3ffe;
  }
  if (param_3 < (float10)_DAT_00464d60) {
    param_3 = (float10)(unkuint10)0;
  }
  if ((float10)_DAT_00464d64 < param_3) {
    param_3 = (float10)CONCAT28(0x3fff,0x8000000000000000);
  }
  fVar2 = (float10)_DAT_00464d68;
  FUN_004028ac();
  FUN_004028ac();
  FUN_004028ac();
  fVar1 = FUN_004028ac();
  param_3 = _DAT_00464da8 *
            (param_3 * (_DAT_00464d9c - _DAT_00464d90 * (param_1 / fVar2)) +
            (_DAT_00464d84 + _DAT_00464d78 * (param_1 / fVar2)) * fVar1);
  if ((float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) < _DAT_00464db4) {
    param_3 = param_3 * (_DAT_00464dcc +
                        _DAT_00464dc0 *
                        ((float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) / _DAT_00464db4));
  }
  fVar2 = (float10)_DAT_00464d64 - param_3;
  *(float10 *)((int)in_EAX + 0x14) = (param_2 - fVar2 * *(float10 *)((int)in_EAX + 10)) - *in_EAX;
  *(float10 *)((int)in_EAX + 10) =
       *(float10 *)((int)in_EAX + 10) +
       (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) * *(float10 *)((int)in_EAX + 0x14);
  *in_EAX = *in_EAX + (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) *
                      *(float10 *)((int)in_EAX + 10);
  *(float10 *)((int)in_EAX + 0x14) = fVar2 * -*(float10 *)((int)in_EAX + 10);
  *(float10 *)((int)in_EAX + 10) =
       *(float10 *)((int)in_EAX + 10) +
       (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) * *(float10 *)((int)in_EAX + 0x14);
  *in_EAX = *in_EAX + (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) *
                      *(float10 *)((int)in_EAX + 10);
  *(float10 *)((int)in_EAX + 0x14) = fVar2 * -*(float10 *)((int)in_EAX + 10);
  *(float10 *)((int)in_EAX + 10) =
       *(float10 *)((int)in_EAX + 10) +
       (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) * *(float10 *)((int)in_EAX + 0x14);
  *in_EAX = *in_EAX + (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) *
                      *(float10 *)((int)in_EAX + 10);
  *(float10 *)((int)in_EAX + 0x14) = fVar2 * -*(float10 *)((int)in_EAX + 10);
  *(float10 *)((int)in_EAX + 10) =
       *(float10 *)((int)in_EAX + 10) +
       (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) * *(float10 *)((int)in_EAX + 0x14);
  *in_EAX = *in_EAX + (float10)CONCAT28(param_6,CONCAT44(param_5,param_4)) *
                      *(float10 *)((int)in_EAX + 10);
  return *in_EAX;
}

