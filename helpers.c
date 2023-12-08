#include <stdint.h>

float custom_abs(float x) {
    return (x < 0) ? -x : x;
}

float custom_sqrt(float x) {
    if (x == 0.0 || x == 1.0) {
        return x;
    }

    float guess = x / 2.0;
    float epsilon = 0.0001;  // Adjust the epsilon value as needed for precision

    while (custom_abs(guess * guess - x) > epsilon) {
        guess = 0.5 * (guess + x / guess);
    }

    return guess;
}