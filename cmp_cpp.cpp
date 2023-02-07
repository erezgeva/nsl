/* SPDX-License-Identifier: GPL-3.0-or-later
   SPDX-FileCopyrightText: Copyright 2018 Erez Geva */

#include <stdio.h>
#include "nsl_statistics.h"
#define format "%ld"
#define uformat "%lu"
// C++ uses one format
#define avformat "%Lf"
// C++ uses one format
// C++ uses one format
// C++ uses one format
void print_vec(nsl_vec_stats &v, const char *msg)
{
    printf("%s: blow " uformat ", above " uformat ", "
           "value 0 " uformat ", value 10 " uformat ", "
           "value 20 " uformat ", value 30 " uformat ", "
           "value 40 " uformat ", value 50 " uformat ", "
           "value 60 " uformat "\n",
            msg,
            v.get_below(),
            v.get_above(),
            v.get_value(0),
            v.get_value(10),
            v.get_value(20),
            v.get_value(30),
            v.get_value(40),
            v.get_value(50),
            v.get_value(60));
}
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
    printf("count " uformat ", min " format ", max " format ", "
           "avg " avformat ", sq " avformat ", sd " avformat "\n",
            s.get_num_elems(),
            s.get_min(),
            s.get_max(),
            s.get_mean(),
            s.get_sq(),
            s.get_sd());
    printf("===============================================\n");
    nsl_vec_stats v(10,5,10);
    // Constructor does init!
    printf("start " format ", end " format ", "
           "step " uformat ", size " uformat "\n",
            v.get_start(),
            v.get_end(),
            v.get_step(),
            v.get_size());
                     print_vec(v, "bf");
    v.add_elem( 9);  print_vec(v, " 9");
    v.add_elem(10);  print_vec(v, "10");
    v.add_elem(19);  print_vec(v, "19");
    v.add_elem(20);  print_vec(v, "20");
    v.add_elem(29);  print_vec(v, "29");
    v.add_elem(30);  print_vec(v, "30");
    v.add_elem(39);  print_vec(v, "39");
    v.add_elem(40);  print_vec(v, "40");
    v.add_elem(49);  print_vec(v, "49");
    v.add_elem(50);  print_vec(v, "50");
    v.add_elem(59);  print_vec(v, "59");
    v.add_elem(60);  print_vec(v, "60");
    nsl_vec_stats v2 = v;
    // Copy during construction
    print_vec(v2, "v2");
    // Destructor does the remove of v!
    // Destructor does the remove of v2!
    return 0;
}
