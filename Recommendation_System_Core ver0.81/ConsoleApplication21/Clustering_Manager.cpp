#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "structs.h"
#include "math.h"
#include "Clustering_Manager.h"
#include "stdafx.h"

using namespace std;

Cluster_Manager::Cluster_Manager(int num_user,int num_item){
	num_of_user=num_user;
	num_of_item=num_item;
}

Cluster_Manager::~Cluster_Manager(){

}

void Cluster_Manager::Make_U_ClusterTable(){
	U_Cluster_Table=new int[num_of_user];
	for(int i=0;i<num_of_user;i++) U_Cluster_Table[i]=NULL;
	return;
}

void Cluster_Manager::Make_I_ClusterTable(){
	I_Cluster_Table=new int[num_of_item];
	for(int i=0;i<num_of_item;i++) I_Cluster_Table[i]=NULL;
	return;
}


int Cluster_Manager::K_decision(bool type){
	int _result;
	float min_d=999999999.0f;
	if(type){
		for(int i=5;i<30;i++) if(min_d>internal_deviation(1,make_cluster(1,i,num_of_user),i,num_of_user)) _result=i;
	}
	else{
		for(int i=5;i<30;i++) if(min_d>internal_deviation(0,make_cluster(0,i,num_of_item),i,num_of_item)) _result=i;
	}
	
	return _result;
}

float Cluster_Manager::internal_deviation(bool type,int* target,int num_of_cluster,int num){
	float _result=0;
	if(type){
		for(int i=0;i<num_of_cluster;i++){
			MemberVector d=deviation(1,target,i,num);
			for(int j=0;j<5;j++) _result=_result+d.Element[j];
		}
	}
	else{
		for(int i=0;i<num_of_cluster;i++){
			MemberVector d=deviation(0,target,i,num);
			for(int j=0;j<5;j++) _result=_result+d.Element[j];
		}
	}
	
	return _result;
}

