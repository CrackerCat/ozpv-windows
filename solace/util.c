#include "dec.h"

int random(void) {
	int r = 0;
	HCRYPTPROV c;

	CryptAcquireContext(&c, NULL, NULL, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT);
	CryptGenRandom(c, sizeof(int), (BYTE*)&r);
	CryptReleaseContext(c, 0);

	return r & INT_MAX;
}

/* not the best idea to use inline asm, but it will decrease the file size. 
 * planning on finding an alternative.
 */

float asmsin(float x) {
	__asm {
		fld x
		fsin
	}
}

float asmround(float x) {
	__asm {
		fld x
		frndint
	}
}

HSL RGBtoHSL(BYTE Red, BYTE Green, BYTE Blue) {
	HSL hsl;

	float d;
	float rf = Red / 255.0f;
	float gf = Green / 255.0f;
	float bf = Blue / 255.0f;
	float max = max(max(rf, gf), bf);
	float min = min(min(rf, gf), bf);
	float h = (max + min) / 2.0f;
	float s = (max + min) / 2.0f;
	float l = (max + min) / 2.0f;

	if (max == min) {
		h = 0.0f;
		s = 0.0f;
	}
	else {
		d = max - min;
		s = (l > 0.5) ? d / (2 - max - min) : d / (max + min);

		if (max == rf) {
			h = (gf - bf) / d;
			if (gf < bf)
				h += 6.0f;
		}
		else if (max == gf)
			h = (bf - rf) / d + 2.0f;
		else if (max == bf)
			h = (rf - gf) / d + 4.0f;

		h /= 6;
	}

	hsl.Hue = asmround(h * 360.0f);
	hsl.Saturation = s;
	hsl.Lightness = l;

	return hsl;
}

float HUEtoCOLOR(float p, float q, float t) {
	if (t < 0.0f)
		t++;
	if (t > 1.0f)
		t--;
	if (1.0f / 6.0f > t)
		return p + (q - p) * 6 * t;
	if (1.0f / 2.0f > t)
		return q;
	if (2.0f / 3.0f > t)
		return p + (q - p) * (2.0f / 3.0f - t) * 6;
	return p;
}

RGB HSLtoRGB(float Hue, float Saturation, float Lightness) {
	RGB rgb;

	float q;
	float p;
	float h = abs(Hue) % 360 / 360.0f;
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;

	if (0.0f == Saturation) {
		r = Lightness;
		g = Lightness;
		b = Lightness;
	}
	else {
		q = Lightness < 0.5f ? Lightness * (1.0f + Saturation) : Lightness + Saturation - Lightness * Saturation;
		p = 2.0f * Lightness - q;
		r = HUEtoCOLOR(p, q, h + 1.0f / 3.0f);
		g = HUEtoCOLOR(p, q, h);
		b = HUEtoCOLOR(p, q, h - 1.0f / 3.0f);
	}

	rgb.Red = asmround(r * 255);
	rgb.Green = asmround(g * 255);
	rgb.Blue = asmround(b * 255);
	rgb.Reserved = 0;

	return rgb;
}

BOOL ProcessSetPrivilege(LPCWSTR lpPrivilegeName, DWORD dwAttributes) {
	HANDLE ht;
	TOKEN_PRIVILEGES tp;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = dwAttributes;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ht);

	LookupPrivilegeValue(NULL, lpPrivilegeName, &tp.Privileges[0].Luid);

	AdjustTokenPrivileges(ht, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);

	return 0;
}