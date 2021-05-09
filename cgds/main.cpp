#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <pthread.h>
#include<unordered_set>


//#include"acyclic_lf.cpp"
#include"cgds_lf.cpp"

using namespace std;

typedef struct 
{
    int     secs;
    int     usecs;
}TIME_DIFF;

TIME_DIFF * my_difftime (struct timeval * start, struct timeval * end)
{
	TIME_DIFF * diff = (TIME_DIFF *) malloc ( sizeof (TIME_DIFF) );
 
	if (start->tv_sec == end->tv_sec) 
	{
        	diff->secs = 0;
        	diff->usecs = end->tv_usec - start->tv_usec;
    	}
   	else 
	{
        	diff->usecs = 1000000 - start->tv_usec;
        	diff->secs = end->tv_sec - (start->tv_sec + 1);
        	diff->usecs += end->tv_usec;
        	if (diff->usecs >= 1000000) 
		{
        	    diff->usecs -= 1000000;
	            diff->secs += 1;
	        }
	}
        return diff;
}

struct oper{
	int type;
	int u;
	int v=-1;
};

oper* input;
enum type {ADDV, ADDE, REMV, REME, CONV, CONE};

void loadInput(int n){
	input = new oper[n];
	ifstream in;
  	in.open("input.txt");
	// int i=0;
	for(int i=0;i<n;i++){
		int type;
		in >> type;
		in >> input[i].u;
		input[i].type=type;
		if( input[i].type == ADDE || input[i].type==REME || input[i].type==CONE)
			in >> input[i].v;
		// cout<<input[i].type<<"\t"<<input[i].u <<"\t"<<input[i].v<<endl;
	}
	in.close();
}

void initGraph(Graph* g) {
	ifstream inGraph("../initGraphs/initGraph_2^12.txt");
	unordered_set<int> vertices;
	int u, v, numEdges = 0;
	while(inGraph >> u >> v) {
		vertices.insert(u);
		vertices.insert(v);
	}
	inGraph.clear();
	inGraph.seekg(0);
	for(int vertex : vertices)
		g->AddV(vertex);
	while(inGraph >> u >> v) {
		if(g->AddE(u, v))
			numEdges++;
	}
	cout << "Initial graph with " << vertices.size() << " vertices  and " << numEdges << " edges created." << endl;
}

FILE *fp;

double seconds;
struct timeval tv1, tv2;
TIME_DIFF * difference;
int NTHREADS, ops;
int  total = 0, total1 = 0;
// enum type {ADDV, ADDE, REMV, REME, CONV, CONE};
char out[30]; // dataset file name,
int naddV=0, naddE=0, nremV=0, nremE=0, nconV=0, nconE=0; 
// pthread_mutex_t lock;																																		

typedef struct infothread{
  long tid;
  int tcount;
  long totalops;
  Graph G;
}tinfo;


void* pthread_call(void* t)
{
        tinfo *ti=(tinfo*)t;
        long Tid = ti->tid;
		int tcount=ti->tcount;
		int totalops=ti->totalops;
        Graph G1 = ti->G;
	int type, res;
 
 	// long long int numOfOperations = 10000000000;
 	// long long int numOfOperations_addEdge = numOfOperations * 0.25; 		// 25% for add edge
  	// long long int numOfOperations_addVertex = numOfOperations * 0.25; 	// 25% for add vertex
  	// long long int numOfOperations_removeVertex = numOfOperations *0.1 ; 	// 10% for remove vertex
  	// long long int numOfOperations_removeEdge = numOfOperations * 0.1; 	// 10% for remove edge
  	// long long int numOfOperations_containsVertex = numOfOperations *0.15; 	// 15% f																														or contains vertex
  	// long long int numOfOperations_containsEdge = numOfOperations * 0.15; 	// 15% for contains edge

	// long long int total = numOfOperations_addEdge + numOfOperations_addVertex + numOfOperations_removeVertex + numOfOperations_removeEdge + numOfOperations_containsVertex + numOfOperations_containsEdge; 
	
	for(int i=Tid;i<totalops;i+=tcount)
	{
		// gettimeofday(&tv2,NULL);input
		// difference = my_difftime (&tv1, &tv2);

		// if(difference->secs >= seconds)
		// 	break;

		int type=input[i].type;	
	    if(type == ADDE) 
		{
			res = G1.AddE(input[i].u,input[i].v); 			
      	}
		else if(type == ADDV)
		{
			res = G1.AddV(input[i].u);
		} 
		else if(type == REMV)
		{
			res = G1.RemoveV(input[i].u);
		}
		else if(type == REME)
		{
			res = G1.RemoveE(input[i].u,input[i].v); 
		}
		else if(type == CONV)
		{
			res = G1.ContainsV(input[i].u); 		
		}
		else if(type == CONE)
		{
			res = G1.ContainsE(input[i].u,input[i].v); 
		}
		
	} 		//end of while loop
}

int main(int argc, char*argv[])
{
	int i;
    Graph sg;
	if(argc < 2)
	{
		cout << "Enter 2 command line arguments - #threads, total #operations" << endl;
		return 0;
	}
	
	NTHREADS = atoi(argv[1]);
	int totalops = atoi(argv[2]);
	// number of operations each thread going to perform 1k,10k,50k,100k,1k^2

	loadInput(totalops);	

	
 	//ops = 0;
//strcpy(out,argv[4]);
	
	//create initial vertices
	initGraph(&sg);
	cout << "Number of Threads: " << NTHREADS << endl;
	pthread_t *thr = new pthread_t[NTHREADS];
	// Make threads Joinable for sure.
    pthread_attr_t attr;
   	pthread_attr_init (&attr);
   	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_JOINABLE);
	auto start = chrono::system_clock::now();
	time_t reqEnterTime = chrono::system_clock::to_time_t(start);
	cout << "timer started . . ." << endl;
	for (i=0;i < NTHREADS;i++)
	{
		tinfo *t =(tinfo*) malloc(sizeof(tinfo));
		t->tid = i;
		t->G = sg;
		t->tcount=NTHREADS;
		t->totalops=totalops;
		pthread_create(&thr[i], &attr, pthread_call, (void*)t);
	}

	for (i = 0; i < NTHREADS; i++)
    	{
		pthread_join(thr[i], NULL);
	}
    	
	auto end = chrono::system_clock::now();
	time_t actEnterTime=chrono::system_clock::to_time_t(end);
	auto elapsed=chrono::duration_cast<chrono::microseconds>(end - start);
	cout << elapsed <<  " seconds elapsed" << endl;
//sg.PrintGraph();
/*
  bool cycle = sg.checkCycle();
    if(cycle == true)
      cout<<"cycle is present"<<endl;
    else
      cout<<"cycle is not present"<<endl;  
 */     
 	return 0;
}
