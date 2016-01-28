/* $Id: ex03.mc,v 2.1 2005/06/14 22:16:47 jls Exp $ */

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


void subr (int64_t A[], int64_t B[], int sz, int64_t *time, int mapnum) {

    OBM_BANK_A (AL, int64_t, MAX_OBM_SIZE)
    OBM_BANK_B (BL, int64_t, MAX_OBM_SIZE)

    int Hist0[512];
    int Hist1[512];
    int Hist2[512];
    int Hist3[512];

    int64_t tim0, tim1;
    int i, idx;

    Stream_64 SA,SB;

    read_timer (&tim0);

    for (i=0; i<512; i++) {
        Hist0[i] = 0;
        Hist1[i] = 0;
        Hist2[i] = 0;
        Hist3[i] = 0;
    }


#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SA, PORT_TO_STREAM, A, sz*sizeof(int64_t));
}
# pragma src section
{
    int i,idx,sel;
    int64_t i64;

    #pragma loop noloop_dep
    for (i=0; i<sz; i++) {
       sel = i & 3;
       get_stream_64 (&SA, &idx);

       if (sel == 0) Hist0[idx]++;
       if (sel == 1) Hist1[idx]++;
       if (sel == 2) Hist2[idx]++;
       if (sel == 3) Hist3[idx]++;
    }
}
}



#pragma src parallel sections
{
#pragma src section
{
    streamed_dma_cpu_64 (&SB, STREAM_TO_PORT, B, 512*sizeof(int64_t));
}
# pragma src section
{
    int i;
    int64_t i64;

    for (i=0; i<512; i++)  {
       i64 = Hist0[i] + Hist1[i] + Hist2[i] + Hist3[i];

       put_stream_64 (&SB, i64, 1);
    }
}
}

    read_timer (&tim1);

    *time = tim1 - tim0;

}
