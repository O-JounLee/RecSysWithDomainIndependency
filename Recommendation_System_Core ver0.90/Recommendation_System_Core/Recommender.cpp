#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "Recommendation_Controller.h"
#include "structs.h"
#include "Recommendation_Manager.h"
#include "Recommender.h"
#include <cstdlib>
#include "math.h"
#include "stdafx.h"

Reccomender::Reccomender(sql::Connection* con,int num_of_User,int num_of_Item){
	_con=con;

	num_of_user=num_of_User;
	num_of_item=num_of_Item;

	//neighbor_list=new int;

	return;
}

Reccomender::~Reccomender(){

}

float Reccomender::user_distance(int target_id,int client_id){
	float _result=0;
	MemberVector* target_mv=new MemberVector;
	MemberVector* client_mv=new MemberVector;

	get_MemberVector(_con,true,target_id,target_mv);
	get_MemberVector(_con,true,client_id,client_mv);

	for(int i=0;i<5;i++)
		_result=_result+(target_mv->Element[i]-client_mv->Element[i])*(target_mv->Element[i]-client_mv->Element[i]);
	_result=sqrt(_result);

	delete target_mv;
	delete client_mv;

	return _result;
}

void Reccomender::make_neighbor_list(int client_id){
	//delete neighbor_list;

	std::cout<<"Start make_neighbor_list.";
	std::cout<<"\n";

	neighbor_list=new int[40];
	for(int i=0;i<40;i++) neighbor_list[i]=NULL;
	float* value_list=new float[40];
	for(int i=0;i<40;i++) value_list[i]=9999999999.0f;

	for(int i=0;i<num_of_user;i++){
		if((i!=client_id)&&(user_cluster[i]==user_cluster[client_id])){
			float _value=user_distance(i,client_id);
			for (int j=0;j<20;j++){		
				if(value_list[j]>_value){
					for(int l=18;l>j-1;l--){
						neighbor_list[l+1]=neighbor_list[l];
						value_list[l+1]=value_list[l];
					}
					value_list[j]=_value;
					neighbor_list[j]=i;
					break;
				}
			}
		}
	}

	for(int i=0;i<40;i++) std::cout<<neighbor_list[i]<<" ";
	std::cout<<"\n";

	if(neighbor_list[19]==NULL){
		int s_point;
		for(s_point=0;s_point<20;s_point++) if(neighbor_list[s_point]==NULL) break;
		for(int i=0;i<num_of_user;i++){
			bool flag=true;
			for(int h=0;(h<20)&&flag;h++) if(neighbor_list[h]==i) flag=false;
			if((i!=client_id)&&flag){
				float _value=user_distance(i,client_id);
				for (int j=s_point;j<20;j++){
					if(value_list[j]>_value){
						for(int l=18;l>j-1;l--){
							neighbor_list[l+1]=neighbor_list[l];
							value_list[l+1]=value_list[l];
						}
						value_list[j]=_value;
						neighbor_list[j]=i;
						break;
					}
				}
			}
		}
	}

	for(int i=0;i<40;i++) std::cout<<neighbor_list[i]<<" ";
	std::cout<<"\n";

	delete value_list;

	return;
}

float Reccomender::W_point(int target_item,int target_id,int client_id){
	float _result=0;
	MemberVector* target_mv=new MemberVector;
	MemberVector* client_mv=new MemberVector;

	get_MemberVector(_con,true,target_id,target_mv);
	get_MemberVector(_con,true,target_id,client_mv);

	for(int i=0;i<5;i++)
		_result=_result+(target_mv->Element[i]-client_mv->Element[i])*(target_mv->Element[i]-client_mv->Element[i]);
	_result=sqrt(_result)*RatingTable[target_id][target_item];

	delete target_mv;
	delete client_mv;

	return _result;
}

