#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
#define MAX_NODE 100001
#define MAX_DEPTH 18
using namespace std;

vector<vector<int>>adj;
int parent[MAX_NODE][MAX_DEPTH];
int depth[MAX_NODE];
vector<int> result;

void make_tree_bfs() {
	int dep = 0;
	int Size;
	queue<int> bfs;

	bfs.push(1);
	while (!bfs.empty()) {
		Size = (int)bfs.size();
		while (Size-- > 0) {
			int node = bfs.front();
			bfs.pop();
			depth[node] = dep;
			for (int i = 0; i < (int)adj[node].size(); i++) {
				int new_node = adj[node][i];
				if (depth[new_node] == -1) {
					parent[new_node][0] = node;
					bfs.push(adj[node][i]);
				}
			}
		}
		dep++;
	}
}
void make_tree_dfs(int node) {
	for (int i = 0; i < (int)adj[node].size(); i++) {
		int new_node = adj[node][i];
		if (depth[new_node] == -1) {
			depth[new_node] = depth[node] + 1;
			parent[new_node][0] = node;
			make_tree_dfs(new_node);
		}
	}
}
void make_parent_table(int num) {
	for(int j = 0 ; j< MAX_DEPTH;j++)
		for (int i = 1; i <= num; i++) {
			if (parent[i][j] != -1) {
				parent[i][j + 1] = parent[parent[i][j]][j];
			}
		}
}
void swap(int &a, int&b) {
	int tmp;
	tmp = b;
	b = a;
	a = tmp;
}

int main(void) {
	int num,query;
	int node_a, node_b;

	scanf("%d", &num);
	adj.resize(num+1);
	for (int i = 0; i < (num - 1); i++) {
		scanf("%d %d", &node_a, &node_b);
		adj[node_a].push_back(node_b);
		adj[node_b].push_back(node_a);
	}

	memset(parent, -1, sizeof(parent));
	fill(depth, depth + num + 1, -1);
	make_tree_bfs();
	//make_tree_dfs(0);
	make_parent_table(num);

	scanf("%d", &query);
	while (query-- > 0) {
		scanf("%d %d", &node_a, &node_b);

		//make two node have same depth
		if (depth[node_a] < depth[node_b])
			swap(node_a, node_b);
		int diff_dep = depth[node_a] - depth[node_b];
		for (int i = 0; diff_dep>0; i++) {
			if (diff_dep % 2)
				node_a = parent[node_a][i];
			diff_dep /= 2;
		}

		//find LCA
		if (node_a != node_b) {
			for (int i = MAX_DEPTH - 1; i >= 0; i--) {
				int parent_a = parent[node_a][i];
				int parent_b = parent[node_b][i];
				if (parent_a != -1 && parent_a != parent_b) {
					node_a = parent_a;
					node_b = parent_b;
				}
			}
			node_a = parent[node_a][0];
			
		}
		result.push_back(node_a);
	}
	for (int i = 0; i < (int)result.size(); i++)
		printf("%d\n", result[i]);
}