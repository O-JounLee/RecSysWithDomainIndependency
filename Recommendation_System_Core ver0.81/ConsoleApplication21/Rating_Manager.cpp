#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "structs.h"
#include "math.h"
#include "Rating_Manager.h"
#include "stdafx.h"

using namespace std;

Rating_Manager::Rating_Manager(int num_user,int num_item){
	num_of_user=num_user;
	num_of_item=num_item;
}

Rating_Manager::~Rating_Manager(){

}

void Rating_Manager::Make_RatingTable(){
	Rating_Table=new float*[num_of_user];
	for(int i=0;i<num_of_user;i++){
		*(Rating_Table+i)=new float[num_of_item];
		for(int j=0;j<num_of_item;j++){
			*(*(Rating_Table+i)+j)=NULL;
		}
	}
	int num_of_log=get_Log_num();
	for(int i=0;i<num_of_log;i++){
		Input_Rating(get_RatingPoint(i).user_id,get_RatingPoint(i).item_id,get_RatingPoint(i).user_pref);
	}
	/*
	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(Rating_Table+i)+j)==NULL) Rating_Prediction(i,j);
		}
	}
	*/
	return;
}

void Rating_Manager::Input_Rating(int user_id,int item_id,int user_pref){
	*(*(Rating_Table+user_id)+item_id)=Rating_Normalizer(user_id,item_id,user_pref);

	return;
}

float Rating_Manager::Rating_Normalizer(int user_id,int item_id,int user_pref){
	float sum=0,dev=0,m=0;
	int cnt=0;
	for(int i=0;i<num_of_item;i++){
//		if(Rating_Table[i][user_id]!=NULL){
		if(*(*(Rating_Table+user_id)+i)!=NULL){
			sum=sum+Rating_Table[i][user_id];
			cnt++;
		}
	}
	if(cnt==0) cnt=1;
	m=sum/cnt;
	sum=0;
	for(int i=0;i<num_of_item;i++){
//		if(Rating_Table[i][user_id]!=NULL){
		if(*(*(Rating_Table+user_id)+i)!=NULL){
			sum=sum+(Rating_Table[i][user_id]-m)*(Rating_Table[i][user_id]-m);
		}
	}
	dev=sqrt(sum/cnt);
	
	return 1-dev/(user_pref-m);
}

float Rating_Manager::distance(bool type,int target_id,int client_id){
	float _result=0;
	for(int i=0;i<5;i++) _result=_result+(get_MemberVector(true,target_id)-get_MemberVector(true,client_id))*(get_MemberVector(true,target_id)-get_MemberVector(true,client_id));
	_result=1/sqrt(_result);

	return _result;
}

