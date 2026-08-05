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
void live_H(double *in_vec, double *out_3938713143761766294);
void live_err_fun(double *nom_x, double *delta_x, double *out_3894184708321022357);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_1332062958029401683);
void live_H_mod_fun(double *state, double *out_6122699571068813308);
void live_f_fun(double *state, double dt, double *out_8096880098342322815);
void live_F_fun(double *state, double dt, double *out_1644005606495800393);
void live_h_4(double *state, double *unused, double *out_413086985817326879);
void live_H_4(double *state, double *unused, double *out_6429417666409318485);
void live_h_9(double *state, double *unused, double *out_6244307786541213249);
void live_H_9(double *state, double *unused, double *out_6188228019779727840);
void live_h_10(double *state, double *unused, double *out_3212854074053480410);
void live_H_10(double *state, double *unused, double *out_4321750945075924847);
void live_h_12(double *state, double *unused, double *out_7506086505450537905);
void live_H_12(double *state, double *unused, double *out_5808318641361724818);
void live_h_35(double *state, double *unused, double *out_9169327670930640881);
void live_H_35(double *state, double *unused, double *out_3062755609036711109);
void live_h_32(double *state, double *unused, double *out_5526417165688159534);
void live_H_32(double *state, double *unused, double *out_2738548763170901449);
void live_h_13(double *state, double *unused, double *out_7463786144949193313);
void live_H_13(double *state, double *unused, double *out_6671507766255712849);
void live_h_14(double *state, double *unused, double *out_6244307786541213249);
void live_H_14(double *state, double *unused, double *out_6188228019779727840);
void live_h_33(double *state, double *unused, double *out_8526957708367770484);
void live_H_33(double *state, double *unused, double *out_87801395602146495);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}