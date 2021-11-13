#include<iostream>
#define BLACK 0
#define RED 1
using namespace std;
class Node{// 어플리케이션 정보
private:
	int id; //ID (기준 key) 
	string name;//이름
	int size;//용량
	int price;//가격
	Node* par;  //부모노드 가리키는 포인터
	Node* left; //왼쪽 자식노드 가리키는 포인터
	Node* right;//오른쪽 자식노드 가리키는 포인터
	int color; 
	
public: 
	Node() {
		this->id = 0;
		this->name = "";
		this->size = 0;
		this->price = 0;
		this->par = NULL;
		this->left = NULL;
		this->right = NULL;
		this->color = RED;
	}
	Node(int id, string name, int size, int price) {
		this->id = id;
		this->name = name; 
		this->size = size; 
		this->price = price;
		this->par = NULL;
		this->left = NULL;
		this->right = NULL;		
		this->color = RED;
	}
	void setLeft(Node* node) {
		if (node == NULL) { this->left = NULL; }
		else { this->left = node; node->par = this; }
	}
	void setRight(Node* node) {
		if (node == NULL) this->right = NULL;
		else { this->right = node; node->par = this; }
	}
	string getName() {
		return this->name;
	}
	void setName(string name) {
		this->name = name;
	}
	int getSize() {
		return this->size;
	}
	void setSize(int size) {
		this->size = size;
	}
	int getPrice() {
		return this->price;
	}
	void setPrice(int price) {
		this->price = price; 
	}
	friend class RBT;
	

};
class RBT { //레드블랙트리를 이용한 기능구현
public: 
	Node* root;
	RBT() {
		this->root = NULL;
	}
	
	Node* search(int id) { //검색기능
		// :id를 통해 검색->루트부터 내려가면서 찾음->존재하면 깊이, 이름, 용량, 가격 출력/ 존재하지않으면 NULL출력인데
		// 일단 이 함수를 통해서는 존재하면 해당노드를 리턴하고 존재하지않으면 NULL을 출력하도록 함.  
		Node* curN = this->root; //현재노드 루트노드로 설정. 
		while (curN != NULL) {
			if (curN->id == id) //현재노드의 ID가 찾는 ID라면
				return curN; //현재노드 리턴.
			else if (id > curN->id) curN = curN->right; //찾는 ID가 현재노드의 ID보다 크다면 현재노드의 오른쪽 노드로 이동. 
			else curN = curN->left; //찾는 ID가 현재노드의 ID보다 작다면 현재노드의 왼쪽 노드로 이동. 
		}
		//계속 내려가서 leaf노드를 만나면 curN이 NULL이되어 while문을 탈출하게 됨.
		// -> 찾는 ID가 존재하지않으므로 NULL리턴. 
		return NULL; 
	}

	int depth(int id) { //특정 ID를 갖는 노드의 높이를 알아내는 메서드
		Node* node = this->search(id); //특정 ID를 갖는 노드를 가져옴. 
		int dep = 0; 
		while (node != root) { //노드가 루트를 만날때까지 
			node = node->par; //한칸씩 올라가면서 
			dep++; //1씩 증가시킴. 
		}
		return dep;
	}

