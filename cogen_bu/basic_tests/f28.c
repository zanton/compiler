int f(int x, int y)
{
  int s;
  s = 0;
  if (y < 0) {
    x = -x;
    y = -y;
  }
  while (y != 0) {
    if (y % 2 == 1) s = s + x;
    x = x + x;
    y = y / 2;
  }
  return s;
}
