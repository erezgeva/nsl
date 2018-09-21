#include <stdio.h>
#include "nsl_statistics.h"
int main()
{
    nsl_stats s;
    // Constructor does init!
    printf("min %ld, max %ld\n",
            s.get_min(),
            s.get_max());
    s.add_elem(10);
    s.add_elem(20);
    s.add_elem(30);
    printf("count %lu, min %ld, max %ld, avg %Lf, sq %Lf, sd %Lf\n",
            s.get_num_elems(),
            s.get_min(),
            s.get_max(),
            s.get_mean(),
            s.get_sq(),
            s.get_sd());
    return 0;
}
