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
	int array[5];
	int a;
	int b;
	int c;
	int d;
	int e;
	array[0]=0;
	array[1]=1;
	array[2]=2;
	array[3]=3;
	array[4]=4;


      	a 	= 10*array[0];			/* a = 0 */
	b 	= 5*array[1];			/* b = 5 */
	c 	= 2*array[2];			/* c = 4 */
	d 	= 20*array[3];			/* d = 60 */
	e	= 10*array[4]/array[2];		/* e = 20 */

	/* array[0] = (85-60-1)/(60/5)=24/12=2   
	*/
	array[0] = ((b+c*e-d-1)/((array[0]+(e*array[3]/(array[2]+array[3])))));

	/* 2*5 = 10			
	*/
	output(array[0]*b);							
}