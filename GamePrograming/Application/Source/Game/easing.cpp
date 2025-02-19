#include "framework.h"
#include "DirectX/DirectX.h"
#include "easing.h"


float Easing::OutQuad(float time) {
    return (-1.0f * time * (time - 2.0f));
}

float Easing::InQuad(float time) {
    return (time * time);
}

float Easing::InOutQuad(float time) {
    time *= 2.0f;
    if (time < 1.0f) {
        return (time * time * 0.5f);
    }
    time -= 1.0f;

    return (time * (time - 2.0f) - 1.0f) * -0.5f;
}

float Easing::OutQuart(float time) {
    time -= 1.0f;
    return -1.0f * (time * time * time * time - 1.0f);
}

float Easing::InQuart(float time) {
    return time * time * time * time;
}

float Easing::InOutQuart(float time) {
    time *= 2.0f;
    if (time < 1.0f) {
        return (0.5f * time * time * time * time);
    }
    time -= 2.0f;
    return (-0.5f * (time * time * time * time - 2.0f));
}

float Easing::OutElastic(float time, float amp, float freq) {
    freq /= 4.0f;
    float sfreq = freq / 4.0f;

    if ((amp > 1.0f) && (time < 0.4f)) {
        amp = 1.0f + (time / 0.4f * (amp - 1.0f));
    }

    return (1.0f + powf(2.0f, -10.0f * time) * sinf((time - sfreq) *  DirectX::XM_2PI / freq) * amp);
}

float Easing::OutBounce(float time) {
    float tmp1 = 2.75f;
    float tmp2 = 7.5625f;

    if (time < (1.0f / tmp1)) {
        return tmp2 * time * time;
    } else if (time < (2.0f / tmp1)) {
        time -= (1.5f / tmp1);
        return tmp2 * time * time + 0.75f;
    } else if (time < (2.5f / tmp1)) {
        time -= (2.25f / tmp1);
        return tmp2 * time * time + 0.9375f;
    } else {
        time -= (2.625f / tmp1);
        return tmp2 * time * time + 0.984375f;
    }
}

float Easing::OutBack(float time, float c1) {
    float c3 = c1 + 1.0f;
    return 1.0f + c3 * powf(time - 1.0f, 3.0f) + c1 * powf(time - 1.0f, 2.0f);
}




