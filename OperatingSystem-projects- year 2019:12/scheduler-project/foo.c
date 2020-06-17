#include <stdio.h>
#include <unistd.h>

int main(int argc,char*argv[])
{
   char *ptr;
   int n = 0;
   n = strtol(argv[1],&ptr,10);
   int i = 0;
   int j = 0;
   for(i=1;i<=n;i++)
   {
       for(j=1;j<=n;j++)
       {
          printf("        ");
       }
       printf("%d\n",n);
       sleep(1);
   }
   printf("\n");
   return 0; 
}
