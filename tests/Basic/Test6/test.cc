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

	int x;
	int y;

	x = 45;

	if (x >=10)
	{
		if (x>=20)
		{
			if (x>= 30)
			{
				if (x >=40)  
				{
					if (x >= 50)  y=-1;
					else y = 4;
				}
				else y=3;
			}
			else y=2;
		}
		else y=1;
	}
	else y=0;

      output(y);
}