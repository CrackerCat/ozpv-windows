/*
 * proto.h:
 * This header contains functions and definitions that were omitted from the project. 
 * They might be useful later, or to you. This header is not included on compile time.
 */


/*
#define RADIANS(degree) degree*M_PI_F/180

typedef struct HSV {
	float Hue;
	float Saturation;
	float Value;
};

float asmcos(float x);
float asmsqrt(float x);
BYTE clamp(float val);
BYTE clampb(BYTE val, float bright);
DWORD WINAPI YIQHueRotation(LPVOID lParam);*/

/*float asmcos(float x) {
	__asm {
		fld x
		fcos
	}
}

float asmsqrt(float x) {
	__asm {
		fld x
		fsqrt
	}
}*/

/*BYTE clamp(float val) {
	if (val < 0)
		return 0;
	if (val > 255)
		return 255;
	return (BYTE)val;
}

BYTE clampb(BYTE val, float bright) {
	if ((val * bright) > 255)
		return 255;
	return (BYTE)val * bright;
}*/

/* HSV "Hue" Rotation applied from a YIQ matrix. This has been scrapped because 
 * it returns inconsistent RGB values. The "Hue" is indeed modified, but so is the
 * Brightness and Saturation. This effect especially shows when ran inside of a loop
 * like shown here.
 * The matrix however can be used for faster rotations since it only needs to be calculated once.
 */

/*DWORD WINAPI YIQHueRotation(LPVOID lParam) {
	METRICS *m = (METRICS*)lParam;
	HDC ddc = GetDC(m->dwh);
	HDC cdc = CreateCompatibleDC(ddc);
	RGBQUAD *rq;
	RGBQUAD rqt;
	HSV hsv = { 6.0f, 1.0f, 1.0f };
	BITMAPINFO bi = { sizeof(BITMAPINFOHEADER), m->dw, m->dh, 1, 32, BI_RGB };

	HBITMAP hdib = CreateDIBSection(ddc, &bi, DIB_RGB_COLORS, (void**)&rq, NULL, 0);

	if (hdib == NULL)
		return 1;

	SelectObject(cdc, hdib);

	float cost = hsv.Value * hsv.Saturation * asmcos(RADIANS(hsv.Hue));
	float sint = hsv.Value * hsv.Saturation * asmsin(RADIANS(hsv.Hue));

	float matrix[3][3] = {
		{cost + (1.0f - cost) / 3.0f, 1.0f / 3.0f * (1.0f - cost) - asmsqrt(1.0f / 3.0f) * sint, 1.0f / 3.0f * (1.0f - cost) + asmsqrt(1.0f / 3.0f) * sint},
		{1.0f / 3.0f * (1.0f - cost) + asmsqrt(1.0f / 3.0f) * sint, cost + 1.0f / 3.0f * (1.0f - cost), 1.0f / 3.0f * (1.0f - cost) - asmsqrt(1.0f / 3.0f) * sint},
		{1.0f / 3.0f * (1.0f - cost) - asmsqrt(1.0f / 3.0f) * sint, 1.0f / 3.0f * (1.0f - cost) + asmsqrt(1.0f / 3.0f) * sint, cost + 1.0f / 3.0f * (1.0f - cost)}
	};

	while (TRUE) {
		BitBlt(cdc, 0, 0, m->dw, m->dh, ddc, 0, 0, SRCCOPY);
		for (int i = 0; i < m->dw * m->dh; i++) {
			rqt.rgbRed = clamp(rq[i].rgbRed * matrix[0][0] + rq[i].rgbGreen * matrix[0][1] + rq[i].rgbBlue * matrix[0][2]) * hsv.Value;
			rqt.rgbGreen = clamp(rq[i].rgbRed * matrix[1][0] + rq[i].rgbGreen * matrix[1][1] + rq[i].rgbBlue * matrix[1][2]) * hsv.Value;
			rqt.rgbBlue = clamp(rq[i].rgbRed * matrix[2][0] + rq[i].rgbGreen * matrix[2][1] + rq[i].rgbBlue * matrix[2][2]) * hsv.Value;
			rqt.rgbRed = clampb(rqt.rgbRed, 1.01f);
			rqt.rgbGreen = clampb(rqt.rgbGreen, 1.01f);
			rqt.rgbBlue = clampb(rqt.rgbBlue, 1.01f);
			rqt.rgbReserved = 0;
			rq[i] = rqt;
		}
		BitBlt(ddc, 0, 0, m->dw, m->dh, cdc, 0, 0, SRCCOPY);
		Sleep(5);
	}
}*/