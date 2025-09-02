#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double X0, Y0, Z0;						// �������� ���������� ������ ������� ������� ���������
	double px, py, pz;						// ������� �������� ������� ������� ���������, ���������� � ��������
	int W, H;							// ���������� ������� ������� ����
	double S_w_to_v[4][4];				// ������� �� ������� ��������� � �������
	double S_v_to_p[4][4];				// ������� �� ������� ��������� � ������������
	double S_w_to_p[4][4];				// ������� �� ������� ��������� � ������������
	double Ov[3];              // ������������ �����
	double T[3];               // ������������ ������
	double N[3];               // ���������� ������
	double D[3];               // ������ "�����"

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

	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (int)(X0 + px * X);
	}
	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (int)(Y0 + py * Y);
	}
	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (double)(X - X0 + 0.5) / px;
	}
	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (double)(Y - Y0 + 0.5) / py;
	}
	double PolarToWorldX(double r, double phi)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return (double)(r * cos(phi));
	}
	double PolarToWorldY(double r, double phi)		// ������� �� �������� ��������� � ������� (��� ��������)
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
	
	double posX, posY,posZ;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
public:
	double L()							// �������� ����� ������� ������� ������� ���� (� ������� �����������)
	{
		return ScreenToWorldX(0);
	}
	double R()							// �������� ������ ������� ������� ������� ���� (� ������� �����������)
	{
		return ScreenToWorldX(W);
	}
	double T()							// �������� ����� ������� ������� ������� ���� (� ������� �����������)
	{
		return ScreenToWorldY(0);
	}
	double B()							//  �������� ������ ������� ������� ������� ���� (� ������� �����������)
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
		// ������� ������� ������� ����
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

		
		// ������ ��������� ���������� ��� ��������� �������� ����
		// � ��� �������� �������� ������� W, H, � ����� ������������� �������� ���������� X0, Y0, px, py ����� �������, ����� ���������� ���������� �������� �� ������������ ����
	}
	void MoveTo(double X, double Y)
	{
		posX = X;
		posY = Y;
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
	}
	void LineTo(HDC dc, double X, double Y)
	{
		::MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);
		::LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));
		MoveTo(X, Y);
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)
	}
	void Axes(HDC dc)
	{
		MoveTo(L(), 0);
		LineTo(dc, R(), 0);
		MoveTo(0, T());
		LineTo(dc, 0, B());
		// ��������� ������������ ����
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
