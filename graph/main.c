/*
 * main.c
 *
 *  Created on: Dec 25, 2017
 *      Author: yas
 */


#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

enum {WHITE,GRAY,BLACK};

typedef struct edge{
    int weight;
    struct edge *next;
    struct vertex *dst;
}edge_t;

typedef struct vertex{
    int key,color;
    int time_discovery,completed_time,distance;
    struct vertex *next;
    struct vertex *pred;

    struct edge *head_edges;
}vertex_t;


typedef struct graph{
    int nv;
    struct vertex *handle;
}graph_t;
graph_t *initGraph(int nv);
vertex_t *createVertex(int i);
vertex_t *insertVertex(vertex_t *head,vertex_t *newNode);
vertex_t *findVertex(graph_t *g,int key);
edge_t *createEdge(int w, vertex_t *dst);
void insertEdge(vertex_t *src,edge_t *tmpEdge);
graph_t *readFile(char *fileName,int *);
void bfs(graph_t *graph,vertex_t *start);
void disposeGraph(graph_t *g);
void dfs_graph(graph_t *g,vertex_t *start);
int dfs_r(graph_t *g,vertex_t *start,int currTime);
void findEdgeType(vertex_t *src,vertex_t *dest);

int main(int argc,char *argv[])
{

    graph_t *g;
    int num_vertex;
    g=readFile(argv[1],&num_vertex);
    vertex_t *start=findVertex(g,2);
     //bfs(g,start);

    dfs_graph(g,start);

    disposeGraph(g);
return 0;
}

graph_t *readFile(char *fileName,int *numVertex){
	 graph_t *g;
	   FILE *fp;
	  int nv,dir,src,dst,w;
	    vertex_t *dstVertex, *srcVertex;
	    edge_t *tmpEdge;
	    fp=fopen(fileName,"r");
	    if(fscanf(fp,"%d %d",&nv,&dir)!=2){
	        fscanf(fp,"%d",&nv);
	        dir=0;
	    }
	    *numVertex=nv;
	    g=initGraph(nv);


	    while(fscanf(fp,"%d %d %d",&src,&dst,&w)!=EOF){
	        dstVertex=findVertex(g,dst);
	        srcVertex=findVertex(g,src);
	        tmpEdge=createEdge(w,dstVertex);
	        insertEdge(srcVertex,tmpEdge);

	        if(dir==0){ // for undirected graph
		        dstVertex=findVertex(g,src);
		        srcVertex=findVertex(g,dst);
		        tmpEdge=createEdge(w,srcVertex);
		        insertEdge(dstVertex,tmpEdge);
	        }
	    }

	    return g;

}

graph_t *initGraph(int nv){
    graph_t *g;
    vertex_t *tmp;
    int i;
    g=(graph_t *)malloc(1*sizeof(graph_t));
    if(g==NULL){
        printf("error!");
        exit(0);
    }
    g->nv=nv;
    g->handle=NULL;
    for(i=1;i<=nv;i++){
        tmp=createVertex(i);
        g->handle=insertVertex(g->handle,tmp);
    }
    return g;
}
vertex_t *createVertex(int i){
    vertex_t *tmp;
    tmp=(vertex_t *)malloc(1*sizeof(vertex_t));
    if(tmp==NULL){
        printf("error!");
        exit(0);
    }
    tmp->head_edges=NULL;
    tmp->next=NULL;
    tmp->key=i;
    tmp->color=WHITE;
    tmp->distance=INT_MAX;
    tmp->pred=NULL;
    tmp->time_discovery=-1;
    tmp->completed_time=-1;

    return tmp;
}
vertex_t *insertVertex(vertex_t *head,vertex_t *newNode){
    vertex_t *tmp;
    tmp=head;
    // just for first element
    if(tmp==NULL){
        head=newNode;
        return head;
    }
    // put tmp ptr on the last element of list
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }

    // add new node to the last element of list
    tmp->next=newNode;

    return head;
}
vertex_t *findVertex(graph_t *g,int key){
    vertex_t *tmp=g->handle;
    while(tmp!=NULL){
        if(tmp->key==key){
            return tmp;
        }
        tmp=tmp->next;
    }
    return NULL;
}
edge_t *createEdge(int w, vertex_t *dst){
    edge_t *tmp;
    tmp=(edge_t *) malloc(1*sizeof(edge_t));
    if(tmp==NULL){
        printf("error!");
        exit(0);
    }
    tmp->weight=w;
    tmp->dst=dst;
    tmp->next=NULL;
    return tmp;
}
void insertEdge(vertex_t *src,edge_t *tmpEdge){
	/** head insertion */
//    tmpEdge->next=src->head_edges;
//    src->head_edges=tmpEdge;

	/** tail insertion */
	edge_t *tmp;
	    tmp=src->head_edges;
	    // just for first element
	    if(tmp==NULL){
	    	src->head_edges=tmpEdge;
	        return ;
	    }
	    // put tmp ptr on the last element of list
	    while(tmp->next!=NULL){
	        tmp=tmp->next;
	    }

	    // add new node to the last element of list
	    tmp->next=tmpEdge;

    return;
}

