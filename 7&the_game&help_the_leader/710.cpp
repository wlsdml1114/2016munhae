#include <iostream>
#include <string>
#include <queue>
#include <stdio.h>
#define vb vector<bool>
using namespace std;
int n,m;
int res[77][77];
char arr[77][77];
int ox[4] = {0,1,0,-1};
int oy[4] = {1,0,-1,0};

// Represents the direction of the path.
enum Change {
  HOR,
  VER,
  NONE,  // Initially the path has no direction.
};

struct Node {
  int x;
  int y;
  int step;
  Change change;

  Node(int _x, int _y, int _step, Change _c)
    : x(_x), y(_y), step(_step), change(_c) {}
};


int visit(int x1, int y1, int x2, int y2, int w, int h) {
  queue<Node> q;
  q.push({x1, y1, 0, NONE});

  vector<vb> visited(h + 10, vb(w + 10));
  visited[y1][x1] = true;

  while (!q.empty()) {
    Node& top = q.front();
    for (int i = 0; i < 4; ++i) {
      int nx = top.x + ox[i];
      int ny = top.y + oy[i];
      Change nc = ox[i] != 0 ? HOR : VER;
      if (ny == y2 && nx == x2)
        return top.step + (top.change != nc ? 1 : 0);

      if (nx < 0 || ny < 0 || nx > w + 1 || ny > h + 1 ||
          visited[ny][nx] || arr[ny][nx] != ' ') {
        continue;
      }

      visited[ny][nx] = true;
      q.push({
        nx, ny,
        // We count changes in the direction of the
        // segment from source to destination:
        //
        // None -> Vertical
        // None -> Horizontal
        // Vertical -> Horizontal
        // Horizontal -> Vertical
        top.step + (top.change != nc ? 1 : 0),
        nc});
    }

    q.pop();
  }

  return -1;
}


int bfs(int x1, int y1, int x2, int y2, int w, int h) {
  queue<Node> q;
  q.push({x1, y1, 0, NONE});

  vector<vb> visited(h + 10, vb(w + 10));
  visited[y1][x1] = true;

  while (!q.empty()) {
    Node& top = q.front();
    for (int i = 0; i < 4; ++i) {
      int nx = top.x + ox[i];
      int ny = top.y + oy[i];
      Change nc = ox[i] != 0 ? HOR : VER;
      if (ny == y2 && nx == x2)
        return top.step + (top.change != nc ? 1 : 0);

      if (nx < 0 || ny < 0 || nx > w + 1 || ny > h + 1 ||
          visited[ny][nx] || arr[ny][nx] != ' ') {
        continue;
      }

      visited[ny][nx] = true;
      q.push({
        nx, ny,
        // We count changes in the direction of the
        // segment from source to destination:
        //
        // None -> Vertical
        // None -> Horizontal
        // Vertical -> Horizontal
        // Horizontal -> Vertical
        top.step + (top.change != nc ? 1 : 0),
        nc});
    }

    q.pop();
  }

  return -1;
}
// typedef struct Node{
// 	int x;
// 	int y;
// 	int step;
// 	int style;
// }Node;
int main(){
	int board=1;
	string s;
	while(true){//all loop
		cin>>n>>m;
		// cout<<n<<m<<endl;
		if(n==0&&m==0)break;


		cout<<"Board #"<<board++<<":"<<endl;


		getline(cin,s);
		for(int i=1;i<=m;++i){//i is column
			getline(cin,s);
			for (int j=1;j<=n;++j){//j is row
				arr[i][j] = s[j-1];
				res[i][j]=0;
			}
		}

		for(int i=0;i<=m+1;++i){
			arr[i][0]=' ';
			arr[i][n+1]=' ';
		}
		for(int i=0;i<=n+1;++i){
			arr[0][i]=' ';
			arr[m+1][i]=' ';
		}

		// for(int i=0;i<=m+1;++i){//i is column
		// 	for (int j=0;j<=n+1;++j){//j is row
		// 		cout<<arr[i][j];
		// 	}
		// 	cout<<endl;
		// }

		int num = 0;
		while(true){
			int x1,x2,y1,y2;
			cin>>x1>>y1>>x2>>y2;
			// cout<<x1<<y1<<x2<<y2<<endl;
			if(x1==0&&y1==0&&x2==0&&y2==0)break;
			int ret = bfs(x1,y1,x2,y2,n,m);

		      if (ret != -1)
		        printf("Pair %d: %d segments.\n", ++num, ret);
		      else
		        printf("Pair %d: impossible.\n", ++num);
		}
		cout<<endl;

	}
	return 0;
}