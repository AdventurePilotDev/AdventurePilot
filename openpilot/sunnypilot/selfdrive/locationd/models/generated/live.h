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
void live_H(double *in_vec, double *out_8422731807747617126);
void live_err_fun(double *nom_x, double *delta_x, double *out_8551704449810585042);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_3175550883655878919);
void live_H_mod_fun(double *state, double *out_3287096923442814197);
void live_f_fun(double *state, double dt, double *out_5242857574304425625);
void live_F_fun(double *state, double dt, double *out_8328358279033599164);
void live_h_4(double *state, double *unused, double *out_91633994369457774);
void live_H_4(double *state, double *unused, double *out_3957674883482680467);
void live_h_9(double *state, double *unused, double *out_4208581920388370897);
void live_H_9(double *state, double *unused, double *out_3716485236853089822);
void live_h_10(double *state, double *unused, double *out_4115656655484875700);
void live_H_10(double *state, double *unused, double *out_7843772193576287672);
void live_h_12(double *state, double *unused, double *out_6624029793730617388);
void live_H_12(double *state, double *unused, double *out_1061781524549281328);
void live_h_35(double *state, double *unused, double *out_4077798688530584961);
void live_H_35(double *state, double *unused, double *out_3807344556874295037);
void live_h_32(double *state, double *unused, double *out_7868626748682051702);
void live_H_32(double *state, double *unused, double *out_7505877329804189447);
void live_h_13(double *state, double *unused, double *out_1420702848786480918);
void live_H_13(double *state, double *unused, double *out_172727235390413316);
void live_h_14(double *state, double *unused, double *out_4208581920388370897);
void live_H_14(double *state, double *unused, double *out_3716485236853089822);
void live_h_33(double *state, double *unused, double *out_7709707964137789182);
void live_H_33(double *state, double *unused, double *out_2559544178528784513);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}