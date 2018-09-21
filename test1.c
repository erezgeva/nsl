#include <stdio.h>
#include "nsl_statistics.h"
int main()
{
    struct nsl_stats s;
    nsl_stats_init(&s);
    printf("min %ld, max %ld\n",
            nsl_stats_get_min(&s),
            nsl_stats_get_max(&s));
    nsl_stats_add_elem(&s, 10);
    nsl_stats_add_elem(&s, 20);
    nsl_stats_add_elem(&s, 30);
#ifdef __STDC_VERSION__
    printf("count %lu, min %ld, max %ld, avg %Lf, sq %Lf, sd %Lf\n",
#else
    printf("count %lu, min %ld, max %ld, avg %lf, sq %lf, sd %lf\n",
#endif
            nsl_stats_get_num_elems(&s),
            nsl_stats_get_min(&s),
            nsl_stats_get_max(&s),
            nsl_stats_get_mean(&s),
            nsl_stats_get_sq(&s),
            nsl_stats_get_sd(&s));
    return 0;
}
