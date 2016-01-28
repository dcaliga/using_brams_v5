static char const cvsid[] = "$Id: main.c,v 2.1 2005/06/14 22:16:47 jls Exp $";

/*
 * Copyright 2005 SRC Computers, Inc.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, Inc.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, Inc.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <libmap.h>

#define SIZE 65536
#define HSIZE 512

void subr (int64_t A[], int64_t B[], int sz, int64_t *tm, int mapnum);

int main () {
    FILE *res_cpu, *res_map;
    int i,j;
    int64_t *A, *B;
    int mapnum = 0;
    int m,ierr;
    int hist[HSIZE];
    int64_t tm;

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "couldn't open file 'res_map'\n");
	exit (1);
	}

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "couldn't open file 'res_cpu'\n");
	exit (1);
	}

    A = (int64_t*) Cache_Aligned_Allocate (8*SIZE * sizeof (int64_t));
    B = (int64_t*) Cache_Aligned_Allocate (HSIZE * sizeof (int64_t));

    srandom (55);

    for (i=0; i<HSIZE; i++)
        hist[i] = 0;

    for (i=0; i<SIZE; i++) {
        A[i] = random () & 0x1ff;
	hist[A[i]]++;
	}

    for (i=0; i<HSIZE; i++)
        fprintf (res_cpu, "%d\n", hist[i]);

    map_allocate (1);

    m = SIZE; 

    subr (A, B, m, &tm, mapnum);

    printf ("%lld clocks\n",tm);

    ierr = 0;
    for (i=0; i<HSIZE; i++) {
      if (hist[i] != B[i])  {
        ierr++;
        printf ("error i %i hist %d B %lld\n",i,hist[i],B[i]);
        }
      }

    if (ierr != 0)  {
      printf ("Results mismatch -  number errors %i\n",ierr);
      exit (1);
      }

      printf ("Results match\n");

    map_free (1);


    for (i=0; i<HSIZE; i++)
        fprintf (res_map, "%lld\n", B[i]);

    exit(0);
    }