void disposeGraph(graph_t *g){

//	graph_t *gTmp=g;
	vertex_t *vTemp=g->handle;
	vertex_t *vTemp2=NULL;
	edge_t *eTmp1=NULL;
	edge_t *eTmp2=NULL;


	while(vTemp!=NULL){

		// free edge
        eTmp1=vTemp->head_edges;
        while(eTmp1!=NULL){
        	eTmp2=eTmp1;
        	eTmp1=eTmp1->next;
        	free(eTmp2);
        }
       // free vertext
        vTemp2=vTemp;
        vTemp=vTemp->next;
        free(vTemp2);

	}
	// free graph
	free(g);

}

void bfs(graph_t *graph,vertex_t *start){
	int numVertex=graph->nv;
	int counter=0;
	queue_t *qp;
	// for the start point
	start->color=GRAY;
	start->distance=0;
	qp=queue_init(numVertex);
	queue_put(qp,(void *)start);

	vertex_t *destTemp;

	edge_t *eTmp;
	// visit Bfs mode
	 while(!queue_empty_m(qp)){
		 queue_get(qp,(void **)&start);
		 eTmp=start->head_edges;

		 while(eTmp!=NULL){
			 destTemp=eTmp->dst;
			 if(destTemp->color==WHITE){
				 destTemp->color=GRAY;
				 destTemp->distance=start->distance+1;
				 destTemp->pred=start;
				 queue_put(qp,(void *)destTemp);
			 }

			 eTmp=eTmp->next;
		 }

		  start->color=BLACK;

		  printf("vertex = %d  distance from start point=%d \n",start->key , start->distance);
		  counter++;
	 }

     printf("number visitd vertex:%d \n",counter);

	 queue_dispose(qp,NULL);

}

void dfs_graph(graph_t *g,vertex_t *start){
	vertex_t * tmpVertex;
	int currTime=0;

	// first call to visit
	currTime=dfs_r(g,start,0);

	 // in case of disconnected graph ( forest dfs tree)
	 tmpVertex=g->handle;
	  while(tmpVertex!=NULL){
		  if(tmpVertex->color==WHITE){
			  currTime=dfs_r(g,tmpVertex,currTime);
		  }
		  tmpVertex=tmpVertex->next;
	  }

}
int dfs_r(graph_t *g,vertex_t *start,int currTime){
    edge_t *eTmp;
    vertex_t *destTmp;
	start->color=GRAY;
    start->time_discovery=++currTime;

    eTmp=start->head_edges;

    while(eTmp!=NULL){
    	destTmp=eTmp->dst;
    	findEdgeType(start,destTmp);
    	if(destTmp->color==WHITE){
    		destTmp->pred=start;
    		currTime=dfs_r(g,destTmp,currTime);
    	}
    	eTmp=eTmp->next;
    }

    start->color=BLACK;
    start->completed_time=++currTime;
    printf("vertex = %d   discovery time: %d  finished Time : %d\n",start->key ,start->time_discovery,start->completed_time);
    return currTime;
}
void findEdgeType(vertex_t *src,vertex_t *dest){
	   /** type edges :*/
	    	switch (dest->color) {
	    	case WHITE:
	    		printf("%d -> %d : TREE EDGE\n", src->key, dest->key);
	    		break;
	    	case GRAY :
	    		printf("%d -> %d : BACK EDGE \n", src->key, dest->key);
	    		break;
	    	case BLACK:
	    		if (src->time_discovery < dest->time_discovery) {
	    	    printf("%d -> %d : FORWARD EDGE\n", src->key, dest->key);
	    	     } else {
	    	     printf("%d -> %d : C\n",src->key, dest->key);
	    	     }
	    	}// end of type edges
}
