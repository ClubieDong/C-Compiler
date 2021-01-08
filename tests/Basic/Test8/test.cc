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

int div(int x)
{
     int factor;

     factor = 10;
     return (x/factor);
}

int rem(int x, int y)
{
   int factor;

   factor =10;
   return (y - (factor*x));
}

void main(void)
{

	int x;
	int dx;
      	int  rx;
 
	x = input();

      	while (x > 0)
     	{
         	dx = div(x);
	   	rx = rem(dx,x);
          	output(rx);
	   	x = dx;
      	}
}