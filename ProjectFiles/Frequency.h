#pragma once

struct Frequency {

	float time;
	float duration;
	float value;

	Frequency() {
		time = 0;
		duration = 0;
		value = 0;
	}

	Frequency(float t, float d, float v) {
		time = t;
		duration = d;
		value = v;
	}

};
