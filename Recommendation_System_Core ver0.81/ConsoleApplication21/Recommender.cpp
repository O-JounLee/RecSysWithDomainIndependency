#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "Recommendation_Controller.h"
#include "structs.h"
#include "Recommendation_Manager.h"
#include "Recommender.h"
#include "math.h"
#include "stdafx.h"

Reccomender::Reccomender(int num_of_User,int num_of_Item){
	num_of_user=num_of_User;
	num_of_item=num_of_Item;

	return;
}

Reccomender::~Reccomender(){

}

float Reccomender::user_distance(int target_id,int client_id){
	float _result=0;
	for(int i=0;i<5;i++) _result=_result+(get_MemberVector(true,target_id)-get_MemberVector(true,client_id))*(get_MemberVector(true,target_id)-get_MemberVector(true,client_id));
	_result=1/sqrt(_result);

	return _result;
}

void Reccomender::make_neighbor_list(int client_id){
	neighbor_list=new int[20];
	for(int i=0;i<20;i++) neighbor_list[i]=NULL;
	float* value_list=new float[20];
	for(int i=0;i<20;i++) value_list[i]=NULL;

	for(int i=0;i<num_of_user;i++){
		if(i!=client_id&&user_cluster[i]==user_cluster[client_id]){
			for (int j=0;j<20;j++){
				float _value=user_distance(i,client_id);
				if(value_list[j]>_value){
					for(int l=j+1;l<19;l++){
						neighbor_list[l+1]=neighbor_list[l];
						value_list[l+1]=value_list[l];
					}
					value_list[j]=_value;
					neighbor_list[j]=i;
				}
			}
		}
	}
	if(neighbor_list[19]==NULL){
		int s_point;
		for(s_point=0;s_point<20;s_point++) if(neighbor_list[s_point]==NULL) break;
		for(int i=0;i<num_of_user;i++){
			if(i!=client_id){
				for (int j=s_point;j<20;j++){
					float _value=user_distance(i,client_id);
					if(value_list[j]>_value){
						for(int l=j+1;l<19;l++){
							neighbor_list[l+1]=neighbor_list[l];
							value_list[l+1]=value_list[l];
						}
						value_list[j]=_value;
						neighbor_list[j]=i;
					}
				}
			}
		}
	}
	delete value_list;

	return;
}

float Reccomender::W_point(int target_item,int target_id,int client_id){
	float _result=0;
	for(int i=0;i<5;i++) _result=_result+(get_MemberVector(true,target_id)-get_MemberVector(true,client_id))*(get_MemberVector(true,target_id)-get_MemberVector(true,client_id));
	_result=1/sqrt(_result)*RatingTable[target_item][target_id];

	return _result;
}

int* Reccomender::make_Rec_Item_list(int client_id){
	int* Rec_Item_list=new int[20];
	for(int i=0;i<20;i++) Rec_Item_list[i]=NULL;
	float* value_list=new float[20];
	for(int i=0;i<20;i++) value_list[i]=NULL;

	for(int i=0;i<20;i++){
		for(int k=0;k<num_of_item;k++){
			if(RatingTable[k][neighbor_list[i]]!=NULL){
				for(int j=0;j<20;j++){
					float _value=W_point(k,neighbor_list[i],client_id);
					if(value_list[j]>_value){
						bool flag=true;
						for (int h=0;h<20;h){
							if(Rec_Item_list[h]==k){
								flag=false;
								break;
							}
						}
						if(flag){
							for(int l=j+1;l<19;l++){
								Rec_Item_list[l+1]=Rec_Item_list[l];
								value_list[l+1]=value_list[l];
							}
							value_list[j]=_value;
							Rec_Item_list[j]=k;
						}
					}
				}
			}
		}
	}
	if(Rec_Item_list[19]==NULL){
		int s_point;
		for(s_point=0;s_point<20;s_point++) if(Rec_Item_list[s_point]==NULL) break;
		for(int i=0;i<num_of_user;i++){
			for(int k=0;k<num_of_item;k++){
				if(RatingTable[k][i]!=NULL){
					for(int j=s_point;j<20;j++){
						float _value=W_point(k,i,client_id);
						if(value_list[j]>_value){
							bool flag=true;
							for (int h=0;h<20;h){
								if(Rec_Item_list[h]==k){
									flag=false;
									break;
								}
							}
							if(flag){
								for(int l=j+1;l<19;l++){
									Rec_Item_list[l+1]=Rec_Item_list[l];
									value_list[l+1]=value_list[l];
								}
								value_list[j]=_value;
								Rec_Item_list[j]=k;
							}
						}
					}
				}
			}
		}
	}
	delete value_list;

	return Rec_Item_list;
}
