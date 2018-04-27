#pragma once


struct Loudness{

	float time;
	float value;
	int remarkable;

	Loudness() {
		time = 0;
		value = 0;
		remarkable = false;
	}

	Loudness(float t, float v, int r) {
		time = t;
		value = v;
		remarkable = r;
	}

	Loudness(TArray<FString> loudParams) {
		time = FCString::Atof(*loudParams[0]);
		value = FCString::Atof(*loudParams[1]);
		remarkable = FCString::Atoi(*loudParams[2]);
	}

};