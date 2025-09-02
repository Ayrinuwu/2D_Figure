#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double X0, Y0, Z0;						// Экранные координаты начала мировой системы координат
	double px, py, pz;						// Единицы масштаба мировой системы координат, выраженные в пикселях
	int W, H;							// Разрешение рабочей области окна
	double S_w_to_v[4][4];				// Матрица от мировых координат к видовым
	double S_v_to_p[4][4];				// Матрица от видовых координат к проекционным
	double S_w_to_p[4][4];				// Матрица от мировых координат к проекционным
	double Ov[3];              // Оригинальная точка
	double T[3];               // Направляющий вектор
	double N[3];               // Нормальный вектор
	double D[3];               // Вектор "вверх"

	void setOv(double x, double y, double z) {
		Ov[0] = x;
		Ov[1] = y;
		Ov[2] = z;
		UpdateCamera();
	}

	void setT(double x, double y, double z) {
		T[0] = x;
		T[1] = y;
		T[2] = z;
		UpdateCamera();
	}

	void setN(double x, double y, double z) {
		N[0] = x;
		N[1] = y;
		N[2] = z;
		UpdateCamera();
	}

	void setD(double x, double y, double z) {
		D[0] = x;
		D[1] = y;
		D[2] = z;
		UpdateCamera();
	}

	int WorldToScreenX(double X)		// Переход от мировых координат к экранным (для абсциссы)
	{
		return (int)(X0 + px * X);
	}
	int WorldToScreenY(double Y)		// Переход от мировых координат к экранным (для ординаты)
	{
		return (int)(Y0 + py * Y);
	}
	double ScreenToWorldX(int X)		// Переход от экранных координат к мировым (для абсциссы)
	{
		return (double)(X - X0 + 0.5) / px;
	}
	double ScreenToWorldY(int Y)		// Переход от экранных координат к мировым (для ординаты)
	{
		return (double)(Y - Y0 + 0.5) / py;
	}
	double PolarToWorldX(double r, double phi)		// Переход от полярных координат к мировым (для абсциссы)
	{
		return (double)(r * cos(phi));
	}
	double PolarToWorldY(double r, double phi)		// Переход от полярных координат к мировым (для ординаты)
	{
		return (double)(r * sin(phi));
	}
	
	void CameraMove(double x, double y, double z)
	{
		
		X0 += x-prevX;
		Y0 += y-prevY;
		Z0 += z - prevZ;
		prevX = x;
		prevY = y;
		prevY = z;
	}
	double prevX, prevY,prevZ;
private:
	
	double posX, posY,posZ;					// Позиция графического курсора в мировых координатах (для функций MoveTo и LineTo)
public:
	double L()							// Абсцисса левой границы рабочей области окна (в мировых координатах)
	{
		return ScreenToWorldX(0);
	}
	double R()							// Абсцисса правой границы рабочей области окна (в мировых координатах)
	{
		return ScreenToWorldX(W);
	}
	double T()							// Ордината левой границы рабочей области окна (в мировых координатах)
	{
		return ScreenToWorldY(0);
	}
	double B()							//  Ордината правой границы рабочей области окна (в мировых координатах)
	{
		return ScreenToWorldY(H);
	}
	Camera2D(double X0, double Y0, double px, double py) : X0(X0), Y0(Y0),Z0(Z0), px(px), py(py),pz(pz)
	{
		/*MoveTo(X0, Y0);*/
		/*MoveTo(ScreenToWorldX(X0), ScreenToWorldY(Y0));*/
	}
	void Clear(HDC dc)
	{
		/*InvalidateRect(WindowFromDC(dc), nullptr, true);*/
		RECT rect = { 0, 0, W, H };
		Rectangle(dc, 0, 0, W, H);
		// Очистка рабочей области окна
	}
	void SetResolution(HDC dc)
	{
		RECT rect;
		GetClientRect(WindowFromDC(dc), &rect);
		
		H = rect.bottom + 1;
		W = rect.right + 1;
		/*px = py;*/

		double BT = (B() + T()) / 2;
		double RL = (R() - L()) / 2 * (H/W);
		double Bn = BT + RL;
		double Tn = BT - RL;
		double prvY = Y0;
		
		Y0 = WorldToScreenY((Tn+Bn)/2);

		py *= Y0 / prvY;

		/*px *= Y0 / prvY;*/

		
		// Данная процедура вызывается при изменении размеров окна
		// В ней задаются значения величин W, H, а также настраиваются значения параметров X0, Y0, px, py таким образом, чтобы обеспечить одинаковые масштабы по координатным осям
	}
	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
		// Перемещение графического курсора (posX, posY)
		// Обратите внимание, что мы действуем в мировых координатах
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// Отрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// Обратите внимание, что мы действуем в мировых координатах
		// При отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		MoveTo(L(), 0);
		LineTo(dc, R(), 0);
		MoveTo(0, T());
		LineTo(dc, 0, B());
		// Отрисовка координатных осей
	}
	/*bool IsDrag()*/
	void Scale(double scale, POINT mp)
	{
		
		if (scale > 0)
		{
			scale = 1.1;
		}
		else
		{
			scale = 0.9;
		}
		X0 = mp.x - (mp.x - X0) * scale;
		Y0 = mp.y - (mp.y - Y0) * scale;
		px *= scale;
		py *= scale;
		
	}
	void UpdateCamera() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				S_w_to_p[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					S_w_to_p[i][j] += S_v_to_p[i][k] * S_w_to_v[k][j];
				}
			}
		}

	}
};

#endif CAMERA_2D_H
