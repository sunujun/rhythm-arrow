#pragma once
#include "Huddle.h"
#include <vector>

#define Hero_x 100

class MG_1
{
public:
	MG_1();
	~MG_1();

	void SetVector(int a);
	void Set_Base(int w, int h);
	void Timer(int time);
	void Get_Jump(int j);
	void Paint(CDC* main_dc,CDC* memdc);

	int width;
	int height;
	int Hero_y;
	int m_hero;
	CRect m_hit; //Ÿ������ Ȯ��
	float m_plus; //������ �ӵ� ������
	int m_jump; //���� ����

	std::vector<Huddle> m_map;
	int head;
	int tail;
	int m_score;
};

