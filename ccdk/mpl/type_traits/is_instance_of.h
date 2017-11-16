#pragma once

#define is_instance_of(a, T) is_same_v<T, decltype(a)>