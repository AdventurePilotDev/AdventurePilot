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
void live_H(double *in_vec, double *out_8802156707427731613);
void live_err_fun(double *nom_x, double *delta_x, double *out_7467085972428811361);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_2939879702230300529);
void live_H_mod_fun(double *state, double *out_6164990158744388196);
void live_f_fun(double *state, double dt, double *out_5733564182234174065);
void live_F_fun(double *state, double dt, double *out_7393539254045095895);
void live_h_4(double *state, double *unused, double *out_4874839313433740512);
void live_H_4(double *state, double *unused, double *out_242488807081145783);
void live_h_9(double *state, double *unused, double *out_3613634747775641045);
void live_H_9(double *state, double *unused, double *out_7529707742345593253);
void live_h_10(double *state, double *unused, double *out_5465955997732526423);
void live_H_10(double *state, double *unused, double *out_8346579044930809139);
void live_h_12(double *state, double *unused, double *out_6870131253929931700);
void live_H_12(double *state, double *unused, double *out_5261945215113107578);
void live_h_35(double *state, double *unused, double *out_6460938545316837464);
void live_H_35(double *state, double *unused, double *out_8007508247438121287);
void live_h_32(double *state, double *unused, double *out_7868062134848271243);
void live_H_32(double *state, double *unused, double *out_7734211313369680392);
void live_h_13(double *state, double *unused, double *out_8065559787409327464);
void live_H_13(double *state, double *unused, double *out_2416241343854919983);
void live_h_14(double *state, double *unused, double *out_3613634747775641045);
void live_H_14(double *state, double *unused, double *out_7529707742345593253);
void live_h_33(double *state, double *unused, double *out_1846108831249887148);
void live_H_33(double *state, double *unused, double *out_6759707869092610763);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}