	void insert(int id, string name, int size, int price) {
		Node* node = new Node(id, name, size, price); //해당 값을 갖는 새 노드 생성
		//빈 트리일 경우
		if (this->root == NULL) {
			this->root = node; //루트에 새 노드 삽입하고 
			root->color = BLACK;
			return; //함수종료
		}

		//삽입할 위치 찾기 
		Node* parN = NULL;
		Node* curN = this->root; //현재노드 루트노드로 설정. 


		//1. 존재여부확인+삽입할 위치 찾기 
		while (curN != NULL) {
			if (curN->id == id) { //삽입하려는 id가 이미 있다면 (등록하려는 앱이 이미 등록되어있다면) 
				//cout << this->depth(id) << endl; //endl; 넣을지말지확인!!!!!!!!!!!!!!!!!!
				return;
			}
			else if (curN->id < id) { //현재노드의 ID가 삽입할 ID보다 작은 경우 -> 오른쪽자식으로 이동
				parN = curN;  //부모노드를 현재노드로 바꾸고
				curN = curN->right; //현재노드를 오른쪽자식으로 바꿈.
			}
			else { //현재노드의 ID가 삽입할 ID보다 큰 경우-> 왼쪽자식으로 이동
				parN = curN;  //부모노드는 현재노드로 바꾸고
				curN = curN->left;  //현재노드를 왼쪽자식으로 바꿈. 
			}
		}
		//이미 존재하는 경우가 아니라면 leaf노드까지 내려옴. 
		//현재노드 curN이 NULL이 되어 while문을 탈출하게 됨. 
		//부모노드의 두 leaf자식노드중 하나가 삽입할 위치. 

		//2. 찾은 위치에 노드삽입. 
		if (id > parN->id) //부모노드의 ID보다 크면 
			parN->setRight(node); //오른쪽 자식에 넣고
		else if (id < parN->id) //부모노드의 ID보다 작으면 
			parN->setLeft(node); //왼쪽 자식에 넣음. 

		insert_root(node); //이 함수로 보내면 재구조까지끝마치고 옴. 

		return;
		//3.내부속성 만족을 위한 더블레드체크
		//삽입하는 노드는 루트가 아니면 레드이므로 이때 부모노드도 레드면 더블레드로 내부속성 불만족.  

		/*while (parN->color == RED) { //부모노드==RED만 해도 되는지 확인!!!!!!!!!!!!!
			Node* uncleN;  //나중에 지워줘야하는지 생각하기!!!!!!!!!!!!!!!!!!!!!
			Node* parN = node->par;
			if (parN == root)
				return; //부모노드가 루트노드면, 루트는 블랙, 삽입한 노드는 레드로 더블레드발생x. 함수종료. 
			else if (parN == parN->par->left) //부모가 왼쪽자식이라면 엉클노드는 오른쪽 자식
				uncleN = parN->par->right;
			else if (parN == parN->par->right) //부모노드가 오른쪽 자식이라면 엉클노드는 왼쪽자식. 
				uncleN = parN->par->left;
			// 나중에 else if 를 else로 바꿔서도 해보기!!!!!!!!!!!!!!
			//uncle노드 색확인후 BLACK이면 Restructuring, RED면 Recoloring.
		}*/
	}
	Node* uncle(Node* node) { 		//uncle노드 
		Node* uncleN=new Node();//나중에 지워줘야하는지 생각하기!!!!!!!!!!!!!!!!!!!!!
		Node* parN = node->par;
		if (parN == root)
			return NULL;
		else if (parN == parN->par->left) //부모가 왼쪽자식이라면 엉클노드는 오른쪽 자식
			uncleN = parN->par->right;
		else if (parN == parN->par->right) //부모노드가 오른쪽 자식이라면 엉클노드는 왼쪽자식. 
			uncleN = parN->par->left;
		return uncleN;
	}
	Node* grand(Node* node) {
		Node* grandN = node->par->par; 
		return grandN;
	}
	void insert_root(Node* node) {
		if (node->par == NULL) //부모가 없으면 루트노드이므로 
			node->color = BLACK;// black으로 바꿔줌.
		else
			insert_parB(node);  
	}
		
	void insert_parB(Node* node) {
		if (node->par->color == BLACK)
			return; // 부모가 Black이면 더블레드 발생x. 문제x. ->함수종료.
		else //부모가 RED면 더블레드 발생. (recolor이거나 restructure상황)
			insert_recoloring(node);
	}

	//부모  red
	void insert_recoloring(Node* node) {
		if (uncle(node) != NULL && uncle(node)->color == RED) { //부모와 삼촌노드 모두 RED면 
			//부모, 삼촌은 블랙, 할아버지는 레드로 색 바꿔줌. 
			node->par->color = BLACK;
			uncle(node)->color = BLACK;
			node->par->par->color = RED;

			//할아버지와 그의 부모에서 또 더블레드발생할 수 있으므로 할아버지 노드기준으로 다시 재구조. 
			insert_root(grand(node));
		}
		else
			insert_restructuring1(node);
	}

	void insert_restructuring1(Node* node) {
		if (node->par == grand(node)->left && node == node->par->right) {
			rotate_left(node->par);
			node = node->left;
		}
		else if (node->par == grand(node)->right && node == node->par->left) {
			rotate_right(node->par);
			node = node->right;
		}
		insert_restructuring2(node);

	}
	void insert_restructuring2(Node* node) {
		node->par->color = BLACK;
		grand(node)->color = RED; 
		if (node->par == grand(node)->left && node == node->par->left) {
			rotate_right(grand(node));
		}
		else  if (node->par == grand(node)->right && node == node->par->right) {
			rotate_left(grand(node));
		}
	}


