#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glut.h>
#include <fstream>
#include "halfedge.h"
using namespace std;

struct POINT3
{
	double X;
	double Y;
	double Z;
};

struct TEXTURE2
{
	double TU;
	double TV;
};

struct NORMAL3
{
	double NX;
	double NY;
	double NZ;
};

struct FACE3
{
	int V[3];
	int T[3];
	int N[3];
};

class Model
{
public:
	vector<POINT3> V;
	vector<TEXTURE2> VT;
	vector<NORMAL3> VN;
	vector<FACE3> F;
};

Model m_model;

void readmodel()
{
//	ifstream FileName("cube_loop.obj");
	ifstream FileName("bunny.obj");
//	ifstream FileName("Eight.obj");
	string s;
	FACE3 *f;
	POINT3 *v;
	NORMAL3 *vn;
	TEXTURE2 *vt;

	while (getline(FileName, s))
	{
		if (s.length() < 2)
			continue;
		if (s[0] == 'v')
		{
			if (s[1] == 't')
			{
				istringstream in(s);
				vt = new TEXTURE2();
				string head;
				in >> head >> vt->TU >> vt->TV;
				m_model.VT.push_back(*vt);
			}

			else if (s[1] == 'n')
			{
				istringstream in(s);
				vn = new NORMAL3();
				string head;
				in >> head >> vn->NX >> vn->NY >> vn->NZ;
				m_model.VN.push_back(*vn);
			}

			else
			{
				istringstream in(s);
				v = new POINT3();
				string head;
				in >> head >> v->X >> v->Y >> v->Z;
				m_model.V.push_back(*v);
			}
		}
		else if (s[0] == 'f')
		{
			for (int k = s.size() - 1; k >= 0; k--)
			{
				if (s[k] == '/')
					s[k] = ' ';
			}
			istringstream in(s);
			f = new FACE3();
			string head;
			in >> head;
			int i = 0;
			while (i < 3)
			{
				if (m_model.V.size() != 0)
				{
					in >> f->V[i];
					f->V[i] -= 1;
				}
				if (m_model.VT.size() != 0)
				{
					in >> f->T[i];
					f->T[i] -= 1;
				}
				if (m_model.VN.size() != 0)
				{
					in >> f->N[i];
					f->N[i] -= 1;
				}
				i++;
			}
			m_model.F.push_back(*f);
		}
	}
}

void drawmodel()
{
	int scale = 5;

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
//	glColor3d(1, 0, 1);
//	glTranslated(0, -0.5, -0.5);
////	glRotated(45, 1, 0, 0);

	for (int i = 0; i < m_model.F.size();i++)
	{

		float d = m_model.VN[m_model.F[i].N[0]].NX + m_model.VN[m_model.F[i].N[0]].NY + m_model.VN[m_model.F[i].N[0]].NZ;
		glBegin(GL_TRIANGLES);
		if (m_model.VT.size() != 0)  glTexCoord2f(m_model.VT[m_model.F[i].T[0]].TU, m_model.VT[m_model.F[i].T[0]].TV);
		if (m_model.VN.size() != 0)  glNormal3f(m_model.VN[m_model.F[i].N[0]].NX, m_model.VN[m_model.F[i].N[0]].NY, m_model.VN[m_model.F[i].N[0]].NZ);
		//glVertex3f(m_model.V[m_model.F[i].V[0]].X, m_model.V[m_model.F[i].V[0]].Y, m_model.V[m_model.F[i].V[0]].Z);
		glVertex3f(m_model.V[m_model.F[i].V[0]].X*scale, m_model.V[m_model.F[i].V[0]].Y*scale, m_model.V[m_model.F[i].V[0]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[0]].X / scale, m_model.V[m_model.F[i].V[0]].Y / scale, m_model.V[m_model.F[i].V[0]].Z / scale);

		if (m_model.VT.size() != 0)  glTexCoord2f(m_model.VT[m_model.F[i].T[1]].TU, m_model.VT[m_model.F[i].T[1]].TV);
		if (m_model.VN.size() != 0)  glNormal3f(m_model.VN[m_model.F[i].N[1]].NX, m_model.VN[m_model.F[i].N[1]].NY, m_model.VN[m_model.F[i].N[1]].NZ);
		//glVertex3f(m_model.V[m_model.F[i].V[1]].X, m_model.V[m_model.F[i].V[1]].Y, m_model.V[m_model.F[i].V[1]].Z);
		glVertex3f(m_model.V[m_model.F[i].V[1]].X*scale, m_model.V[m_model.F[i].V[1]].Y*scale, m_model.V[m_model.F[i].V[1]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[1]].X / scale, m_model.V[m_model.F[i].V[1]].Y / scale, m_model.V[m_model.F[i].V[1]].Z / scale);

		if (m_model.VT.size() != 0)  glTexCoord2f(m_model.VT[m_model.F[i].T[2]].TU, m_model.VT[m_model.F[i].T[2]].TV);
		if (m_model.VN.size() != 0)  glNormal3f(m_model.VN[m_model.F[i].N[2]].NX, m_model.VN[m_model.F[i].N[2]].NY, m_model.VN[m_model.F[i].N[2]].NZ);
		//glVertex3f(m_model.V[m_model.F[i].V[2]].X, m_model.V[m_model.F[i].V[2]].Y, m_model.V[m_model.F[i].V[2]].Z);
		glVertex3f(m_model.V[m_model.F[i].V[2]].X*scale, m_model.V[m_model.F[i].V[2]].Y*scale, m_model.V[m_model.F[i].V[2]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[2]].X / scale, m_model.V[m_model.F[i].V[2]].Y / scale, m_model.V[m_model.F[i].V[2]].Z / scale);

		glEnd();

		//
		//glBegin(GL_LINES);
		//if (m_model.VN.size() != 0) {
		////glBegin(GL_LINES);                            // 绘制三角形
		//glVertex3f(m_model.V[m_model.F[i].V[0]].X*scale, m_model.V[m_model.F[i].V[0]].Y*scale, m_model.V[m_model.F[i].V[0]].Z*scale);        // 上顶点
		//glVertex3f(m_model.V[m_model.F[i].V[1]].X*scale, m_model.V[m_model.F[i].V[1]].Y*scale, m_model.V[m_model.F[i].V[1]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[1]].X*scale, m_model.V[m_model.F[i].V[1]].Y*scale, m_model.V[m_model.F[i].V[1]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[2]].X*scale, m_model.V[m_model.F[i].V[2]].Y*scale, m_model.V[m_model.F[i].V[2]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[2]].X*scale, m_model.V[m_model.F[i].V[2]].Y*scale, m_model.V[m_model.F[i].V[2]].Z*scale);
		//glVertex3f(m_model.V[m_model.F[i].V[0]].X*scale, m_model.V[m_model.F[i].V[0]].Y*scale, m_model.V[m_model.F[i].V[0]].Z*scale);

		//}// 左下
		//glEnd();
		//
	}
	//glutSwapBuffers();
}

