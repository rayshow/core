#pragma once

#include<ccdk/mpl/mpl_module.h>
#include<ccdk/mpl/base/bool_.h>
#include<ccdk/mpl/mcontainer/val_pack.h>

ccdk_namespace_mpl_units_start

//base
typedef val_pack< uint8, 1, 0, 0, 0, 0, 0, 0> mass_;
typedef val_pack< uint8, 0, 1, 0, 0, 0, 0, 0> length_;
typedef val_pack< uint8, 0, 0, 1, 0, 0, 0, 0> time_;
typedef val_pack< uint8, 0, 0, 0, 1, 0, 0, 0> charge_;
typedef val_pack< uint8, 0, 0, 0, 0, 1, 0, 0> temperature_;
typedef val_pack< uint8, 0, 0, 0, 0, 0, 1, 0> intensity_;
typedef val_pack< uint8, 0, 0, 0, 0, 0, 0, 1> substance_;
typedef val_pack< uint8, 0, 0, 0, 0, 0, 0, 0> scaler_;

//compose
typedef val_pack< uint8, 0, 1, -1, 0, 0, 0, 0> velocity_;     /* length / time   */
typedef val_pack< uint8, 0, 1, -2, 0, 0, 0, 0> acceleration_; /* length / time^2 */
typedef val_pack< uint8, 1, 1, -1, 0, 0, 0, 0> momentum_;     /* mass * velocity */
typedef val_pack< uint8, 1, 1, -2, 0, 0, 0, 0> force_;


ccdk_namespace_mpl_units_end