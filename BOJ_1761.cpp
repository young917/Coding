#include <cstdio>
#include <vector>
#include <algorithm>
#define MAX_DEPTH 17
#define MAX_NODE 40001
using namespace std;


struct node_info{
	int idx;
	long long dist;
	node_info() :idx(-1), dist(0) {}
};

vector<vector<node_info>>adj;
int depth[MAX_NODE];
node_info parent[MAX_NODE][MAX_DEPTH];
vector<long long> answer;

void make_tree(int node) {
	for (int i = 0; i < (int)adj[node].size(); i++) {
		int next = adj[node][i].idx;
		long long d = adj[node][i].dist;
		if (depth[next] == -1) {
			depth[next] = depth[node] + 1;
			parent[next][0].idx = node;
			parent[next][0].dist = d;
			make_tree(next);
		}
	}
}
void make_parent_table(int num) {
	for(int j=0;j<(MAX_DEPTH-1);j++)
		for (int i = 1; i < num; i++) {
			if (parent[i][j].idx != -1) {
				parent[i][j + 1].idx = parent[parent[i][j].idx][j].idx;
				parent[i][j + 1].dist = parent[parent[i][j].idx][j].dist + parent[i][j].dist;
			}
		}
}
void swap(int &a, int &b) {
	int tmp;
	tmp = b;
	b = a;
	a = tmp;
}

int main(void) {
	int num;
	int node_a, node_b;
	long long d;

	scanf("%d", &num);
	adj.resize(num + 1);
	for(int i=1;i<=num;i++)
		for (int j = 0; j < MAX_DEPTH;j++) {
			parent[i][j].idx = -1;
			parent[i][j].dist = 0;
		}
	fill(depth, depth + num + 1, -1);

	for (int i = 0; i < (num - 1); i++) {
		scanf("%d %d %lld", &node_a, &node_b,&d);
		node_info tmp;
		tmp.idx = node_b;
		tmp.dist = d;
		adj[node_a].push_back(tmp);
		tmp.idx = node_a;
		adj[node_b].push_back(tmp);
	}

	make_tree(1);
	make_parent_table(num);

	int query;
	scanf("%d", &query);

	while (query-- > 0) {
		long long res = 0;
		scanf("%d %d", &node_a, &node_b);
		if (depth[node_a] < depth[node_b])
			swap(node_a, node_b);
		int diff_dep = depth[node_a] - depth[node_b];
		for (int i = 0; diff_dep > 0; i++) {
			if (diff_dep % 2) {
				res += parent[node_a][i].dist;
				node_a = parent[node_a][i].idx;
			}
			diff_dep /= 2;
		}

		if (node_a != node_b) {
			for (int j = MAX_DEPTH - 1; j >= 0; j--) {
				if ( (parent[node_a][j].idx != -1) && (parent[node_a][j].idx != parent[node_b][j].idx)) {
					res += parent[node_a][j].dist;
					res += parent[node_b][j].dist;
					node_a = parent[node_a][j].idx;
					node_b = parent[node_b][j].idx;
				}
			}
			res += parent[node_a][0].dist;
			res += parent[node_b][0].dist;
		}
		answer.push_back(res);
	}
	for (int i = 0; i < (int)answer.size(); i++)
		printf("%lld\n", answer[i]);
}