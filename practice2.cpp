#include<iostream>
#define BLACK 0
#define RED 1
using namespace std;
class Node {// ���ø����̼� ����
private:
	int id; //ID (���� key) 
	string name;//�̸�
	int size;//�뷮
	int price;//����
	Node* par;  //�θ��� ����Ű�� ������
	Node* left; //���� �ڽĳ�� ����Ű�� ������
	Node* right;//������ �ڽĳ�� ����Ű�� ������
	int color;

public:
	
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
class RBT { //�����Ʈ���� �̿��� ��ɱ���
public:
	Node* root;
	RBT() {
		this->root = NULL;
	}

	Node* search(int id) { //�˻����
		// :id�� ���� �˻�->��Ʈ���� �������鼭 ã��->�����ϸ� ����, �̸�, �뷮, ���� ���/ �������������� NULL����ε�
		// �ϴ� �� �Լ��� ���ؼ��� �����ϸ� �ش��带 �����ϰ� �������������� NULL�� ����ϵ��� ��.  
		Node* curN = this->root; //������ ��Ʈ���� ����. 
		while (curN != NULL) {
			if (curN->id == id) //�������� ID�� ã�� ID���
				return curN; //������ ����.
			else if (id > curN->id) curN = curN->right; //ã�� ID�� �������� ID���� ũ�ٸ� �������� ������ ���� �̵�. 
			else curN = curN->left; //ã�� ID�� �������� ID���� �۴ٸ� �������� ���� ���� �̵�. 
		}
		//��� �������� leaf��带 ������ curN�� NULL�̵Ǿ� while���� Ż���ϰ� ��.
		// -> ã�� ID�� �������������Ƿ� NULL����. 
		return NULL;
	}

	int depth(int id) { //Ư�� ID�� ���� ����� ���̸� �˾Ƴ��� �޼���
		Node* node = this->search(id); //Ư�� ID�� ���� ��带 ������. 
		int dep = 0;
		while (node != root) { //��尡 ��Ʈ�� ���������� 
			node = node->par; //��ĭ�� �ö󰡸鼭 
			dep++; //1�� ������Ŵ. 
		}
		return dep;
	}

	void insert(int id, string name, int size, int price) {
		Node* node = new Node(id, name, size, price); //�ش� ���� ���� �� ��� ����
		//�� Ʈ���� ���
		if (this->root == NULL) {
			this->root = node; //��Ʈ�� �� ��� �����ϰ� 
			root->color = BLACK;
			return; //�Լ�����
		}

		//������ ��ġ ã�� 
		Node* parN = NULL;
		Node* curN = this->root; //������ ��Ʈ���� ����. 


		//1. ���翩��Ȯ��+������ ��ġ ã�� 
		while (curN != NULL) {
			if (curN->id == id) { //�����Ϸ��� id�� �̹� �ִٸ� (����Ϸ��� ���� �̹� ��ϵǾ��ִٸ�) 
				return; //��Ͼ��ϹǷ� �Լ�����. 
			}
			else if (curN->id < id) { //�������� ID�� ������ ID���� ���� ��� -> �������ڽ����� �̵�
				parN = curN;  //�θ��带 ������� �ٲٰ�
				curN = curN->right; //�����带 �������ڽ����� �ٲ�.
			}
			else { //�������� ID�� ������ ID���� ū ���-> �����ڽ����� �̵�
				parN = curN;  //�θ���� ������� �ٲٰ�
				curN = curN->left;  //�����带 �����ڽ����� �ٲ�. 
			}
		}
		//�̹� �����ϴ� ��찡 �ƴ϶�� leaf������ ������. 
		//������ curN�� NULL�� �Ǿ� while���� Ż���ϰ� ��. 
		//�θ����� �� leaf�ڽĳ���� �ϳ��� ������ ��ġ. 

		//2. ã�� ��ġ�� ������. 
		if (id > parN->id) //�θ����� ID���� ũ�� 
			parN->setRight(node); //������ �ڽĿ� �ְ�
		else if (id < parN->id) //�θ����� ID���� ������ 
			parN->setLeft(node); //���� �ڽĿ� ����. 
		
		if (node->par->color == RED) {
			manageRBT(node); //�� �Լ��� ������ �籸����������ġ�� ��. 
		}
	
	}
	void manageRBT(Node* node) {
		while (node->par!=NULL&&node->par->color ==  RED) { //�θ���RED
			Node* uncleN;
			if (node->par == node->par->par->left) {//�θ� ���� �ڽĳ���ΰ��.
				uncleN = node->par->par->right;
				if (uncleN != NULL && uncleN->color == RED) { //�θ��� RED, ���� ��� RED=> recoloring
					//����, �θ�� ��, ���θ�� ����� �� �ٲ���. 
					node->par->color = BLACK;
					uncleN->color = BLACK;
					node->par->par->color = RED;

					//��� ������ ������ �ö󰡸鼭 ������ϹǷ� �Ҿƹ����� �������� �ٲ����. 
					node = node->par->par;
					continue; //while�� ���� ��. 
				}
				if (node == node->par->right) { //�θ� RED, ���� BLACK, node�� ������ �ڽĳ���ϰ��
					//�θ���� ��ȸ�����ְ� 
					node = node->par;
					rotate_left(node);
				}
				//�� �ٲ��� �� �Ҿƹ����������� ��ȸ�� �ѹ� �ؾ���. 
			    //�θ� RED, ������ BLACK�� �ڽ��� �����ڽ��� ��쿡�� 
				//�� �θ���� ��ȸ���� ���ϰ� �Ʒ� ���ٲٰ� �Ҿƹ����������� ��ȸ�� ���ָ� ��. 

				node->par->color = BLACK;
				node->par->par->color = RED;
				

				rotate_right(node->par->par);
			}
			else { //�θ� ������ �ڽĳ���� ��� (�� ���� �ݴ�� �̷����) 
				uncleN = node->par->par->left; //���̳��� ���� �ڽĳ��. 
				if (uncleN != NULL && uncleN->color == RED) { //�θ�� ���� ��� RED�϶� => recoloring. 
					node->par->color = BLACK; 
					uncleN->color = BLACK; 
					node->par->par->color = RED; 
					node = node->par->par; 
					continue;
				}
				if (node == node->par->left) {
					node = node->par;
					rotate_right(node);
				}
				node->par->color = BLACK; 
				node->par->par->color = RED;

				rotate_left(node->par->par);
			}
		}
		root->color = BLACK; //�ٽ� Ȯ���ʿ�. 
	}


