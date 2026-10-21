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
void car_err_fun(double *nom_x, double *delta_x, double *out_8811977435261191108);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_6946685726979547511);
void car_H_mod_fun(double *state, double *out_1170278188409279072);
void car_f_fun(double *state, double dt, double *out_1916089704721153233);
void car_F_fun(double *state, double dt, double *out_4987018272475214840);
void car_h_25(double *state, double *unused, double *out_3532977926145256286);
void car_H_25(double *state, double *unused, double *out_7849974308245583983);
void car_h_24(double *state, double *unused, double *out_3307929210016927272);
void car_H_24(double *state, double *unused, double *out_5677324709240084417);
void car_h_30(double *state, double *unused, double *out_3237857300205094650);
void car_H_30(double *state, double *unused, double *out_3322277978117975785);
void car_h_26(double *state, double *unused, double *out_8239530567865940295);
void car_H_26(double *state, double *unused, double *out_4108470989371527759);
void car_h_27(double *state, double *unused, double *out_2589468073243593851);
void car_H_27(double *state, double *unused, double *out_1147514666317550874);
void car_h_29(double *state, double *unused, double *out_8809845256090607820);
void car_H_29(double *state, double *unused, double *out_3832509322432367969);
void car_h_28(double *state, double *unused, double *out_7591141340904439496);
void car_H_28(double *state, double *unused, double *out_5796139593997694220);
void car_h_31(double *state, double *unused, double *out_6273888670845753446);
void car_H_31(double *state, double *unused, double *out_3482262887138176283);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}