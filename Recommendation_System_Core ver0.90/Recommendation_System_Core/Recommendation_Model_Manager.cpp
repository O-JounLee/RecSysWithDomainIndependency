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

Rec_Model_Manager::Rec_Model_Manager(sql::Connection* con){
	_con=con;

	num_of_user=get_User_num(_con);
	cout<<"num_of_user is "<< num_of_user <<" \n";
	num_of_item=get_Item_num(_con);
	cout<<"num_of_item is "<< num_of_item <<" \n";

	cout<<"Start making Member_Manager \n";
	M_Manager=new Member_Manager(_con,num_of_user,num_of_item);
	cout<<"Start making Cluster_Manager \n";
	C_Manager=new Cluster_Manager(_con,num_of_user,num_of_item);
	cout<<"Start making Rating_Manager \n";
	R_Manager=new Rating_Manager(_con,num_of_user,num_of_item);

	cout<<"Start Make_UserList \n";
	M_Manager->Make_UserList();
	cout<<"Start Make_ItemList \n";
	M_Manager->Make_ItemList();

	cout<<"Start Make_U_ClusterTable \n";
	C_Manager->Make_U_ClusterTable();
	cout<<"Start Make_I_ClusterTable \n";
	C_Manager->Make_I_ClusterTable();

	cout<<"Start Make_RatingTable \n";
	R_Manager->Make_RatingTable();

	cout<<"Start Create User Cluster \n";
	C_Manager->Create_Cluster(true);
	cout<<"Finish Create User Cluster \n";
	cout<<"Start Create Item Cluster \n";
	C_Manager->Create_Cluster(false);
	cout<<"Finish Create Item Cluster \n";

	/*
	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			cout<<"Start Predict RatingPoint log:"<< i << j <<" \n";
			if(*(*(R_Manager->Rating_Table+i)+j)==NULL||*(*(R_Manager->Rating_Table+i)+j)<0)
				*(*(R_Manager->Rating_Table+i)+j)=R_Manager->Rating_Prediction(i,j,C_Manager->U_Cluster_Table,C_Manager->I_Cluster_Table);
		}
	}
	*/

	/*for(int i=0;i<num_of_user;i++){
		for(int j=0;j<5;j++) cout<<M_Manager->U_Vectorlist[i].Element[j]<<" ";
		cout<<C_Manager->U_Cluster_Table[i]<<"\n";
	}
	
	for(int i=0;i<num_of_item;i++){
		for(int j=0;j<5;j++) cout<<M_Manager->I_Vectorlist[i].Element[j]<<" ";
		cout<<C_Manager->I_Cluster_Table[i]<<"\n";
	}*/

	//HANDLE thread1=(HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))Input_Member,NULL,0,NULL);
	//HANDLE thread2=(HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))Input_Rating,NULL,0,NULL);
	//HANDLE thread3=(HANDLE)_beginthreadex(NULL,0,(unsigned int (__stdcall *)(void *))Time_Interrupt,NULL,0,NULL);

}

Rec_Model_Manager::~Rec_Model_Manager(){

}

void Rec_Model_Manager::Time_Interrupt(){
	std::cout<<"Start Clustering";
	C_Manager->Create_Cluster(true);
	C_Manager->Create_Cluster(false);

	for(int i=0;i<num_of_user;i++) delete *((R_Manager->Rating_Table)+i);
	delete R_Manager->Rating_Table;

	R_Manager->Make_RatingTable();

	std::cout<<"Start Missing Rating Prediction";
	for(int i=0;i<num_of_user;i++){
		for(int j=0;j<num_of_item;j++){
			if(*(*(R_Manager->Rating_Table+i)+j)==NULL||*(*(R_Manager->Rating_Table+i)+j)<0)
				R_Manager->Rating_Table[i][j]=R_Manager->Rating_Prediction(i,j,C_Manager->U_Cluster_Table,C_Manager->I_Cluster_Table);
		}
	}

	return;
}

void Rec_Model_Manager::Input_Rating(int log_num){
	Rating_Point* rp=new Rating_Point;
	get_RatingPoint(_con,log_num,rp);

	int user_id=rp->user_id;
	int item_id=rp->item_id;
	float user_pref=rp->user_pref;
	float item_prior=rp->item_prior;

	std::cout<<"Start Input_Rating";

	//R_Manager->Input_Rating(user_id,item_id,user_pref);

	float m=R_Manager->m[user_id];
	float d=R_Manager->d[user_id];
	float N_rating=R_Manager->Rating_Normalizer(d,user_pref,m);

	R_Manager->Input_Rating(user_id,item_id,N_rating);

	M_Manager->learning_U_Vector(user_id,item_id,N_rating);
	M_Manager->learning_I_Vector(user_id,item_id,item_prior);

	delete rp;

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



