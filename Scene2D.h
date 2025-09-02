#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"
#include "Model2D.h"

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
	bool isDrag;
	Model2D model;

public:
	void addModel(const Model2D& model) {
		this->model = model;
	}
	Scene2D(double X0, double Y0, double px, double py, Model2D model = Model2D()) : Camera2D(X0, Y0, px, py), isDrag(false)
	{

	}
	void Render(HDC dc, bool axes = true)
	{
		if (axes)
			Axes(dc);
		for (int i = 0; i < model.getNumVertices(); ++i)
		{
			for (int j = i + 1; j < model.getNumVertices(); ++j)
			{
				if (model.getAdjacencyMatrix(i, j) == 1)
				{
					double x1 = model.getVerticeX(i);
					double y1 = model.getVerticeY(i);
					double x2 = model.getVerticeX(j);
					double y2 = model.getVerticeY(j);
					MoveTo(model.getVerticeX(i), model.getVerticeY(i));
					LineTo(dc,model.getVerticeX(j), model.getVerticeY(j));
				}
			}
		}
	}
	void Plot(HDC dc, Func f, bool axes=true)
	{
		if (axes)
			Axes(dc);
		MoveTo(L(), f(L()));
		
		for (double x = L(); x < R(); x += 1/px)
		{
			LineTo(dc, x, f(x));
		}
		
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}
	void PolarPlot(HDC dc, Func f, bool axes = true)
	{
		if (axes)
			Axes(dc);
		double phi = 0;
		double r = f(phi);

		MoveTo(PolarToWorldX(r,phi), PolarToWorldY(r, phi));

		for (double phi = 0; phi < 7; phi += 1 / (px*2))
		{
			LineTo(dc, PolarToWorldX(f(phi), phi), PolarToWorldY(f(phi), phi));
		}
		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);
	}
	bool IsDrag() const
	{
		return isDrag;
	}
	void BeginDrag(double x, double y)
	{
		prevX = x;
		prevY = y;
		isDrag = true;
	}
	void EndDrag()
	{
		isDrag = false;
	}
	void Drag(double x, double y)
	{
		this->CameraMove(x, y);
	}
};

#endif SCENE_2D_H
