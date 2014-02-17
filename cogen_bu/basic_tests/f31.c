int f(int y, int x)
{
  if (x < 0) {
    if (y < 0) return g(-y, -x);
    else return -g(y, -x);
  } else {
    if (y < 0) return -g(-y, x);
    else return g(y, x);
  }
}

int g(int y, int x)
{
  if (y < x) return 0;
  else {
    int z;
    int q;
    int r;
    z = x;
    q = 1;
    r = y - x;
    while (r > z) {
      r = r - z;
      z = z + z;
      q = q + q;
    }
    return q + g(r, x);
  }
}