void Cluster_Manager::Create_Cluster(bool type){
	if(type){
		float k=(float)K_decision(1);

		int* _result=new int[num_of_user];

		float* prob=new float[(int)k];
		for(int i=0;i<k;i++) prob[i]=1/k;
		MemberVector* m=new MemberVector[(int)k];
		for(int i=0;i<k;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(1,i)->Element[j];
		MemberVector* d=new MemberVector[(int)k];
		for(int i=0;i<k;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(1,i)->Element[j];

		float** pd=new float*[num_of_user];
		for(int i=0;i<num_of_user;i++) *(pd+i)=new float[(int)k];

		for(int i=0;i<100;i++){
			for(int j=0;j<k;j++) for(int l=0;l<num_of_user;l++) pd[j][l]=probabillity_density(1,l,m[j],d[j])*prob[j];
			for(int j=0;j<num_of_user;j++){
				float max=0;
				//int cluster;
				for(int l=0;l<k;l++){
					if(pd[j][l]>max){
						max=pd[j][l];
						_result[j]=l;
					}
				}
			}
			for(int i=0;i<k;i++) prob[i]=(float)count(_result,i,num_of_user)/(float)num_of_user;
			for(int i=0;i<k;i++) m[i]=mean(1,_result,i,num_of_user);
			for(int i=0;i<k;i++) d[i]=deviation(1,_result,i,num_of_user);
		}

		U_Cluster_Table=_result;
		num_of_user_cluster=(int)k;
		for(int i=0;i<k;i++){
			User_Cluster_Info_list[i].mean=m[i];
			User_Cluster_Info_list[i].s_deviation=d[i];
			User_Cluster_Info_list[i].prob=prob[i];
		}
	}
	else{
		float k=(float)K_decision(0);

		int* _result=new int[num_of_item];

		float* prob=new float[(int)k];
		for(int i=0;i<k;i++) prob[i]=1/k;
		MemberVector* m=new MemberVector[(int)k];
		for(int i=0;i<k;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(0,i)->Element[j];
		MemberVector* d=new MemberVector[(int)k];
		for(int i=0;i<k;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(0,i)->Element[j];

		float** pd=new float*[num_of_item];
		for(int i=0;i<num_of_item;i++) *(pd+i)=new float[(int)k];

		for(int i=0;i<100;i++){
			for(int j=0;j<k;j++) for(int l=0;l<num_of_item;l++) pd[j][l]=probabillity_density(0,l,m[j],d[j])*prob[j];
			for(int j=0;j<num_of_item;j++){
				float max=0;
				//int cluster;
				for(int l=0;l<k;l++){
					if(pd[j][l]>max){
						max=pd[j][l];
						_result[j]=l;
					}
				}
			}
			for(int i=0;i<k;i++) prob[i]=(float)count(_result,i,num_of_item)/(float)num_of_item;
			for(int i=0;i<k;i++) m[i]=mean(0,_result,i,num_of_item);
			for(int i=0;i<k;i++) d[i]=deviation(0,_result,i,num_of_item);
		}

		I_Cluster_Table=_result;
		num_of_item_cluster=(int)k;
		for(int i=0;i<k;i++){
			Item_Cluster_Info_list[i].mean=m[i];
			Item_Cluster_Info_list[i].s_deviation=d[i];
			Item_Cluster_Info_list[i].prob=prob[i];
		}
	}

	return;
}

int* Cluster_Manager::make_cluster(bool type,const int num_of_cluster,int num){
	int* _result=new int[num];
	float* prob=new float[num_of_cluster];
	for(int i=0;i<num_of_cluster;i++) prob[i]=1/(float)num_of_cluster;

	if(type){
		MemberVector* m=new MemberVector[num_of_cluster];
		for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(1,i)->Element[j];
		MemberVector* d=new MemberVector[num_of_cluster];
		for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(1,i)->Element[j];

		float** pd=new float*[num];
		for(int i=0;i<num;i++) *(pd+i)=new float[num_of_cluster];

		for(int i=0;i<100;i++){
			for(int j=0;j<num_of_cluster;j++) for(int l=0;l<num;l++) pd[j][l]=probabillity_density(1,l,m[j],d[j])*prob[j];
			for(int j=0;j<num;j++){
				float max=0;
				//int cluster;
				for(int l=0;l<num_of_cluster;l++){
					if(pd[j][l]>max){
						max=pd[j][l];
						_result[j]=l;
					}
				}
			}
			for(int i=0;i<num_of_cluster;i++) prob[i]=(float)count(_result,i,num)/(float)num;
			for(int i=0;i<num_of_cluster;i++) m[i]=mean(1,_result,i,num);
			for(int i=0;i<num_of_cluster;i++) d[i]=deviation(1,_result,i,num);
		}
	}
	else{
		MemberVector* m=new MemberVector[num_of_cluster];
		for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(0,i)->Element[j];
		MemberVector* d=new MemberVector[num_of_cluster];
		for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(0,i)->Element[j];

		float** pd=new float*[num];
		for(int i=0;i<num;i++) *(pd+i)=new float[num_of_cluster];

		for(int i=0;i<100;i++){
			for(int j=0;j<num_of_cluster;j++) for(int l=0;l<num;l++) pd[j][l]=probabillity_density(0,l,m[j],d[j])*prob[j];
			for(int j=0;j<num;j++){
				float max=0;
				//int cluster;
				for(int l=0;l<num_of_cluster;l++){
					if(pd[j][l]>max){
						max=pd[j][l];
						_result[j]=l;
					}
				}
			}
			for(int i=0;i<num_of_cluster;i++) prob[i]=(float)count(_result,i,num)/(float)num;
			for(int i=0;i<num_of_cluster;i++) m[i]=mean(0,_result,i,num);
			for(int i=0;i<num_of_cluster;i++) d[i]=deviation(0,_result,i,num);
		}
	}

	return _result;
}

void Cluster_Manager::adjust_user_cluster(int user_id){
	float p=0;
	//int cluster;
	for(int i=0;i<num_of_user_cluster;i++){
		MemberVector m=mean(1,U_Cluster_Table,i,num_of_user),d=deviation(1,U_Cluster_Table,i,num_of_user);
		float pre=probabillity_density(1,user_id,m,d);
		if(p>pre){
			p=pre;
			U_Cluster_Table[user_id]=i;
		}
	}
	
	return;
}

void Cluster_Manager::adjust_item_cluster(int item_id){
	float p=0;
	//int cluster;
	for(int i=0;i<num_of_item_cluster;i++){
		MemberVector m=mean(0,I_Cluster_Table,i,num_of_item),d=deviation(0,I_Cluster_Table,i,num_of_item);
		float pre=probabillity_density(0,item_id,m,d);
		if(p>pre){
			p=pre;
			I_Cluster_Table[item_id]=i;
		}
	}

	return;
}

float Cluster_Manager::probabillity_density(bool type,int id,MemberVector m,MemberVector d){
	float _result=1;
	//UserVector m=mean(target,cluster_num,num),d=deviation(target,cluster_num,num);

	if(type){
		for(int i=0;i<5;i++) _result=(float)(_result*(1/sqrt(2*3.14159265)*d.Element[i])*exp(-1*(get_MemberVector(1,id)->Element[i]-m.Element[i])*(get_MemberVector(1,id)->Element[i]-m.Element[i])/(2*d.Element[i]*d.Element[i])));
	}
	else{
		for(int i=0;i<5;i++) _result=(float)(_result*(1/sqrt(2*3.14159265)*d.Element[i])*exp(-1*(get_MemberVector(0,id)->Element[i]-m.Element[i])*(get_MemberVector(0,id)->Element[i]-m.Element[i])/(2*d.Element[i]*d.Element[i])));
	}
	
	return _result;
}

MemberVector Cluster_Manager::sum(bool type,int* target,int cluster_num,int num){
	MemberVector _result;
	for(int i=0;i<5;i++) _result.Element[i]=0;

	if(type){
		for(int i=0;i<num;i++) if(target[i]==cluster_num) for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+get_MemberVector(1,i)->Element[j];
	}
	else{
		for(int i=0;i<num;i++) if(target[i]==cluster_num) for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+get_MemberVector(0,i)->Element[j];
	}
	
	return _result;
}

MemberVector Cluster_Manager::mean(bool type,int* target,int cluster_num,int num){
	MemberVector _result;

	if(type){
		for(int i=0;i<5;i++) _result.Element[i]=sum(1,target,cluster_num,num).Element[i]/count(target,cluster_num,num);
	}
	else{
		for(int i=0;i<5;i++) _result.Element[i]=sum(0,target,cluster_num,num).Element[i]/count(target,cluster_num,num);
	}
	
	return _result;
}

MemberVector Cluster_Manager::deviation(bool type,int* target,int cluster_num,int num){
	MemberVector _result;
	for(int i=0;i<5;i++) _result.Element[i]=0;

	if (type){
		MemberVector m=mean(1,target,cluster_num,num);
		for(int i=0;i<num;i++) if(target[i]==cluster_num) for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+((m.Element[j]-get_MemberVector(1,i)->Element[j])*(m.Element[j]-get_MemberVector(1,i)->Element[j]));
		for(int i=0;i<5;i++) _result.Element[i]=sqrt(_result.Element[i]/count(target,cluster_num,num));
	}
	else{
		MemberVector m=mean(0,target,cluster_num,num);
		for(int i=0;i<num;i++) if(target[i]==cluster_num) for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+((m.Element[j]-get_MemberVector(0,i)->Element[j])*(m.Element[j]-get_MemberVector(0,i)->Element[j]));
		for(int i=0;i<5;i++) _result.Element[i]=sqrt(_result.Element[i]/count(target,cluster_num,num));
	}
	
	return _result;
}

int Cluster_Manager::count(int* target,int cluster_num,int num){
	int _result=0;
	for(int i=0;i<num;i++) if(target[i]==cluster_num) _result++;

	return _result;
}

void Cluster_Manager::add_User(int User_id){
	num_of_user++;
	int* temp=new int[num_of_user];
	for(int i=0;i<num_of_user-1;i++) temp[i]=U_Cluster_Table[i];
	adjust_user_cluster(num_of_user-1);
	delete U_Cluster_Table;
	U_Cluster_Table=temp;

	return;
}

void Cluster_Manager::add_Item(int Item_id){
	num_of_item++;
	int* temp=new int[num_of_item];
	for(int i=0;i<num_of_item-1;i++) temp[i]=I_Cluster_Table[i];
	adjust_item_cluster(num_of_item-1);
	delete I_Cluster_Table;
	I_Cluster_Table=temp;

	return;
}