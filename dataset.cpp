/*
* datasetmain.cpp:
Author(s):
 *   Muktikanta Sa   <cs15resch11012@iith.ac.in>
 *   
 * Description:
 *   Generation of datasets for input graph
 * Copyright (c) 2017.
 * last Updated: 31/10/2017
*/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

using namespace std;
int COUNT=7;
enum type {ADDV, ADDE, REMV, REME, CONV, CONE, APP};
unsigned int id=1000;// vertex ID
long int nOps; //
/* number of operations of each type*/
int naddV, naddE, nremV, nremE, nconV, nconE, nApp=0; 
//int n; // total number operations
FILE *fp; // file to write all the opeartions 
int optype; // what type of opearations 
char out[30]; // dataset file name,
int x;
void dataSet(int n){ 
  int u,v, flipdice;
  fp = fopen(out, "w");
  
    cout <<n<<endl;
    int i;
   for( i=1; i <= n; i++){
      optype = rand()% COUNT; /* flipdice value from 1 through type::count */
      // cout<<"optype:"<<optype<<endl;
      //                 cin>>x;
      switch(optype){
      
      case ADDV : if(naddV > 0){
                       u = id++; 
          //             cout<<"addv:"<<u<<endl;
          //             cin>>x;
                       fprintf(fp,"%d %d\n",ADDV, u); // print type of operation and value
                       naddV--;
                         
                }
                i--;
                break;
                
      case ADDE : if(naddE > 0){
               adde:   u = rand()%id+1;
                       v = rand()%id+2;   
                  //      cout<<"addE:"<<u<<" "<<v<<endl;
                  //     cin>>x;                      
                       if(u == v || u == 0 || v == 0) goto adde;
                       fprintf(fp,"%d %d %d\n",ADDE, u, v); // print type of operation and value
                       naddE--;
                       
                }
                i--;
                break;
        case REMV : if(nremV > 0){
             remv:     u = rand()%id+1 ; 
             // cout<<"remv:"<<u<<endl;
              //         cin>>x;
                        if(u == 0) goto remv;
                       fprintf(fp,"%d %d\n",REMV, u); // print type of operation and value
                       nremV--;
                         
                }
                i--;
                break;
     case REME : if(nremE > 0){
               reme:   u = rand()%id+1 ;
                       v = rand()%id+2 ;  
                 //       cout<<"remE:"<<u<<" "<<v<<endl;
                 //      cin>>x;                       
                       if(u == v || u == 0 || v == 0) goto reme;
                       fprintf(fp,"%d %d %d\n",REME, u, v); // print type of operation and value
                       nremE--;
                         
                }
                i--;
                break;                              
        case CONV : if(nconV > 0){
             conv:     u = rand()%id+1; 
             // cout<<"conv:"<<u<<endl;
              //         cin>>x;
                        if(u == 0) goto conv;
                       fprintf(fp,"%d %d\n",CONV, u); // print type of operation and value
                       nconV--;
                         
                }
                i--;
                break;
     case CONE : if(nconE > 0){
               cone:   u = rand()%id+1 ;
                       v = rand()%id +2;       
                  //      cout<<"cone:"<<u<<" "<<v<<endl;
                 //      cin>>x;                  
                       if(u == v || u == 0 || v == 0) goto cone;
                       fprintf(fp,"%d %d %d\n",CONE, u, v); // print type of operation and value
                       nconE--;
                         
                }
                i--;
                break;    
                    
        default: break;                                          
      }
    
   }
   cout<<i<<endl;
   fclose(fp);


}

int main(int argc, char*argv[])	{

        int n;
	if(argc < 6){
		cout << "Enter total#operations, %%ofaddV, %%ofaddE, %%ofremV, %%ofremE, %%ofconV, %%ofconE and %%ofApp" << endl;
		return 0;
	}
	n = atoi(argv[1]);
	naddV = (int)(atof(argv[2])); 
	naddE = (int)(atof(argv[3]));
	nremV = (int)(atof(argv[4])); 
	nremE = (int)(atof(argv[5])); 
	nconV = (int)(atof(argv[6])); 
	nconE = (int)(atof(argv[7])); 
	// nApp = (int)(atof(argv[8])*n*(0.1)); 
	cout<<naddV<<" "<<naddE<<" "<<nremV<<" "<<nremE<<" "<<nconV<<" "<<nconE<<" "<<endl;
        if((n) != (naddV + naddE + nremV + nremE + nconV + nconE)){
                cout << "total#operations and %% of operatations are not matching" << endl;
                cout<<"Please try again !!!"<<endl;
		return 0;
	}
	strcpy(out, argv[8]);
	dataSet(n);
       	return 0;
}