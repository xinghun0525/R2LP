
#include <iostream>
#include <gperftools/tcmalloc.h>
 
int main()
{
    char *cp = (char *)tc_malloc(23 * sizeof(char));
    tc_free(cp);
    cp = NULL;
    return 0;
}