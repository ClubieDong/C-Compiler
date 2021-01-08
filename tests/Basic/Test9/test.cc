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

int Fibonacci(int x1)
{
     	int val;

	if (x1 == 1) val = 1;
 	if (x1 == 2) val = 1;
	if (x1 > 2) val = Fibonacci(x1-2)+Fibonacci(x1-1);
      	return val;
}

void main(void)
{
       	int x;
       	int y;
       	int result;

	 x = 5;
	 y = 2;
       	result = Fibonacci(x*y);
       	output(result);
}