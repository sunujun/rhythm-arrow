#pragma once
class ArrowInfo
{
public:
	ArrowInfo();
	~ArrowInfo();
	ArrowInfo(int x);
	ArrowInfo(int t, int d, int l);
	ArrowInfo(int t, int d, int l, int k);
	int arrowX; // x��ǥ
	int arrowY; // y��ǥ
	int arrowT; // �ð����� 10ms == 1
	int arrowD; // ����Ű ���� 0:down, 1:left, 2:right, 3:up
	int arrowL; // ���ӽð�
	int arrowA; // ��Ʈ�� �������� 0:����, 1:����
	int arrowLD;
};

