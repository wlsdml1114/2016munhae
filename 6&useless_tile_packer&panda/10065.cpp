#include <iostream>
// #include <stdio.h>

using namespace std;

struct vertex {
    int x, y;
};

int absolute(int a){
	return (a>0)?a:-a;
}
double fabsolute(float a){
	return (a>0)?a:-a;	
}
double angle(vertex a, vertex b) {
    	int dx, dy, ax, ay;
    	dx = b.x - a.x;   
    	ax = absolute(dx);
    	dy = b.y - a.y;   
    	ay = absolute(dy);
    	// dx = b.x - a.x;   
    	// ax = absolute(dx);
    	// dy = b.y - a.y;   
    	// ay = absolute(dy);
    	double t;
    	if (dx==0 && dy==0) t = 0;
    	else t = (double)dy/(ax + ay);
    	if (dx<0) t = 2-t;
    	else if (dy<0) t += 4;
    	return t*90.0;
}

double Area(vertex v[], int n) {
    	v[n+1] = v[1];
    	double area = 0;
    	for (int i=1; i<=n; i++)
        	area += v[i].x*v[i+1].y - v[i+1].x*v[i].y;
    	// for (int i=1; i<=n; i++)
     //    	area += v[i].x*v[i+1].y - v[i+1].x*v[i].y;
    	return fabsolute(area/2.0);
}

int main() {
	vertex v[105];
    	int n, m, i, u, cas=0;
    	while (cin >> n && n) {
        	for (i=1; i<=n; i++){
            	// scanf("%d %d", &v[i].x, &v[i].y);
        		cin>>v[i].x;
        		cin>>v[i].y;
        	}
        	double area1 = Area(v, n);
        
        	double minangle, ver; 
        	vertex t;
        	for (i=1, u=1; i<=n; i++)
            	if (v[i].y < v[u].y || (v[i].y==v[u].y && v[i].x>v[u].x)) u = i;
        	v[n+1] = v[u]; 
        	m = 0; 
        	minangle = 0;
        	do {
            	m++;
            	t = v[m]; 
            	v[m] = v[u]; 
            	// for (i=m+1; i<=n+1; i++) {
             //   	double tempa = angle(v[m], v[i]);
             //    	if (tempa<minangle && tempa>ver) {
             //        	u = i; 
             //        	minangle = tempa;
             //    	}
            	v[u] = t;
            	ver = minangle; 
            	minangle = 360.0; 
            	u = n+1;
            	for (i=m+1; i<=n+1; i++) {
               	double tempa = angle(v[m], v[i]);
                	if (tempa<minangle && tempa>ver) {
                    	u = i; 
                    	minangle = tempa;
                	}
            	}
        	} while (u!=n+1);
        	double area2 = Area(v, m);
        	cout<<"Tile #"<< ++cas<<endl;
        	cout<<fixed;
        	cout.precision(2);
        	cout.setf(ios::showpoint);
        	cout<<"Wasted Space = "<< ((area2-area1)*100.0/area2)<<" %\n\n";
        	// printf("Wasted Space = %.2lf %%\n\n", (area2-area1)*100.0/area2);
    	}
}