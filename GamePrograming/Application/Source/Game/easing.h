#pragma once

class Easing {
public:
	Easing() = default;
	~Easing() = default;

	//徐々に減速(二次関数)
	static float OutQuad(float time);
	//徐々に加速(二次関数)
	static float InQuad(float time);
	//遅い速い遅い(二次関数)
	static float InOutQuad(float time);
	//徐々に減速(四次関数)
	static float OutQuart(float time);
	//徐々に加速(四次関数)
	static float InQuart(float time);
	//遅い速い遅い(四次関数)
	static float InOutQuart(float time);
	//伸びたゴムを離した
	static float OutElastic(float time, float amp, float freq);
	//加速して到達地点でバウンド
	static float OutBounce(float time);
	//行き過ぎて戻る
	static float OutBack(float time, float c1 = 1.70158f);

private:

};


