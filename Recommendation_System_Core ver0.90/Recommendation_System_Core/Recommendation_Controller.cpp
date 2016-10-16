#include <iostream>
#include <stdio.h>
#include "structs.h"
#include "Recommendation_Controller.h"
#include "stdafx.h"

using namespace std;

Rec_Controller::Rec_Controller(sql::Connection* con){
	_con=con;

	cout<<"Start making Rec_Model_Manager \n";
	Rec_Model=new Rec_Model_Manager(_con);
	cout<<"Start making Rec_Manager \n";
	Rec_recommand_list=new Rec_Manager(_con);

}

Rec_Controller::~Rec_Controller(){

}

void Rec_Controller::Time_Interrupt(){
	Rec_Model->Time_Interrupt();

	return;
}

int* Rec_Controller::get_Rec_list(int user_id){
	int* _result=new int[20];
	//int num_of_this;

	/*
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
	*/

	_result=Rec_recommand_list->get_Rec_list(user_id,(Rec_Model->C_Manager->U_Cluster_Table),(Rec_Model->R_Manager->Rating_Table),false);

	return _result;
}

void Rec_Controller::Input_Rating(int log_num){
	Rec_Model->Input_Rating(log_num);

	return;
}

void Rec_Controller::Input_Member(bool Type,int id){
	Rec_Model->Input_Member(Type,id);

	return;
}

/*
 * 사용자 수정 시 추천 시스템에서 알고 있어야 할 듯 하여 멤버 함수 추가하였음
 * (Recommendation_controller.h 참조)
 * 구현 바람
 * from SJ
 */
void Rec_Controller::modify_member(bool type, int id){
	return;
}