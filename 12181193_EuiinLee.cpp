////�˰��� ������� 2_��å�� ������ ����� ���α׷� ����
////�а�: �����������а�, �й�: 12181193, �̸�: ������

#include <iostream>
#include <vector>
using namespace std;

vector<pair<int, int>> graph[10001]; //(��������ȣ, ���)���� ����Ǵ� �׷���
vector<int> path;  //������ ��ȣ�� ��� ��ι迭
int prim(int s);  //prim�˰����� ������ �Լ�

class Heap { //�켱����ť ������ ����. 
private:
	vector<pair<int, int>> v; // (����ġ, ������ȣ)���� �����ϴ� ����
	int heap_size; //��������
	int root_index;// ��Ʈ�ε���

public:
	Heap() { //������
		v.push_back(make_pair(-1, -1)); //�ε���0�� �����ϹǷ� (-1,-1)�־�α�.
		this->heap_size = 0; //�������� 0���� �ʱ�ȭ
		this->root_index = 1;// ��Ʈ�ε��� 1�� �ʱ�ȭ.
	}

	void swap(int idx1, int idx2) { //swap method ������ �� ���� ��ȯ�ϴ� �Լ�.
		v[0] = v[idx1];
		v[idx1] = v[idx2];
		v[idx2] = v[0];
	}
	void upHeap(int idx) { //����. 
		if (idx == root_index)return;
		else {
			int parent = idx / 2;
			if (v[parent].first == v[idx].first) { //�θ��� ����ġ�� �ڽ��� ����ġ�� ���� ��� ������ȣ�� �� ���� �� �켱������ ���⶧����
				if (v[parent].second > v[idx].second) {//�θ��� ������ȣ�� �� ũ�ٸ� 
					swap(parent, idx);//��ġ�� �ٲٰ�
					upHeap(parent);// �θ� ���ؼ� upHeap�Ͽ� �� ������ �ִ��� ���캻��. 
				}
			}
			else if (v[parent].first > v[idx].first) { //�θ��� ����ġ�� �� ū ��� 
				swap(parent, idx);//swap�Ͽ� �ٲ��ְ�
				upHeap(parent);//�θ� ���� �ٽ� upHeap�Ѵ�. 
			}
		}
	}

	void downHeap(int idx) { //�ٿ���.  (���� ū �� ���� �ö���־ ���ڸ��� ã�� �������� ����) 
		int left = idx * 2; //���� �ڽĳ��
		int right = idx * 2 + 1; //������ �ڽĳ��
		if (right <= heap_size) { 
			if (v[left].first == v[right].first) { //���� �ڽĳ���� ����ġ�� ������ �ڽĳ���� ����ġ�� ���� ���! 
				if (v[left].second < v[right].second) { //������ȣ�� ���� �� ���� ���� ���� ������ swap���ش�. 
					swap(left, idx);
					downHeap(left); //left�� ���� ������ ���� ������ �ٽ� downHeap
				}
				else { //�������� �� ������ �����ʰ� �ڽ��� swap����.
					swap(right, idx);
					downHeap(right);
				}
			}
			else if (v[left].first < v[right].first) {//���� �ڽĳ���� ����ġ�� ������ �ڽĳ���� ����ġ���� �� ���� ���
				if (v[left].first < v[idx].first) { //���� �ڽĳ���� ����ġ�� �� ����ġ���� �� ���� ���
					swap(left, idx);//���� ���� �ڽĳ�� swap
					downHeap(left);
				}
				else return;
			}
			else {//������ �ڽĳ���� ����ġ�� ���� �ڽĳ���� ����ġ���� �� �������
				if (v[right].first < v[idx].first) {  //������ �ڽĳ���� ����ġ�� �� ����ġ���� ���� ���
					swap(right, idx);  //���� swqp 
					downHeap(right);
				}
				else return;
			}
		}
		else if (left <= heap_size) { 
			if (v[left].first < v[idx].first) {
				swap(left, idx);
				downHeap(left);
			}
			else return;
		}
		else return;
	}