float Rating_Manager::Rating_Prediction(int User_id,int Item_id,int* User_Cluster_Table,int* Item_Cluster_Table){
	float _result;

	int* user_neighbor_list=new int[20];
	for(int i=0;i<20;i++) user_neighbor_list[i]=NULL;
	float* user_value_list=new float[20];
	for(int i=0;i<20;i++) user_value_list[i]=NULL;

	for(int i=0;i<num_of_user;i++){
		if(i!=User_id&&User_Cluster_Table[i]==User_Cluster_Table[User_id]){
			for (int j=0;j<20;j++){
				float _value=distance(1,i,User_id);
				if(user_value_list[j]>_value){
					for(int l=j+1;l<19;l++){
						user_neighbor_list[l+1]=user_neighbor_list[l];
						user_value_list[l+1]=user_value_list[l];
					}
					user_value_list[j]=_value;
					user_neighbor_list[j]=i;
				}
			}
		}
	}
	if(user_neighbor_list[19]==NULL){
		int s_point;
		for(s_point=0;s_point<20;s_point++) if(user_neighbor_list[s_point]==NULL) break;
		for(int i=0;i<num_of_user;i++){
			if(i!=User_id){
				for (int j=s_point;j<20;j++){
					float _value=distance(1,i,User_id);
					if(user_value_list[j]>_value){
						for(int l=j+1;l<19;l++){
							user_neighbor_list[l+1]=user_neighbor_list[l];
							user_value_list[l+1]=user_value_list[l];
						}
						user_value_list[j]=_value;
						user_neighbor_list[j]=i;
					}
				}
			}
		}
	}

	int* item_neighbor_list=new int[20];
	for(int i=0;i<20;i++) item_neighbor_list[i]=NULL;
	float* item_value_list=new float[20];
	for(int i=0;i<20;i++) item_value_list[i]=NULL;

	for(int i=0;i<num_of_item;i++){
		if(i!=Item_id&&Item_Cluster_Table[i]==Item_Cluster_Table[Item_id]){
			for (int j=0;j<20;j++){
				float _value=distance(0,i,Item_id);
				if(item_value_list[j]>_value){
					for(int l=j+1;l<19;l++){
						item_neighbor_list[l+1]=item_neighbor_list[l];
						item_value_list[l+1]=item_value_list[l];
					}
					item_value_list[j]=_value;
					item_neighbor_list[j]=i;
				}
			}
		}
	}
	if(user_neighbor_list[19]==NULL){
		int s_point;
		for(s_point=0;s_point<20;s_point++) if(item_neighbor_list[s_point]==NULL) break;
		for(int i=0;i<num_of_item;i++){
			if(i!=Item_id){
				for (int j=s_point;j<20;j++){
					float _value=distance(0,i,User_id);
					if(item_value_list[j]>_value){
						for(int l=j+1;l<19;l++){
							item_neighbor_list[l+1]=item_neighbor_list[l];
							item_value_list[l+1]=item_value_list[l];
						}
						item_value_list[j]=_value;
						item_neighbor_list[j]=i;
					}
				}
			}
		}
	}
	
	float sum_pre_user_R=0,sum_user_W=0,sum_pre_item_R=0,sum_item_W=0;

	for(int i=0;i<20&&user_neighbor_list[i]!=NULL;i++){
		if(Rating_Table[Item_id][user_neighbor_list[i]]==NULL) sum_user_W=sum_user_W+user_value_list[i];
		sum_pre_user_R=sum_pre_user_R+user_value_list[i]*Rating_Table[Item_id][user_neighbor_list[i]];
		sum_user_W=sum_user_W+user_value_list[i];
	}
	for(int i=0;i<20&&item_neighbor_list[i]!=NULL;i++){
		if(Rating_Table[item_neighbor_list[i]][User_id]==NULL) sum_item_W=sum_item_W+item_value_list[i];
		sum_pre_item_R=sum_pre_item_R+item_value_list[i]*Rating_Table[item_neighbor_list[i]][User_id];
		sum_item_W=sum_item_W+item_value_list[i];
	}

	_result=(float)(sum_pre_user_R/sum_user_W*0.5+sum_pre_item_R/sum_item_W*0.5);

	return _result;
}

void Rating_Manager::add_User(int User_id){
	num_of_user++;
	float** temp;
	temp=new float*[num_of_user];
	for(int i=0;i<num_of_user;i++){
		*(temp+i)=new float[num_of_item];
		for(int j=0;j<num_of_item;j++){
			if(i!=(num_of_user-1)){
				*(*(temp+i)+j)=*(*(Rating_Table+i)+j);
			}
			else *(*(temp+i)+j)=NULL;
		}
	}
	delete Rating_Table;
	Rating_Table=temp;

	/*
	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(Rating_Table+i)+j)==NULL||*(*(Rating_Table+i)+j)<0) Rating_Prediction(i,j);
		}
	}
	*/
	return;
}

void Rating_Manager::add_Item(int Item_id){
	num_of_item++;
	float** temp;
	temp=new float*[num_of_user];
	for(int i=0;i<num_of_user;i++){
		*(temp+i)=new float[num_of_item];
		for(int j=0;j<num_of_item;j++){
			if(j!=(num_of_item-1)){
				*(*(temp+i)+j)=*(*(Rating_Table+i)+j);
			}
			else *(*(temp+i)+j)=NULL;
		}
	}
	delete Rating_Table;
	Rating_Table=temp;

	/*
	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(Rating_Table+i)+j)==NULL||*(*(Rating_Table+i)+j)<0) Rating_Prediction(i,j);
		}
	}
	*/
	return;
}