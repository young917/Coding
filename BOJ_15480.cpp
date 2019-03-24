#include <cstdio>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#define MAX_NUM 100001
#define MAX_DEP 18
using namespace std;

vector<vector<int>> adj;
int parent[MAX_NUM][MAX_DEP];
int depth[MAX_NUM];
vector<int>answers;

void initialize(int num) {
	fill(depth, depth + MAX_NUM, -1);
	for (int i = 1; i <= num; i++)
		fill(parent[i], parent[i] + MAX_DEP, -1);
}
void swap(int &a, int &b) {
	int temp;
	temp = b;
	b = a;
	a = temp;
}
void make_tree(int root) {
	// BFS
	queue<int>bfs;
	depth[root] = 0;
	bfs.push(root);
	while (!bfs.empty()) {
		int num_same_dep = (int)bfs.size();
		while (num_same_dep-- > 0) {
			int ver = bfs.front();
			bfs.pop();
			for (int i = 0; i < (int)adj[ver].size(); i++) {
				int next = adj[ver][i];
				//If it is child of ver
				if (depth[next] == -1) {
					depth[next] = depth[ver] + 1;
					parent[next][0] = ver;
					bfs.push(next);
				}
			}
		}
	}
}
void make_parent_table(int num) {
	for(int d=0;d<MAX_DEP;d++)
		for (int v = 1; v <= num; v++) {
			if (parent[v][d] != -1)
				parent[v][d + 1] = parent[parent[v][d]][d];
		}
}
void make_same_depth(int &u, int &v) {
	if (depth[u] < depth[v])
		swap(u, v);
	int diff = depth[u] - depth[v];
	for (int i = 0; diff > 0; i++) {
		if (diff % 2)
			u = parent[u][i];
		diff /= 2;
	}
}
int find_LCA(int u, int v) {
	int a, b;
	a = u;
	b = v;
	make_same_depth(a,b);
	if (a != b) {
		for (int anc = MAX_DEP-1; anc >= 0; anc--) {
			if (parent[a][anc] != parent[b][anc]) {
				a = parent[a][anc];
				b = parent[b][anc];
			}
		}
		a = parent[a][0];
	}
	return a;
}
void find_answer(int root, int nroot, int u, int v) {
	int LCA,LCA1, LCA2;
	LCA = find_LCA(u, v);
	LCA1 = find_LCA(u, nroot);
	LCA2 = find_LCA(v, nroot);
	if (depth[LCA] > depth[LCA1]) {
		answers.push_back(LCA);
	}
	else if (LCA1 == LCA) {
		answers.push_back(LCA2);
	}
	else {
		answers.push_back(LCA1);
	}
}
int main(void) {
	int ver_num;
	int r, u, v;
	scanf("%d", &ver_num);
	adj.resize(ver_num + 1);
	for (int i = 0; i < (ver_num - 1); i++) {
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int root = 1;
	initialize(ver_num);
	make_tree(root);
	make_parent_table(ver_num);

	int query_num;
	scanf("%d", &query_num);
	while (query_num-- > 0) {
		scanf("%d %d %d", &r, &u, &v);
		find_answer(root, r, u, v);
	}
	for (int i = 0; i < (int)answers.size(); i++)
		printf("%d\n", answers[i]);
	return 0;
}