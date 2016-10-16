#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "Rating_Manager.h"
#include "Member_Manager.h"
#include "Clustering_Manager.h"
#include "structs.h"
#include "Recommendation_Model_Manager.h"
#include "stdafx.h"

using namespace std;

Rec_Model_Manager::Rec_Model_Manager(){
	num_of_user=get_User_num();
	num_of_item=get_Item_num();

	M_Manager=new Member_Manager(num_of_user,num_of_item);
	C_Manager=new Cluster_Manager(num_of_user,num_of_item);
	R_Manager=new Rating_Manager(num_of_user,num_of_item);

	M_Manager->Make_UserList();
	M_Manager->Make_ItemList();

	C_Manager->Make_I_ClusterTable();
	C_Manager->Make_U_ClusterTable();

	R_Manager->Make_RatingTable();

	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(R_Manager->Rating_Table+i)+j)==NULL||*(*(R_Manager->Rating_Table+i)+j)<0)
				*(*(R_Manager->Rating_Table+i)+j)=R_Manager->Rating_Prediction(i,j,C_Manager->U_Cluster_Table,C_Manager->I_Cluster_Table);
		}
	}

	//HANDLE thread1=(HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))Input_Member,NULL,0,NULL);
	//HANDLE thread2=(HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))Input_Rating,NULL,0,NULL);
	//HANDLE thread3=(HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))Time_Interrupt,NULL,0,NULL);

}

Rec_Model_Manager::~Rec_Model_Manager(){

}

void Rec_Model_Manager::Time_Interrupt(){
	C_Manager->Create_Cluster(1);
	C_Manager->Create_Cluster(0);

	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(R_Manager->Rating_Table+i)+j)==NULL||*(*(R_Manager->Rating_Table+i)+j)<0)
				R_Manager->Rating_Table[j][i]=R_Manager->Rating_Prediction(i,j,C_Manager->U_Cluster_Table,C_Manager->I_Cluster_Table);
		}
	}

	return;
}

void Rec_Model_Manager::Input_Rating(int user_id,int item_id,int user_pref,int item_prior){
	R_Manager->Input_Rating(user_id,item_id,user_pref);
	float N_rating=R_Manager->Rating_Normalizer(user_id,item_id,user_pref);
	M_Manager->learning_U_Vector(user_id,item_id,N_rating);
	M_Manager->learning_I_Vector(user_id,item_id,item_prior);

	return;
}

void Rec_Model_Manager::Input_Member(bool Type,int id){
	if(Type==1){
		num_of_user++;
	
		M_Manager->add_User(id);
		C_Manager->add_User(id);
		R_Manager->add_User(id);
	}
	else{
		num_of_user++;

		M_Manager->add_Item(id);
		C_Manager->add_Item(id);
		R_Manager->add_Item(id);
	}
	return;
}



