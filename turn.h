#pragma once
enum turn { x = 1, o = 2 };
inline const char* ToString(turn t) { return t == x ? "X" : "O"; }