	void rotate_left(Node* node) { //노드 좌회전
		if (node->par != NULL) {//회전하려는 노드의 부모가 존재할때 (루트가 아닌 노드를 회전하는 경우) 
			if (node == node->par->left)
				node->par->left = node->right;
			else  //오른쪽자식이면
				node->par->right = node->right;
			node->right->par = node->par;
			node->par = node->right;
			if (node->right->left != NULL)
				node->right->left->par = node;
			node->right = node->right->left;
			node->par->left = node;
		}
		else {//회전하려는 노드의 부모노드가 없을 경우(= 회전하려는 노드는 루트노드. =루트노드를 회전하는 경우) 
			Node* rootRight = root->right;
			rootRight->par = NULL;
			if (rootRight->left != NULL) {
				rootRight->left->par = root;
				root->right = rootRight->left;
			}
			root->par = rootRight; 
			rootRight->left = root; 
			root = rootRight; 
		}
	}
	void rotate_right(Node* node) { //노드 우회전 
		if (node->par != NULL) { //루트노드가 아닌 경우

			//자신의 부모노드와 자신의 왼쪽자식노드를 부모-자식관계로 연결. 
			if (node == node->par->left)  //회전하려는 노드가 왼쪽 자식노드인 경우
				node->par->left = node->left; //부모의 왼쪽 자식을 자신의 왼쪽 자식노드로 설정. 
			else//회전하려는 노드가 오른쪽 자식노드인 경우
				node->par->right = node->left; //부모의 오른쪽 자식노드를 자신의 왼쪽 자식노드로 설정.
			node->left->par = node->par;// 자신의 왼쪽 노드의 부모노드를 자신의 부모노드로 설정. 

			node->par = node->left;

			//자신의 왼쪽 자식노드의 오른쪽 자식노드가 존재한다면 자신의 왼쪽 자식으로 연결. 
			if (node->left->right != NULL) {
				node->left->right->par = node;  //자신->왼쪽->오른쪽 노드의 부모를 자신으로 설정. 
			}
			node->left = node->left->right;// 자신의 왼쪽 자식노드를 자신의 왼쪽의 오른쪽 자식노드로 설정.

			node->par->right = node; 
		}
		else {// 루트노드 우회전
			Node* rootLeft = root->left;
			rootLeft->par = NULL; 
			rootLeft->right->par = root; 
			root->left = rootLeft->right; 
			root->par = rootLeft; 
			rootLeft->right = root; 
			root = rootLeft; 
		}

	}


	void update(int id, string name, int size, int price) {
		if (search(id) == NULL) {
			cout << "NULL" << endl;
		}
		else {
			Node* node = search(id); 
			node->setName(name); 
			node->setSize(size);
			node->setPrice(price);
			cout << depth(id) << endl;
		}
	}
	void inorder(Node* node,int x, int y, int p) {
		if (node == NULL) return;
		inorder(node->left, x, y, p); 
		discount(node, x, y, p);
		inorder(node->right, x, y, p);
	}
	void discount(Node* node, int x, int y, int p) {
		if (node->id >= x && node->id <= y) {
			int curPrice = node->getPrice();
			int salePrice = curPrice * ((100 - p) / double(100));
			node->setPrice(salePrice);
			//cout << node->id << " " << curPrice << " " <<" " << salePrice << endl;
		}
	}
};
int main() {
	int N; //질의 수
	cin >> N;
	RBT rbt;
	string sign; // 질문, 검색, 업데이트, 할인
	int id, size, price;
	string name;
	while (N--) {//질의 수만큼 반복
		cin>>sign;
		if (sign == "I") { //앱 등록기능
			cin >> id>>name >> size >> price; 
			rbt.insert(id, name, size, price);
			cout << rbt.depth(id) << endl;
		}
		else if (sign == "F") { //앱 검색기능
			cin >> id;
			Node* node = rbt.search(id); //해당노드찾기
			if(node==NULL) //존재하지않으면 
				cout<<"NULL"<<endl;//NULL출력
			else {//존재하면 노드의 깊이, 이름, 용량, 가격 출력. 
				cout << rbt.depth(id) << " " << node->getName() << " " << node->getSize() << " " << node->getPrice() << endl;
			}
		}
		else if (sign == "R") { //앱 업데이트기능
			cin >> id >> name >> size >> price;
			rbt.update(id, name, size, price);
		}
		else if (sign == "D") {
			int x, y, P; 
			cin >> x >> y >> P; 
			rbt.inorder(rbt.root, x, y, P);
		}
	}
}