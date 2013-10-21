#define read(x)         scanf("%d", &(x))
#define print(x)        printf("%d\n", (x))

int main()
{
        int a,b;
        a = 0;
	b = 1;
	while (a == 0)
        {
                if(b == 0)
                {
                        print(b);
                }
                else
                {
                        read(b);
                }
                while(b == 0)
                {
                        read(b);
                }
        }
}