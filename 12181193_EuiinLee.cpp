////알고리즘 설계과저 2_산책로 디자인 비용계산 프로그램 설계
////학과: 공간정보공학과, 학번: 12181193, 이름: 이의인

#include <iostream>
#include <vector>
using namespace std;

vector<pair<int, int>> graph[10001]; //(조형물번호, 비용)쌍이 저장되는 그래프
vector<int> path;  //조형물 번호가 담긴 경로배열
int prim(int s);  //prim알고리즘을 수행할 함수

class Heap { //우선순위큐 힙으로 구현. 
private:
	vector<pair<int, int>> v; // (가중치, 정점번호)쌍을 저장하는 벡터
	int heap_size; //힙사이즈
	int root_index;// 루트인덱스

public:
	Heap() { //생성자
		v.push_back(make_pair(-1, -1)); //인덱스0은 사용안하므로 (-1,-1)넣어두기.
		this->heap_size = 0; //힙사이즈 0으로 초기화
		this->root_index = 1;// 루트인덱스 1로 초기화.
	}

	void swap(int idx1, int idx2) { //swap method 벡터의 두 셀을 교환하는 함수.
		v[0] = v[idx1];
		v[idx1] = v[idx2];
		v[idx2] = v[0];
	}
	void upHeap(int idx) { //업힙. 
		if (idx == root_index)return;
		else {
			int parent = idx / 2;
			if (v[parent].first == v[idx].first) { //부모의 가중치와 자신의 가중치가 같을 경우 정점번호가 더 낮은 게 우선순위를 갖기때문에
				if (v[parent].second > v[idx].second) {//부모의 정점번호가 더 크다면 
					swap(parent, idx);//위치를 바꾸고
					upHeap(parent);// 부모에 대해서 upHeap하여 또 문제가 있는지 살펴본다. 
				}
			}
			else if (v[parent].first > v[idx].first) { //부모의 가중치가 더 큰 경우 
				swap(parent, idx);//swap하여 바꿔주고
				upHeap(parent);//부모에 대해 다시 upHeap한다. 
			}
		}
	}

	void downHeap(int idx) { //다운힙.  (값이 큰 게 위에 올라와있어서 제자리를 찾아 내려가는 과정) 
		int left = idx * 2; //왼쪽 자식노드
		int right = idx * 2 + 1; //오른쪽 자식노드
		if (right <= heap_size) { 
			if (v[left].first == v[right].first) { //왼쪽 자식노드의 가중치와 오른쪽 자식노드의 가중치가 같을 경우! 
				if (v[left].second < v[right].second) { //정점번호를 비교해 더 작은 것이 위로 가도록 swap해준다. 
					swap(left, idx);
					downHeap(left); //left에 대해 문제가 없을 때까지 다시 downHeap
				}
				else { //오른쪽이 더 작으면 오른쪽과 자신을 swap해줌.
					swap(right, idx);
					downHeap(right);
				}
			}
			else if (v[left].first < v[right].first) {//왼쪽 자식노드의 가중치가 오른쪽 자식노드의 가중치보다 더 작은 경우
				if (v[left].first < v[idx].first) { //왼쪽 자식노드의 가중치가 내 가중치보다 더 작은 경우
					swap(left, idx);//나와 왼쪽 자식노드 swap
					downHeap(left);
				}
				else return;
			}
			else {//오른쪽 자식노드의 가중치가 왼쪽 자식노드의 가중치보다 더 작은경우
				if (v[right].first < v[idx].first) {  //오른쪽 자식노드의 가중치가 내 가중치보다 작은 경우
					swap(right, idx);  //나와 swqp 
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

	void insert(pair<int, int> e) { //삽입함수
		v.push_back(e); //벡터에 넣고
		heap_size++; //힙사이즈 늘리고
		upHeap(heap_size); //upHeap으로 힙 특징 유지한다.
	}

	void pop() { //가장 우선순위 높은 원소 삭제 
		pair<int, int> top = v[root_index];
		v[root_index] = v[heap_size];
		heap_size--;
		v.pop_back();
		downHeap(root_index);
	}

	pair<int, int> top() { return v[root_index]; } //가장 우선순위 높은 원소 리턴함수

	int size() { //힙사이즈 리턴하는 함수
		return heap_size;
	}
	bool isEmpty() { return heap_size == 0; } //힙이 비었는지 (우선순위큐가 비었는지) 확인하는 함수
};
int main()
{
	int n, m, q; //n: 조형물 정보의 수, m : 산책로 정보의 수, q: 질의 수 
	int src, dst, cost; //src: 출발 조형물번호, dst: 도착 조형물번호, cost: 비용

	cin >> n >> m >> q; //조형물 정보의 수, 산책로 정보의 수, 질의의 수 입력

	//m개의 줄에 걸쳐 간선 정보 입력받음. 
	for (int i = 0; i < m; i++)
	{
		cin >> src >> dst >> cost;

		//그래프 만듦. (정점, 비용)저장. 
		graph[src].push_back(make_pair(dst, cost));
		graph[dst].push_back(make_pair(src, cost));
	}
	string icon; //질의기호
	int s, total; //s: 출발조형물번호, total: 최소비용계산한 총 비용.
	while (q--) { //질의 수만큼 반복. 
		cin >> icon;
		if (icon == "P") { //설계과정출력질의
			cin >> s; //출발조형물번호 입력. 
			total = prim(s); //prim함수를 통해 최소 비용이 계산되어 리턴됨. 이를 total변수에 저장. 
			cout << total << " "; //비용출력
			for (int i = 0; i < path.size(); i++) {
				cout << path[i] << " "; //경로 출력. 
			}
			cout << endl;
			path.clear();  //path벡터 비우기. 
		}
	}


}
int prim(int s) { //프림알고리즘 함수

	Heap pq; //우선순위큐 하나 생성. 
	bool visit[10001] = { false };  //방문여부를 담는 배열. 
	pq.insert(make_pair(0, s)); // 우선순위큐에 가중치 0이고 정점번호가 S인 시작할 정점을 삽입한다. 
	int totalCost = 0;  //비용 변수

	while (!pq.isEmpty())  //우선순위큐 pq가 빌때까지 반복
	{
		int curW = pq.top().first; //현재 정점의 웨이트 : 가장 우선순위 높은 정점의 웨이트
		int curN = pq.top().second;//현재 정점의 번호: 가장 우선순위높은 정점번호. 
		pq.pop();  //큐에서 top에 있는거 pop. 

		if (visit[curN])  //현재 노드를 처리한 적있는지 확인해서 처리한 적있다면
			continue;//조건문으로 바로 가기

		//처리한 적이 없다면 
		visit[curN] = true; // true로 바꿔서 방문했다고 바꾸기. 
		totalCost = totalCost + curW; //총 비용에 현재 웨이트 누적 덧셈. 
		path.push_back(curN); //현재 노드번호를 path배열에 삽입 (경로출력을 위해) 

		//현재 정점에서 인접한 정점들을 우선순위큐에 넣어줌. 
		for (int i = 0; i < graph[curN].size(); i++)
		{
			int nextN = graph[curN][i].first; //인접한 정점의 정점번호
			int nextW = graph[curN][i].second;//인접한 정점의 웨이트

			pq.insert(make_pair(nextW, nextN)); //우선순위큐에 삽입(fringe set에 넣기) 
		}
	}
	return totalCost;  //비용 리턴.
}