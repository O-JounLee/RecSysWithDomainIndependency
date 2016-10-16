#include <iostream>
#include <stdio.h>
#include "structs.h"
#include "Recommendation_Controller.h"
#include "stdafx.h"

using namespace std;

Rec_Controller::Rec_Controller(){
	Rec_Model=new Rec_Model_Manager;
	Rec_recommand_list=new Rec_Manager[20];
}

Rec_Controller::~Rec_Controller(){

}

void Rec_Controller::Time_Interrupt(){
	Rec_Model->Time_Interrupt();

	return;
}

int* Rec_Controller::get_Rec_list(int user_id){
	int* _result=new int[20];
	int num_of_this;

	while(true){
		if(num_Rec_Request==20);
		else{
			num_of_this=num_Rec_Request++;
			_result=Rec_recommand_list[num_of_this-1].get_Rec_list(user_id,Rec_Model->C_Manager->U_Cluster_Table,Rec_Model->R_Manager->Rating_Table);
			delete (Rec_recommand_list+num_of_this-1);
			break;
		}
	}
	for(int i=num_of_this;i<20;i++) Rec_recommand_list[i-1]=Rec_recommand_list[i];
	num_Rec_Request--;

	return _result;
}

void Rec_Controller::Input_Rating(int user_id,int item_id,int user_pref,int item_prior){
	Rec_Model->Input_Rating(user_id,item_id,user_pref,item_prior);

	return;
}

void Rec_Controller::Input_Member(bool Type,int id){
	Rec_Model->Input_Member(Type,id);

	return;
}