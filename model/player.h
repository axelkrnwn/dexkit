#ifndef PLAYER
#define PLAYER

namespace Player{
	
	struct Node{
		char name[100];
		int trophy;
		int gold;
		int level;
		int height;
		int troopCapacity;
		int id;
		Node *left, *right;
	} *root;
	
	int getSize();
	
	Player::Node *createPlayer(char *name, int trophy, int gold, int level){
		Player::Node *newP = (Player::Node*)malloc(sizeof(Player::Node));
		
		strcpy(newP->name, name);
		newP->id = getSize() + 1;
		newP->trophy = trophy;
		newP->gold = gold;
		newP->level = level;
		newP->troopCapacity = 10;
		newP->left = newP->right = NULL;
		
		return newP;
	}
	
	int max(int a, int b){
		return a > b ? a : b;
	}
	
	int getHeight(Player::Node *node){
		if (!node){
			return 0;
		}
		return node->height;
	}
	
	int getBalanceFactor(Player::Node *root){
		return - getHeight(root->left) + getHeight(root->right);
	}
	
	int getSize(){
		Player::Node *curr = root;
		if (!curr){
			return 0;
		}
		while(curr->right){
			curr = curr->right;
		}
		return curr->id;
		
	}
	
	void pagination(Player::Node *root, int min, int max){
		if (!root){
			return;
		}
		Player::pagination(root->left,min,max);
		if (root->id >= min && root->id <= max){			
			gotoXY(30, root->id + 10 - min);
			printf("%-3d %s - %d\n", root->id, root->name,root->trophy);
		}
		Player::pagination(root->right, min,max);
	}
	
	void inOrder(Node *root){
		if (!root){
			return;
		}
		printf("%d\n", root->id);
		inOrder(root->left);
		inOrder(root->right);
	}
	
	Player::Node *search(Player::Node *root, int id){
		if (!root){
			if (id == 6)
			return root;
		}
		
		if (root->id == id){
			return root;
		}
		
		if (root->id < id){
			return search(root->right, id);
		}else if (root->id > id){
			return search(root->left, id);
		}
	}
	
	Player::Node *leftRotation(Player::Node *node){
		Player::Node *left = node->left;
		Player::Node *leftRight = left->right;
		
		left->right = node;
		node->left = leftRight;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		left->height = max(getHeight(left->left), getHeight(left->right)) + 1;
		return left;
	}

	Player::Node *rightRotation(Player::Node *node){
		Player::Node *right = node->right;
		Player::Node *rightLeft = right->left;
		
		right->left = node;
		node->right = rightLeft;
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		right->height = max(getHeight(right->left), getHeight(right->right)) + 1;
		return right;
	}

	Player::Node *balancing(Player::Node *node){
		
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		
		int bf = getBalanceFactor(node);
		
		if (bf < -1){
			int bfLeft = getBalanceFactor(node->left);
			if (bfLeft > 0){
				node->left = rightRotation(node->left);
				return leftRotation(node);
			}else if (bfLeft < 0){
				return leftRotation(node);
			}
		}
		if (bf > 1){
			int bfRight = getBalanceFactor(node->right);
			if (bfRight > 0){
				return rightRotation(node);
			}else if (bfRight < 0){
				node->right = leftRotation(node->right);
				return rightRotation(node);
			}
		}
		
		return node;
	}

	Player::Node *insert(Player::Node *root, Player::Node *inserted){
		if (!root){
			root = inserted;
		}else if (root->id < inserted->id){
			root->right = insert(root->right, inserted);
		}else if (root->id > inserted->id){
			root->left = insert(root->left, inserted);
		}
		return balancing(root);
	}

	void insertToFile(char name[], int trophy, int lvl){
		FILE *fp = fopen("./database/players.csv","a");
		fprintf(fp,"%s,%d,%d,%d\n", name, trophy,lvl,0);
		fclose(fp);
	}
	
	void readFromFile(){
		FILE *fp = fopen("./database/players.csv","r");
		
		if (fp == 0){
			return;
		}
		char name[1000];
		int trophy, lvl, gold;
		while(!feof(fp)){
			fscanf(fp,"%[^,],%d,%d,%d\n", name, &trophy,&lvl, &gold);
			Player::root = insert(Player::root, createPlayer(name,trophy,gold,lvl));
		}
		fclose(fp);
	}

	void saveProgress(){
		FILE *fp = fopen("./database/players.csv","w");
		int size = getSize();
		for (int i = 1;i <= size;i++){
			Player::Node *p = search(root, i);
			fprintf(fp,"%s,%d,%d,%d\n", p->name, p->trophy,p->level,p->gold);
		}
		fclose(fp);	
	}	
}

#endif
