#pragma once

#define GMATH_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define GMATH_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define GMATH_PIN(a, min_value, max_value) GMATH_MIN(max_value, GMATH_MAX(a, min_value))

#define GMATH_VALID_INDEX(idx, range) (((idx) >= 0) && ((idx) < (range)))
#define GMATH_PIN_INDEX(idx, range) GMATH_PIN(idx, 0, (range)-1)

#define GMATH_SIGN(x) ((((x) > 0.0f) ? 1.0f : 0.0f) + (((x) < 0.0f) ? -1.0f : 0.0f))
