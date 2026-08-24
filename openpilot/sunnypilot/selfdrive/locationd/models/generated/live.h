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
void live_H(double *in_vec, double *out_5919417919395135162);
void live_err_fun(double *nom_x, double *delta_x, double *out_7862934056957424443);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_8101266463654214362);
void live_H_mod_fun(double *state, double *out_8187575145081660141);
void live_f_fun(double *state, double dt, double *out_67096697228203721);
void live_F_fun(double *state, double dt, double *out_5257991356069090461);
void live_h_4(double *state, double *unused, double *out_7619275869505448533);
void live_H_4(double *state, double *unused, double *out_4435828402076443789);
void live_h_9(double *state, double *unused, double *out_1601795889458588551);
void live_H_9(double *state, double *unused, double *out_2851390533188003681);
void live_h_10(double *state, double *unused, double *out_4865898079779618969);
void live_H_10(double *state, double *unused, double *out_8224750357284157239);
void live_h_12(double *state, double *unused, double *out_1391970625673320673);
void live_H_12(double *state, double *unused, double *out_7629657294590374831);
void live_h_35(double *state, double *unused, double *out_2152890684532998694);
void live_H_35(double *state, double *unused, double *out_3329191038280531715);
void live_h_32(double *state, double *unused, double *out_1889522006605197955);
void live_H_32(double *state, double *unused, double *out_442709615432077219);
void live_h_13(double *state, double *unused, double *out_6048290794821993026);
void live_H_13(double *state, double *unused, double *out_4224874538215542216);
void live_h_14(double *state, double *unused, double *out_1601795889458588551);
void live_H_14(double *state, double *unused, double *out_2851390533188003681);
void live_h_33(double *state, double *unused, double *out_5879039935858706044);
void live_H_33(double *state, double *unused, double *out_9127419948569878016);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}