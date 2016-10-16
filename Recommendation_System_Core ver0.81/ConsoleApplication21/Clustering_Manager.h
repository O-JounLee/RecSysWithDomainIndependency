//#include "stdafx.h"
//#include "structs.h"
//#include "Recommendation_Model_Manager.h"
//#include "Recommendation_Controller.h"

#ifndef CLUSTERING_MANAGER_H
#define CLUSTERING_MANAGER_H

class Rec_Controller;
class Rec_Model_Manager;

class Cluster_Manager{
public:
	friend Rec_Controller;
	friend Rec_Model_Manager;

	Cluster_Manager(int num_user,int num_item);
	~Cluster_Manager();

	void Make_U_ClusterTable();
	void Make_I_ClusterTable();

	int K_decision(bool type);
	void Create_Cluster(bool type);
	int* make_cluster(bool type,const int num_of_cluster,int num);
	float internal_deviation(bool type,int* target,int num_of_cluster,int num);

	void add_User(int User_id);
	void add_Item(int Item_id);

	void adjust_user_cluster(int user_id);
	void adjust_item_cluster(int item_id);

	MemberVector sum(bool type,int* target,int cluster_num,int num);
	MemberVector mean(bool type,int* target,int cluster_num,int num);
	MemberVector deviation(bool type,int* target,int cluster_num,int num);
	int count(int* target,int cluster_num,int num);
	float probabillity_density(bool type,int id,MemberVector m,MemberVector d);

private: 
	Cluster_Info* User_Cluster_Info_list;
	Cluster_Info* Item_Cluster_Info_list;

	Cluster_Info** temp_Clusters_Info;

	int* U_Cluster_Table;
	int* I_Cluster_Table;

	int num_of_user;
	int num_of_item;

	int num_of_user_cluster;
	int num_of_item_cluster;

};

#endif CLUSTERING_MANAGER_H