	void rotate_left(Node* node) { //��� ��ȸ��
		if (node->par != NULL) {//ȸ���Ϸ��� ����� �θ� �����Ҷ� (��Ʈ�� �ƴ� ��带 ȸ���ϴ� ���) 
			if (node == node->par->left)
				node->par->left = node->right;
			else  //�������ڽ��̸�
				node->par->right = node->right;
			node->right->par = node->par;
			node->par = node->right;
			if (node->right->left != NULL)
				node->right->left->par = node;
			node->right = node->right->left;
			node->par->left = node;
		}
		else {//ȸ���Ϸ��� ����� �θ��尡 ���� ���(= ȸ���Ϸ��� ���� ��Ʈ���. =��Ʈ��带 ȸ���ϴ� ���) 
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
	void rotate_right(Node* node) { //��� ��ȸ�� 
		if (node->par != NULL) { //��Ʈ��尡 �ƴ� ���

			//�ڽ��� �θ���� �ڽ��� �����ڽĳ�带 �θ�-�ڽİ���� ����. 
			if (node == node->par->left)  //ȸ���Ϸ��� ��尡 ���� �ڽĳ���� ���
				node->par->left = node->left; //�θ��� ���� �ڽ��� �ڽ��� ���� �ڽĳ��� ����. 
			else//ȸ���Ϸ��� ��尡 ������ �ڽĳ���� ���
				node->par->right = node->left; //�θ��� ������ �ڽĳ�带 �ڽ��� ���� �ڽĳ��� ����.
			node->left->par = node->par;// �ڽ��� ���� ����� �θ��带 �ڽ��� �θ���� ����. 

			node->par = node->left;

			//�ڽ��� ���� �ڽĳ���� ������ �ڽĳ�尡 �����Ѵٸ� �ڽ��� ���� �ڽ����� ����. 
			if (node->left->right != NULL) {
				node->left->right->par = node;  //�ڽ�->����->������ ����� �θ� �ڽ����� ����. 
			}
			node->left = node->left->right;// �ڽ��� ���� �ڽĳ�带 �ڽ��� ������ ������ �ڽĳ��� ����.

			node->par->right = node;
		}
		else {// ��Ʈ��� ��ȸ��
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
	void inorder(Node* node, int x, int y, int p) {
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
	int N; //���� ��
	cin >> N;
	RBT rbt;
	string sign; // ����, �˻�, ������Ʈ, ����
	int id, size, price;
	string name;
	while (N--) {//���� ����ŭ �ݺ�
		cin >> sign;
		if (sign == "I") { //�� ��ϱ��
			cin >> id >> name >> size >> price;
			rbt.insert(id, name, size, price);
			cout << rbt.depth(id) << endl;
		}
		else if (sign == "F") { //�� �˻����
			cin >> id;
			Node* node = rbt.search(id); //�ش���ã��
			if (node == NULL) //�������������� 
				cout << "NULL" << endl;//NULL���
			else {//�����ϸ� ����� ����, �̸�, �뷮, ���� ���. 
				cout << rbt.depth(id) << " " << node->getName() << " " << node->getSize() << " " << node->getPrice() << endl;
			}
		}
		else if (sign == "R") { //�� ������Ʈ���
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