	void insert(pair<int, int> e) { //�����Լ�
		v.push_back(e); //���Ϳ� �ְ�
		heap_size++; //�������� �ø���
		upHeap(heap_size); //upHeap���� �� Ư¡ �����Ѵ�.
	}

	void pop() { //���� �켱���� ���� ���� ���� 
		pair<int, int> top = v[root_index];
		v[root_index] = v[heap_size];
		heap_size--;
		v.pop_back();
		downHeap(root_index);
	}

	pair<int, int> top() { return v[root_index]; } //���� �켱���� ���� ���� �����Լ�

	int size() { //�������� �����ϴ� �Լ�
		return heap_size;
	}
	bool isEmpty() { return heap_size == 0; } //���� ������� (�켱����ť�� �������) Ȯ���ϴ� �Լ�
};
int main()
{
	int n, m, q; //n: ������ ������ ��, m : ��å�� ������ ��, q: ���� �� 
	int src, dst, cost; //src: ��� ��������ȣ, dst: ���� ��������ȣ, cost: ���

	cin >> n >> m >> q; //������ ������ ��, ��å�� ������ ��, ������ �� �Է�

	//m���� �ٿ� ���� ���� ���� �Է¹���. 
	for (int i = 0; i < m; i++)
	{
		cin >> src >> dst >> cost;

		//�׷��� ����. (����, ���)����. 
		graph[src].push_back(make_pair(dst, cost));
		graph[dst].push_back(make_pair(src, cost));
	}
	string icon; //���Ǳ�ȣ
	int s, total; //s: �����������ȣ, total: �ּҺ������ �� ���.
	while (q--) { //���� ����ŭ �ݺ�. 
		cin >> icon;
		if (icon == "P") { //��������������
			cin >> s; //�����������ȣ �Է�. 
			total = prim(s); //prim�Լ��� ���� �ּ� ����� ���Ǿ� ���ϵ�. �̸� total������ ����. 
			cout << total << " "; //������
			for (int i = 0; i < path.size(); i++) {
				cout << path[i] << " "; //��� ���. 
			}
			cout << endl;
			path.clear();  //path���� ����. 
		}
	}


}
int prim(int s) { //�����˰��� �Լ�

	Heap pq; //�켱����ť �ϳ� ����. 
	bool visit[10001] = { false };  //�湮���θ� ��� �迭. 
	pq.insert(make_pair(0, s)); // �켱����ť�� ����ġ 0�̰� ������ȣ�� S�� ������ ������ �����Ѵ�. 
	int totalCost = 0;  //��� ����

	while (!pq.isEmpty())  //�켱����ť pq�� �������� �ݺ�
	{
		int curW = pq.top().first; //���� ������ ����Ʈ : ���� �켱���� ���� ������ ����Ʈ
		int curN = pq.top().second;//���� ������ ��ȣ: ���� �켱�������� ������ȣ. 
		pq.pop();  //ť���� top�� �ִ°� pop. 

		if (visit[curN])  //���� ��带 ó���� ���ִ��� Ȯ���ؼ� ó���� ���ִٸ�
			continue;//���ǹ����� �ٷ� ����

		//ó���� ���� ���ٸ� 
		visit[curN] = true; // true�� �ٲ㼭 �湮�ߴٰ� �ٲٱ�. 
		totalCost = totalCost + curW; //�� ��뿡 ���� ����Ʈ ���� ����. 
		path.push_back(curN); //���� ����ȣ�� path�迭�� ���� (�������� ����) 

		//���� �������� ������ �������� �켱����ť�� �־���. 
		for (int i = 0; i < graph[curN].size(); i++)
		{
			int nextN = graph[curN][i].first; //������ ������ ������ȣ
			int nextW = graph[curN][i].second;//������ ������ ����Ʈ

			pq.insert(make_pair(nextW, nextN)); //�켱����ť�� ����(fringe set�� �ֱ�) 
		}
	}
	return totalCost;  //��� ����.
}