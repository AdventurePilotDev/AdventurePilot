#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_90385049399362206);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_8181508716825132695);
void car_H_mod_fun(double *state, double *out_5759991962524402783);
void car_f_fun(double *state, double dt, double *out_5759603068195081339);
void car_F_fun(double *state, double dt, double *out_653181039187222724);
void car_h_25(double *state, double *unused, double *out_7648381391944393320);
void car_H_25(double *state, double *unused, double *out_2237011312534911106);
void car_h_24(double *state, double *unused, double *out_7085248213111083045);
void car_H_24(double *state, double *unused, double *out_1320914286267412030);
void car_h_30(double *state, double *unused, double *out_7805568060295522465);
void car_H_30(double *state, double *unused, double *out_2366350259678151176);
void car_h_26(double *state, double *unused, double *out_6429677476758224996);
void car_H_26(double *state, double *unused, double *out_5978514631408967330);
void car_h_27(double *state, double *unused, double *out_963781366941022);
void car_H_27(double *state, double *unused, double *out_4541113571478576087);
void car_h_29(double *state, double *unused, double *out_3862647257795442674);
void car_H_29(double *state, double *unused, double *out_1856118915363758992);
void car_h_28(double *state, double *unused, double *out_2276492598979993156);
void car_H_28(double *state, double *unused, double *out_6938517932433289566);
void car_h_31(double *state, double *unused, double *out_6858206660939351830);
void car_H_31(double *state, double *unused, double *out_2206365350657950678);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}