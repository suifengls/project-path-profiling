#define read(x)         scanf("%d", &(x))
#define print(x)        printf("%d\n", (x))

int main()
{
    int n,i,j,a[10],s;
  
    read(n);
    for (i=0;i<n;i=i+1) {
       read(a[i]);
       s=0;
       for (j=0;j<=i;j=j+1) {
	 s=s+a[j];
       }
       if (n%2 ==0 ) {
          print(s);
       }
    }
    print(n);
}
