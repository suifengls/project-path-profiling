#define read(x)		scanf("%d", &(x))
#define print(x)	printf("%d\n", (x))

int main()
{
    int n, m, k, s0, s1;

    read(n);
    m = n;
    while ( n > 0 ) {
        s1 = m + m;
        if ( n % 2 == 0 ) {
            s0 = m + m;
            m = -m;
        }
        else {
            s0 = m + m;
        }
        n = n - 1;
    }
    print(s0);
    print(s1);
}
