#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BPoint_v5.h"

/* Compilation: 
   g++ -c BPoint_v5.cpp -I ../include 
   g++ -c BPoint_v5_test.cpp -I ../include
   g++ BPoint_v5_test.o BPoint_v5.o jpl_eph.o ElapsedTime.o -L ../lib -o BPoint_v5_test -lm -lsofa_c -lrt
*/
// Mercury transit
#define NOV_11_2019   1573430400.0
#define JAN_1_2017    1483228800.0
#define MAR_7_2017    1488844800.0  
#define MAR_28_2017   1490659200.0  
#define APR_6_2017    1491436800.0
#define JAN_1_1998    883612800.0
#define DEG_RAD (180.0 / M_PI)
#define CLASS_SITE_LAT_RAD  (-22.959748/180.0 * M_PI)
#define CLASS_SITE_LON_RAD  (-67.787260/180.0 * M_PI)
#define CLASS_SITE_HEIGHT_M (5186.0)
int main(int argc, char * argv[])
{
  //char eph_file[120] = "jpleph_1950_2050.440";
  char eph_file[120] = "DEc405";
  double dx[72], dy[72], det_pol[72];
  int i;
  int nsamples = 696, ndet = 1;
  double time_delta = 3600.0;
  //double dut1 = 0.589354;
  //double dut1 = 0.590715;
  // January 1, 2017
  /* JPL Horizons eop.190621.p190912
  double dut1_start = 0.589930;
  double dut1_stop  = 0.588778;
  double xp_start = 0.07669;
  double xp_stop  = 0.07648;
  double yp_start = 0.25947;
  double yp_stop  = 0.25993;
  double dpsi_start = -98.23; // mas
  double dpsi_stop  = -98.36;
  double deps_start = -9.06;  // mas
  double deps_stop  = -9.02;
  */

  // April 6, 2017 0.009587;0.000015;0.384548;0.000019;final;0.4634347  0.010352;0.000016;0.386538;0.000016;final;0.4619147
  /* Skyfield 
  double dut1_start = 0.4634347;
  double dut1_stop  = 0.4619147;
  double xp_start = 0.009587;
  double xp_stop  = 0.010352;
  double yp_start = 0.384548;
  double yp_stop  = 0.386538;
  */
  // Astropy
  double dut1_start = 0.4634343;
  double dut1_stop  = 0.4619097;
  double xp_start = 0.0095870;
  double xp_stop  = 0.0104550;
  double yp_start = 0.3846030;
  double yp_stop  = 0.3865750;
  //
  BPoint bp(eph_file, CLASS_SITE_LON_RAD, CLASS_SITE_LAT_RAD, CLASS_SITE_HEIGHT_M, CLK_UTC, true, true);

  double * ct       = (double*)malloc( sizeof(double) * nsamples );
  double * az_ac    = (double*)malloc( sizeof(double) * nsamples );
  double * alt_ac   = (double*)malloc( sizeof(double) * nsamples );
  double * rot_ac   = (double*)malloc( sizeof(double) * nsamples );
  double * fp_dist  = (double*)malloc( sizeof(double) * nsamples * ndet );
  double * fp_bear  = (double*)malloc( sizeof(double) * nsamples * ndet );
  double * fp_rot   = (double*)malloc( sizeof(double) * nsamples * ndet );

  ct[0] = MAR_28_2017;
  az_ac[0] = 0.0;
  alt_ac[0] = M_PI / 4.0;
  rot_ac[0] = -M_PI / 4.0;

  for(i = 1; i < nsamples; i++) {
    ct[i] = ct[0] + i * time_delta;
    az_ac[i] = az_ac[0] + (double) i / DEG_RAD;
    alt_ac[i] = alt_ac[0];
    rot_ac[i] = rot_ac[0];
  }
  for(i = 0; i < ndet; i++) {
    dx[i] = (2.0 * (i % 9) - 8.0) / DEG_RAD;
    dy[i] = (2.0 * (i / 9) - 7.0) / DEG_RAD;
    dx[i] = 0.0;
    dy[i] = 0.0;
    det_pol[i] = 45.0 / DEG_RAD;
  }
 
  bp.set_earth_orientation(dut1_start, dut1_stop, xp_start, xp_stop, yp_start, yp_stop);
  //bp.set_earth_orientation(dut1_start, dut1_stop);
  bp.recenter_focal_plane_to_source(GeoCMoon, nsamples, ct, az_ac, alt_ac, rot_ac,
                                           ndet, dx, dy, det_pol,
                                           fp_dist, fp_bear, fp_rot);
   
 // for(i = 0; i < nsamples; i++) {
 //   printf(" %lf %lf %lf %lf \n", az_ac[i] * DEG_RAD, fp_dist[i] * DEG_RAD, fp_bear[i] * DEG_RAD, fp_rot[i] * DEG_RAD);
 // }
  return 0;
}








