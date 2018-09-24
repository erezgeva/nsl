#include <stdio.h>
#include "nsl_statistics.h"
#define format "%ld"
#ifdef __STDC_VERSION__
#define avformat "%Lf"
#else
#define avformat "%lf"
#endif
int main()
{
    struct nsl_stats s;
    nsl_stats_init(&s);
    printf("min " format ", max " format "\n",
            nsl_stats_get_min(&s),
            nsl_stats_get_max(&s));
    nsl_stats_add_elem(&s, 10);
    nsl_stats_add_elem(&s, 20);
    nsl_stats_add_elem(&s, 30);
    printf("count %lu, min " format ", max " format ", avg " avformat ", sq " avformat ", sd " avformat "\n",
            nsl_stats_get_num_elems(&s),
            nsl_stats_get_min(&s),
            nsl_stats_get_max(&s),
            nsl_stats_get_mean(&s),
            nsl_stats_get_sq(&s),
            nsl_stats_get_sd(&s));
    return 0;
}
