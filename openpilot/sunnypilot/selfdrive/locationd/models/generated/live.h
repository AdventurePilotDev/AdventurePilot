#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_951396063714138655);
void live_err_fun(double *nom_x, double *delta_x, double *out_8829230484853939956);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_8324831583329208104);
void live_H_mod_fun(double *state, double *out_6195164098102103666);
void live_f_fun(double *state, double dt, double *out_1121382142245591861);
void live_F_fun(double *state, double dt, double *out_913997275165172776);
void live_h_4(double *state, double *unused, double *out_7724482750046349026);
void live_H_4(double *state, double *unused, double *out_3399767373733084066);
void live_h_9(double *state, double *unused, double *out_5281184705868235879);
void live_H_9(double *state, double *unused, double *out_3887451561531363404);
void live_h_10(double *state, double *unused, double *out_5116080562554625324);
void live_H_10(double *state, double *unused, double *out_4308124524159739515);
void live_h_12(double *state, double *unused, double *out_5513916570484401719);
void live_H_12(double *state, double *unused, double *out_4267360939949366426);
void live_h_35(double *state, double *unused, double *out_1164628804221353665);
void live_H_35(double *state, double *unused, double *out_7012923972274380135);
void live_h_32(double *state, double *unused, double *out_4180954791390452400);
void live_H_32(double *state, double *unused, double *out_98059468580263779);
void live_h_13(double *state, double *unused, double *out_2893889737829027355);
void live_H_13(double *state, double *unused, double *out_828842334729858155);
void live_h_14(double *state, double *unused, double *out_5281184705868235879);
void live_H_14(double *state, double *unused, double *out_3887451561531363404);
void live_h_33(double *state, double *unused, double *out_2315310325260983070);
void live_H_33(double *state, double *unused, double *out_8283263096796313877);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}