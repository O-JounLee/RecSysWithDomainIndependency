#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "structs.h"
#include "math.h"
#include "Rating_Manager.h"
#include "stdafx.h"

using namespace std;

Rating_Manager::Rating_Manager(sql::Connection* con,int num_user,int num_item){
	_con=con;

	num_of_user=num_user;
	num_of_item=num_item;
}

Rating_Manager::~Rating_Manager(){

}

void Rating_Manager::Make_RatingTable(){
	Rating_Table=new float*[num_of_user];
	Rating_Point* rp=new Rating_Point;
	for(int i=0;i<num_of_user;i++){
		*(Rating_Table+i)=new float[num_of_item];
		for(int j=0;j<num_of_item;j++){
			*(*(Rating_Table+i)+j)=NULL;
			//cout<<*(*(Rating_Table+i)+j);
		}
	}

	int num_of_log=get_Log_num(_con);
	for(int i=0;i<num_of_log;i++){
		get_RatingPoint(_con,i,rp);
		Input_Rating(rp->user_id,rp->item_id,rp->user_pref);
	}

	m=new float[num_of_user];
	for(int i=0;i<num_of_user;i++) m[i]=_mean(i);
	d=new float[num_of_user];
	for(int i=0;i<num_of_user;i++) d[i]=deviation(i,m[i]);

	for(int i=0;i<num_of_log;i++){
		get_RatingPoint(_con,i,rp);
		
		//m[rp->user_id]=_mean(rp->user_id);
		//d[rp->user_id]=deviation(rp->user_id,m[rp->user_id]);

		Input_Rating(rp->user_id,rp->item_id,rp->user_pref,d[rp->user_id],m[rp->user_id]);
	}
	/*
	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(Rating_Table+i)+j)==NULL) Rating_Prediction(i,j);
		}
	}
	*/
	delete rp;

	return;
}

void Rating_Manager::Input_Rating(int user_id,int item_id,int user_pref){
	*(*(Rating_Table+user_id)+item_id)=(float)user_pref/10.0f;

	return;
}

void Rating_Manager::Input_Rating(int user_id,int item_id,int user_pref,float dev,float mu){
	(*(*(Rating_Table+user_id)+item_id))=Rating_Normalizer(dev,user_pref,mu);

	return;
}

float Rating_Manager::_mean(int user_id){
	float sum=0,m=0;
	int cnt=0;
	for(int i=0;i<num_of_item;i++){
		//		if(Rating_Table[i][user_id]!=NULL){
		//cout<<*(*(Rating_Table+user_id)+i);
		if((*(*(Rating_Table+user_id)+i)!=NULL)&&(*(*(Rating_Table+user_id)+i)!=0.0f)){
			sum=sum+*(*(Rating_Table+user_id)+i);
			cnt++;
		}
	}
	if(cnt==0) cnt=1;
	m=sum/cnt;

	return m;
}

float Rating_Manager::deviation(int user_id,float m){
	float sum=0,dev=0;
	int cnt=0;
	for(int i=0;i<num_of_item;i++){
		//		if(Rating_Table[i][user_id]!=NULL){
		if((*(*(Rating_Table+user_id)+i)!=NULL)&&(*(*(Rating_Table+user_id)+i)!=0.0f)){
			sum=sum+(*(*(Rating_Table+user_id)+i)-m)*(*(*(Rating_Table+user_id)+i)-m);
			cnt++;
		}
	}
	if(cnt==0) cnt=1;
	dev=sqrt(sum/cnt);

	return dev;
}

float Rating_Manager::Rating_Normalizer(float dev,float user_pref,float m){
	//수정 필요
	float mom=user_pref-m;
	if(mom==0) mom=1;
	float _result=1-dev/mom;

	return _result;
}

float Rating_Manager::distance(bool type,int target_id,int client_id){
	float _result=0;
	MemberVector* mv_target=new MemberVector;
	MemberVector* mv_client=new MemberVector;

	get_MemberVector(_con,true,target_id,mv_target);
	get_MemberVector(_con,true,target_id,mv_client);

	for(int i=0;i<5;i++)
		_result=_result+(mv_target->Element[i]-mv_client->Element[i])*(mv_target->Element[i]-mv_client->Element[i]);
	_result=1/sqrt(_result);

	delete mv_target;
	delete mv_client;

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

	delete user_neighbor_list;
	delete user_value_list;
	delete item_neighbor_list;
	delete item_value_list;

	return _result;
}

void Rating_Manager::add_User(int User_id){
	num_of_user++;
	float** temp=new float*[num_of_user];
	for(int i=0;i<num_of_user;i++){
		*(temp+i)=new float[num_of_item];
		for(int j=0;j<num_of_item;j++){
			if(i!=(num_of_user-1)) *(*(temp+i)+j)=*(*(Rating_Table+i)+j);
			else *(*(temp+i)+j)=NULL;
		}
	}

	for(int i=0;i<num_of_user-1;i++) delete *(Rating_Table+i);
	delete Rating_Table;

	Rating_Table=temp;

	float* tmp_m=new float[num_of_user];
	for(int i=0;i<num_of_user-1;i++) tmp_m[i]=m[i];
	tmp_m[num_of_user-1]=0;

	delete m;
	m=tmp_m;

	float* tmp_d=new float[num_of_user];
	for(int i=0;i<num_of_user-1;i++) tmp_d[i]=d[i];
	tmp_d[num_of_user-1]=0;

	delete d;
	d=tmp_d;

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
	float** temp=new float*[num_of_user];
	for(int i=0;i<num_of_user;i++){
		*(temp+i)=new float[num_of_item];
		for(int j=0;j<num_of_item;j++){
			if(j!=(num_of_item-1)) *(*(temp+i)+j)=*(*(Rating_Table+i)+j);
			else *(*(temp+i)+j)=NULL;
		}
	}

	for(int i=0;i<num_of_user;i++) delete *(Rating_Table+i);
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