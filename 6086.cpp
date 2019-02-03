#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
#define standard 65
using namespace std;

typedef struct edge {
	int to, capacity, flow;
	edge* dual;
	edge *same;
	edge() :edge(-1, 0){}
	edge(int t, int c): to(t),capacity(c),flow(0),dual(nullptr),same(nullptr){}
	int remain() {
		return capacity - flow;
	}
	void add(int f) {
		flow += f;
		dual->flow -= f;
		if (same != nullptr)
			same->capacity -= f;
		else
			dual->same->capacity += f;
	}
};

vector<vector<edge*>>network; //network[i] stores edge pointers that starts from 'i' vertex.
int pre[58];
edge *path[58];

void make_two_way_pipe(int ver1, int ver2, int cap) {
	edge *e1, *e2;
	edge *e1_dual, *e2_dual;
	
	e1 = new edge(ver2, cap);
	e1_dual = new edge(ver1, 0);
	e2 = new edge(ver1, cap);
	e2_dual = new edge(ver2, 0);
	e1->dual = e1_dual;
	e1_dual->dual = e1;
	e1->same = e2;
	e2->dual = e2_dual;
	e2->same = e1;
	e2_dual->dual = e2;
	network[ver1].push_back(e1);
	network[ver1].push_back(e2_dual);
	network[ver2].push_back(e2);
	network[ver2].push_back(e1_dual);
}

void initialize(int e_num) {
	network.resize(58); // 58 = 'z'-'A' + 1
	char ch1, ch2;
	int ver1, ver2;
	int cap;
	for (int i = 0; i < e_num; i++) {
		scanf("%c %c %d%*c", &ch1, &ch2, &cap);
		ver1 = (int)ch1 - standard;
		ver2 = (int)ch2 - standard;
		make_two_way_pipe(ver1, ver2, cap);
	}
}

bool find_path_bfs(int source, int sink) {
	queue<int> bfs;
	fill(pre, pre + 58,-1);
	pre[source] = source;

	bfs.push(source);
	while (!bfs.empty() && pre[sink]==-1) {
		int cur = bfs.front();
		bfs.pop();
		for (int i = 0; i < (int)network[cur].size(); i++) {
			edge* next_e = network[cur][i];
			int next = next_e->to;
			if (pre[next] == -1 && next_e->remain() > 0) {
				pre[next] = cur;
				path[next] = next_e;
				bfs.push(next);
			}
		}
	}
	return pre[sink] != -1;
}
int augment(int source, int sink) {
	int possible_flow = 1001; //max value
	for (int cur = sink; cur != source; cur = pre[cur]) {
		if (possible_flow > path[cur]->remain())
			possible_flow = path[cur]->remain();
	}
	for (int cur = sink; cur != source; cur = pre[cur]) {
		path[cur]->add(possible_flow);
	}
	return possible_flow;
}

int Ford_Fulkerson_algo() {
	int source = 0;
	int sink = 'Z' - 'A';
	int max_flow = 0;
	while (find_path_bfs(source, sink)) {
		max_flow+=augment(source, sink);
	}
	return max_flow;
}
int main(void) {
	int e_num;
	scanf("%d%*c", &e_num);
	initialize(e_num);
	printf("%d\n", Ford_Fulkerson_algo());
}