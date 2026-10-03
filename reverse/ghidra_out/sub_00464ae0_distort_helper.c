// distort_helper  addr=00464ae0  func=FUN_00464ae0


float10 FUN_00464ae0(float10 param_1,float10 param_2)

{
  float10 *in_EAX;
  
  *in_EAX = *in_EAX + param_2 * (param_1 - *in_EAX);
  return *in_EAX;
}

