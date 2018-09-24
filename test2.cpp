#include <stdio.h>
#include "nsl_statistics.h"
#define format "%ld"
#define avformat "%Lf"
int main()
{
    nsl_stats s;
    // Constructor does init!
    printf("min " format ", max " format "\n",
            s.get_min(),
            s.get_max());
    s.add_elem(10);
    s.add_elem(20);
    s.add_elem(30);
    printf("count %lu, min " format ", max " format ", avg " avformat ", sq " avformat ", sd " avformat "\n",
            s.get_num_elems(),
            s.get_min(),
            s.get_max(),
            s.get_mean(),
            s.get_sq(),
            s.get_sd());
    return 0;
}
