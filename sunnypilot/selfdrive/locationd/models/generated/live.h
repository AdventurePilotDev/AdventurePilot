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
void live_H(double *in_vec, double *out_8084260872127367100);
void live_err_fun(double *nom_x, double *delta_x, double *out_6761957117083789292);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_7365178126072744273);
void live_H_mod_fun(double *state, double *out_608253720416289053);
void live_f_fun(double *state, double dt, double *out_7678752018667876776);
void live_F_fun(double *state, double dt, double *out_2189268329336555190);
void live_h_4(double *state, double *unused, double *out_7515555695815230231);
void live_H_4(double *state, double *unused, double *out_3702021727079942022);
void live_h_9(double *state, double *unused, double *out_8394289488518294222);
void live_H_9(double *state, double *unused, double *out_3585197208184505448);
void live_h_10(double *state, double *unused, double *out_1628295188826542340);
void live_H_10(double *state, double *unused, double *out_6142649344153323419);
void live_h_12(double *state, double *unused, double *out_793020812889191275);
void live_H_12(double *state, double *unused, double *out_8363463969586876598);
void live_h_35(double *state, double *unused, double *out_1553940871748869296);
void live_H_35(double *state, double *unused, double *out_4062997713277033482);
void live_h_32(double *state, double *unused, double *out_8260201670763494065);
void live_H_32(double *state, double *unused, double *out_1536412699590048751);
void live_h_13(double *state, double *unused, double *out_6446855917223484905);
void live_H_13(double *state, double *unused, double *out_3239455093687013097);
void live_h_14(double *state, double *unused, double *out_8394289488518294222);
void live_H_14(double *state, double *unused, double *out_3585197208184505448);
void live_h_33(double *state, double *unused, double *out_5280090123074576646);
void live_H_33(double *state, double *unused, double *out_7213554717915891086);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}