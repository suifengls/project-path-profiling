#define read(x)         scanf("%d", &(x))
#define print(x)        printf("%d\n", (x))

int main()
{
    int a[2], n, m, k, j, i, s0, s1, s2, s3, s4;

    read(a[0]);
    n = a[0];
    m = a[0];
    read(a[1]);
    k = a[1];
    j = 1;
    while ( n > 0 ) {
        s3 = k + a[j];
        j = j - 1;

        s1 = a[j] + m;
        if ( n % 2 == 0 ) {
            m = -m;
            s0 = a[j] + m;
        }
        else {
            s0 = a[j] + m;
            m = -m;
        }
        s2 = a[j] + m;
        n = n - 1;

        j = j + 1;
        s4 = k + a[j];
    }
    print(s0);
    print(s1);
    print(s2);
    print(s3);
    print(s4);
}
