{
  y = 1;
  x=1;
  while (1) {
    y = y*2;
    if (y > 99999) break;
    if (y < 20) continue;
    x = y;
    while (x > 1) {
      print(x % 10);
      x = x / 10;
    }
    break;
  }
  print(y);
}
