#include <iostream>
#include <stdio.h>
#include "Recommender.h"
#include "Rule_Apllier.h"
#include "Intra_Interface.h"
#include "Recommendation_Controller.h"
#include "structs.h"
#include "Recommendation_Manager.h"
#include "stdafx.h"

Rec_Manager::Rec_Manager(){
	num_of_user=get_User_num();
	num_of_item=get_Item_num();

	Rec_executer=new Reccomender(num_of_user,num_of_item);
	Rule_applier=new Rule_Applier(num_of_user,num_of_item);

	return;
}

Rec_Manager::~Rec_Manager(){

}

int* Rec_Manager::get_Rec_list(int user_id,int* cur_Cluster_Table,float** cur_Rating_Table){
	Rec_executer->user_cluster=cur_Cluster_Table;
	Rec_executer->RatingTable=cur_Rating_Table;

	Rec_executer->make_neighbor_list(user_id);
	int* Rec_Item_list=Rule_applier->rule_applier(user_id,Rec_executer->make_Rec_Item_list(user_id));

	/*
	Rec_Content* Rec_Content_list=new Rec_Content[20];
	for(int i=0;i<20;i++){
		Rec_Content_list[i].content_id=Rec_Item_list[i];
	}
	*/
	return Rec_Item_list;
}