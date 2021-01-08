int getchar();
int putchar(int ch);

void output(int n)
{
    if (n < 0)
    {
        putchar('-');
        n = -n;
    }
    else if (n == 0)
    {
        putchar('0');
        return;
    }
    int num[20], idx = 0;
    while (n != 0)
    {
        num[idx] = n % 10;
        n /= 10;
        ++idx;
    }
    for (--idx; idx >= 0; --idx)
        putchar(num[idx] + '0');
}

int input()
{
    int n = 0, t = getchar();
    while ('0' <= t & t <= '9')
    {
        n = n * 10 + t - '0';
        t = getchar();
    }
    return n;
}

void main(void)
{
   int i;
   int j;
   int k;

   i=5;
   j=i+j;
   k=i+j+k;

   output(i);
   output(j);
   output(k);
   
   if (i==i)
   {
      int i;
      int j;
      int k;
      i=10;
      j=i+j;
      k=i+j+k;
      output(i);
      output(j);
      output(k);

      if (j < k)
      {
         int j;
         int k;

         j = i*5;
         k = i*10;
         

         output(i);
         output(j);
         output(k);
      }

      output(i);
      output(j);
      output(k);
   }

   output(i);
   output(j);
   output(k);
}