#include <stdio.h>
#include "nsl_statistics.h"
#define format "%ld"
#define uformat "%lu"
#ifdef __STDC_VERSION__
#define avformat "%Lf"
#else
#define avformat "%lf"
#endif
void print_vec(struct nsl_vec_stats *v, const char *msg)
{
    printf("%s: blow " uformat ", above " uformat ", "
           "value 0 " uformat ", value 10 " uformat ", "
           "value 20 " uformat ", value 30 " uformat ", "
           "value 40 " uformat ", value 50 " uformat ", "
           "value 60 " uformat "\n",
            msg,
            nsl_vec_stats_get_below(v),
            nsl_vec_stats_get_above(v),
            nsl_vec_stats_get_value(v, 0),
            nsl_vec_stats_get_value(v, 10),
            nsl_vec_stats_get_value(v, 20),
            nsl_vec_stats_get_value(v, 30),
            nsl_vec_stats_get_value(v, 40),
            nsl_vec_stats_get_value(v, 50),
            nsl_vec_stats_get_value(v, 60));
}
int main()
{
    struct nsl_stats s0, *s = &s0;
    nsl_stats_init(s);
    printf("min " format ", max " format "\n",
            nsl_stats_get_min(s),
            nsl_stats_get_max(s));
    nsl_stats_add_elem(s, 10);
    nsl_stats_add_elem(s, 20);
    nsl_stats_add_elem(s, 30);
    printf("count " uformat ", min " format ", max " format ", "
           "avg " avformat ", sq " avformat ", sd " avformat "\n",
            nsl_stats_get_num_elems(s),
            nsl_stats_get_min(s),
            nsl_stats_get_max(s),
            nsl_stats_get_mean(s),
            nsl_stats_get_sq(s),
            nsl_stats_get_sd(s));
    printf("===============================================\n");
    struct nsl_vec_stats v0, *v = &v0;
    nsl_vec_stats_init(v, 10,5,10);
    printf("start " format ", end " format ", "
           "step " uformat ", size " uformat "\n",
            nsl_vec_stats_get_start(v),
            nsl_vec_stats_get_end(v),
            nsl_vec_stats_get_step(v),
            nsl_vec_stats_get_size(v));
                                   print_vec(v, "bf");
    nsl_vec_stats_add_elem(v, 9);  print_vec(v, " 9");
    nsl_vec_stats_add_elem(v,10);  print_vec(v, "10");
    nsl_vec_stats_add_elem(v,19);  print_vec(v, "19");
    nsl_vec_stats_add_elem(v,20);  print_vec(v, "20");
    nsl_vec_stats_add_elem(v,29);  print_vec(v, "29");
    nsl_vec_stats_add_elem(v,30);  print_vec(v, "30");
    nsl_vec_stats_add_elem(v,39);  print_vec(v, "39");
    nsl_vec_stats_add_elem(v,40);  print_vec(v, "40");
    nsl_vec_stats_add_elem(v,49);  print_vec(v, "49");
    nsl_vec_stats_add_elem(v,50);  print_vec(v, "50");
    nsl_vec_stats_add_elem(v,59);  print_vec(v, "59");
    nsl_vec_stats_add_elem(v,60);  print_vec(v, "60");
    struct nsl_vec_stats v02, *v2 = &v02;
	nsl_vec_stats_copy(v2, v);
    print_vec(v2, "v2");
    nsl_vec_stats_remove(v);
    nsl_vec_stats_remove(v2);
    return 0;
}