float Reccomender::W_point_em(int target_id,int client_id){
	float _result=0;
	MemberVector* target_mv=new MemberVector;
	MemberVector* client_mv=new MemberVector;

	get_MemberVector(_con,false,target_id,target_mv);
	get_MemberVector(_con,true,client_id,client_mv);

	for(int i=0;i<5;i++)
		_result=_result+(target_mv->Element[i]-client_mv->Element[i])*(target_mv->Element[i]-client_mv->Element[i]);
	_result=sqrt(_result)*0.1f;

	delete target_mv;
	delete client_mv;

	return _result;
}

int* Reccomender::make_Rec_Item_list(int client_id){
	std::cout<<"Start make_Rec_Item_list.";
	std::cout<<"\n";

	int* Rec_Item_list=new int[40];
	for(int i=0;i<40;i++) Rec_Item_list[i]=NULL;
	float* value_list=new float[40];
	for(int i=0;i<40;i++) value_list[i]=99999999.0f;

	for(int i=0;i<40;i++){
		for(int k=0;k<num_of_item;k++){
			if(RatingTable[neighbor_list[i]][k]!=NULL){
				for(int j=0;j<40;j++){
					float _value=W_point(k,neighbor_list[i],client_id);
					if(value_list[j]>_value){
						bool flag=true;
						for(int h=0;(h<40)&&flag;h++) if(Rec_Item_list[h]==k) flag=false;
						if(flag){
							for(int l=38;l>j-1;l--){
								Rec_Item_list[l+1]=Rec_Item_list[l];
								value_list[l+1]=value_list[l];
							}
							value_list[j]=_value;
							Rec_Item_list[j]=k;
							break;
						}
					}
				}
			}
		}
	}

	for(int i=0;i<40;i++) std::cout<<Rec_Item_list[i]<<" ";
	std::cout<<"\n";

	if(Rec_Item_list[39]==NULL){
		int s_point;
		for(s_point=0;s_point<40;s_point++) if(Rec_Item_list[s_point]==NULL) break;
		for(int i=0;i<num_of_user;i++){
			for(int k=0;k<num_of_item;k++){
				bool flag=true;
				for(int h=0;(h<40)&&flag;h++) if(Rec_Item_list[h]==k) flag=false;
				if((RatingTable[i][k]!=NULL)&&(flag)){
					float _value=W_point(k,i,client_id);
					for(int j=s_point;j<40;j++){
						if(value_list[j]>_value){
							for(int l=38;l>j-1;l--){
								Rec_Item_list[l+1]=Rec_Item_list[l];
								value_list[l+1]=value_list[l];
							}
							value_list[j]=_value;
							Rec_Item_list[j]=k;
							break;
						}
					}
				}
			}
		}
	}

	for(int i=0;i<40;i++) std::cout<<Rec_Item_list[i]<<" ";
	std::cout<<"\n";

	if(Rec_Item_list[39]==NULL){
		int s_point;
		for(s_point=0;s_point<40;s_point++) if(Rec_Item_list[s_point]==NULL) break;
		for(int i=0;i<num_of_user;i++){
			for(int k=0;k<num_of_item;k++){
				bool flag=true;
				for(int h=0;(h<40)&&flag;h++) if(Rec_Item_list[h]==k) flag=false;
				if(flag){
					float _value=W_point_em(k,i);
					for(int j=s_point;j<40;j++){
						if(value_list[j]>_value){
							for(int l=38;l>j-1;l--){
								Rec_Item_list[l+1]=Rec_Item_list[l];
								value_list[l+1]=value_list[l];
							}
							value_list[j]=_value;
							Rec_Item_list[j]=k;
							break;
						}
					}
				}
			}
		}
	}

	for(int i=0;i<40;i++) std::cout<<Rec_Item_list[i]<<" ";
	std::cout<<"\n";

	delete neighbor_list;
	delete value_list;

	return Rec_Item_list;
}

int Reccomender::get_rec_item(int target_id,int client_id){
	int _result=rand()%60;

	return _result;
}