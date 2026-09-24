#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_8919129796448948883);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_5759862885670035392);
void pose_H_mod_fun(double *state, double *out_7065747119210693848);
void pose_f_fun(double *state, double dt, double *out_558603448437855289);
void pose_F_fun(double *state, double dt, double *out_1495718385613388763);
void pose_h_4(double *state, double *unused, double *out_5955033610265769474);
void pose_H_4(double *state, double *unused, double *out_1217665312458735127);
void pose_h_10(double *state, double *unused, double *out_6977900249410694712);
void pose_H_10(double *state, double *unused, double *out_4531985994016089052);
void pose_h_13(double *state, double *unused, double *out_1830313317809173444);
void pose_H_13(double *state, double *unused, double *out_4429939137791067928);
void pose_h_14(double *state, double *unused, double *out_194477322244382774);
void pose_H_14(double *state, double *unused, double *out_5180906168798219656);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}