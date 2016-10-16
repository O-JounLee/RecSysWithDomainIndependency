#pragma comment(lib, "mysqlcppconn.lib")
#pragma comment(lib, "libmysql.dll")


#include <iostream>
#include <stdio.h>
#include "Intra_Interface.h"
#include "structs.h"
#include "math.h"
#include "Clustering_Manager.h"
#include "stdafx.h"

using namespace std;

Cluster_Manager::Cluster_Manager(sql::Connection* con,int num_user,int num_item){
	_con=con;

	num_of_user=num_user;
	num_of_item=num_item;
}

Cluster_Manager::~Cluster_Manager(){

}

void Cluster_Manager::Make_U_ClusterTable(){
	User_Cluster_Info_list=new Cluster_Info[1];
	U_Cluster_Table=new int[num_of_user];
	for(int i=0;i<num_of_user;i++) U_Cluster_Table[i]=NULL;

	return;
}

void Cluster_Manager::Make_I_ClusterTable(){
	Item_Cluster_Info_list=new Cluster_Info[1];
	I_Cluster_Table=new int[num_of_item];
	for(int i=0;i<num_of_item;i++) I_Cluster_Table[i]=NULL;

	return;
}


int Cluster_Manager::K_decision(bool type){
	int _result=0;
	int* temp_Cluster;
	float min_d=999999999.0f;

	if(type){
		for(int i=1;i<log10((double)num_of_user)+10;i++){
			if(i>num_of_user) break;

			cout<<"Start make "<< i <<"temp cluster \n";
			temp_Cluster=make_cluster(type,i,num_of_user);

			float i_deviation=internal_deviation(type,temp_Cluster,i,num_of_user);
			cout<<i<<" i_deviation is "<< i_deviation <<" \n";

			//for(int j=0;j<i;j++) if(count(temp_Cluster,j,num_of_user)==0) i_deviation=999999999.0f;

			//delete temp_Cluster;

			if(min_d>i_deviation){
				min_d=i_deviation;
				_result=i;
			}
		}
	}
	else{
		for(int i=1;i<log10((double)num_of_item)+10;i++){
			if(i>num_of_item) break;

			cout<<"Start make "<< i <<"temp cluster \n";
			temp_Cluster=make_cluster(type,i,num_of_item);

			float i_deviation=internal_deviation(type,temp_Cluster,i,num_of_item);
			cout<<i<<" i_deviation is "<< i_deviation <<" \n";

			//for(int j=0;j<i;j++) if(count(temp_Cluster,j,num_of_item)==0) i_deviation=999999999.0f;

			//delete temp_Cluster;

			if(min_d>i_deviation){
				min_d=i_deviation;
				_result=i;
			}
		}
	}
	
	return _result;
}

float Cluster_Manager::internal_deviation(bool type,int* target,int num_of_cluster,int num){
	float _result=0.0f;
	int c_num=0;

	/*
	if(type){
		for(int i=0;i<num_of_cluster;i++){
			MemberVector d=deviation(true,target,i,num);
			for(int j=0;j<5;j++) _result=_result+d.Element[j];
		}
		delete target;
	}
	else{
		for(int i=0;i<num_of_cluster;i++){
			MemberVector d=deviation(false,target,i,num);
			for(int j=0;j<5;j++) _result=_result+d.Element[j];
		}
		delete target;
	}
	*/
	
	for(int i=0;i<num_of_cluster;i++){
		MemberVector d=deviation(type,target,i,num);
		for(int j=0;j<5;j++){
			if(count(target,i,num_of_user)!=0){
				_result=_result+d.Element[j];
				c_num++;
			}
		}
	}
	delete target;

	_result=_result/c_num;

	return _result;
}

void Cluster_Manager::Create_Cluster(bool type){
	MemberVector* mv = new MemberVector;

	if(type){
		cout<<"Start K_decision \n";
		num_of_user_cluster=K_decision(type);
		cout<<"num_of_user_cluster is "<< num_of_user_cluster <<" \n";
		if(num_of_user_cluster==0) cout<<"ERROR \n";

		delete []User_Cluster_Info_list;
		User_Cluster_Info_list=new Cluster_Info[num_of_user_cluster];

		cout<<"Start make usable user cluster \n";
		int* _result=new int[num_of_user];
		for(int i=0;i<num_of_user;i++) _result[i]=0;

		float* prob=new float[num_of_user_cluster];
		for(int i=0;i<num_of_user_cluster;i++) prob[i]=1.0f/(float)num_of_user_cluster;
		MemberVector* m=new MemberVector[num_of_user_cluster];
		//for(int i=0;i<k;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(_con,1,i,mv)->Element[j];
		for(int i=0;i<num_of_user_cluster;i++) m[i]=*get_MemberVector(_con,type,i,mv);
		MemberVector* d=new MemberVector[num_of_user_cluster];
		//for(int i=0;i<k;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(_con,1,i,mv)->Element[j];
		for(int i=0;i<num_of_user_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=1;

		float** pd=new float*[num_of_user];
		for(int i=0;i<num_of_user;i++) *(pd+i)=new float[(int)num_of_user_cluster];

		for(int i=0;i<10;i++){
			//cout<<"Iteration Num is "<< i <<" \n";
			for(int j=0;j<num_of_user_cluster;j++)
				for(int l=0;l<num_of_user;l++) *(*(pd+l)+j)=probabillity_density(type,l,m[j],d[j])*prob[j];

			for(int j=0;j<num_of_user;j++){
				float mom=0,max=0;
				float* G_prob=new float[num_of_user_cluster];

				for(int l=0;l<num_of_user_cluster;l++) mom=mom+*(*(pd+j)+l)*prob[l];
				for(int l=0;l<num_of_user_cluster;l++) G_prob[l]=*(*(pd+j)+l)*prob[l]/mom;

				//int cluster;
				for(int l=0;l<num_of_user_cluster;l++){
					if(G_prob[l]>max){
						max=G_prob[l];
						_result[j]=l;
					}
				}
				delete G_prob;
			}
			for(int i=0;i<num_of_user_cluster;i++) prob[i]=(float)count(_result,i,num_of_user)/(float)num_of_user;
			for(int i=0;i<num_of_user_cluster;i++) m[i]=mean(type,_result,i,num_of_user);
			for(int i=0;i<num_of_user_cluster;i++) d[i]=deviation(type,_result,i,num_of_user);
		}

		for(int i=0;i<num_of_user;i++){
			U_Cluster_Table[i]=_result[i];
			//cout<<U_Cluster_Table[i];
		}

		for(int i=0;i<num_of_user_cluster;i++){
			User_Cluster_Info_list[i].mean=m[i];
			User_Cluster_Info_list[i].s_deviation=d[i];
			User_Cluster_Info_list[i].prob=prob[i];
		}

		/*
		for(int i=0;i<k;i++){
			for(int j=0;j<5;j++){
				User_Cluster_Info_list[i].mean.Element[j]=m[i].Element[j];
				User_Cluster_Info_list[i].s_deviation.Element[j]=d[i].Element[j];
			}
			User_Cluster_Info_list[i].prob=prob[i];
		}
		*/

		delete _result;
		delete []m;
		delete []d;
		delete []prob;

		for(int i=0;i<num_of_user;i++) delete []*(pd+i);
		delete []pd;

	}
	else{
		cout<<"Start K_decision \n";
		num_of_item_cluster=K_decision(type);
		cout<<"K is "<< num_of_item_cluster <<" \n";
		if(num_of_item_cluster==0) cout<<"ERROR \n";

		delete []Item_Cluster_Info_list;
		Item_Cluster_Info_list=new Cluster_Info[num_of_item_cluster];

		cout<<"Start make usable item cluster \n";
		int* _result=new int[num_of_item];
		for(int i=0;i<num_of_item;i++) _result[i]=0;

		float* prob=new float[num_of_item_cluster];
		for(int i=0;i<num_of_item_cluster;i++) prob[i]=1.0f/(float)num_of_item_cluster;
		MemberVector* m=new MemberVector[num_of_item_cluster];
		//for(int i=0;i<k;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(_con,0,i,mv)->Element[j];
		for(int i=0;i<num_of_item_cluster;i++) m[i]=*get_MemberVector(_con,type,i,mv);
		MemberVector* d=new MemberVector[num_of_item_cluster];
		//for(int i=0;i<k;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(_con,0,i,mv)->Element[j];
		for(int i=0;i<num_of_item_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=1;

		float** pd=new float*[num_of_item];
		for(int i=0;i<num_of_item;i++) *(pd+i)=new float[num_of_item_cluster];

		for(int i=0;i<10;i++){
			//cout<<"Iteration Num is "<< i <<" \n";
			for(int j=0;j<num_of_item_cluster;j++)
				for(int l=0;l<num_of_item;l++) *(*(pd+l)+j)=probabillity_density(type,l,m[j],d[j])*prob[j];

			for(int j=0;j<num_of_item;j++){
				float mom=0,max=0;
				float* G_prob=new float[num_of_item_cluster];

				for(int l=0;l<num_of_item_cluster;l++) mom=mom+*(*(pd+j)+l)*prob[l];
				for(int l=0;l<num_of_item_cluster;l++) G_prob[l]=*(*(pd+j)+l)*prob[l]/mom;

				//int cluster;
				for(int l=0;l<num_of_item_cluster;l++){
					if(G_prob[l]>max){
						max=G_prob[l];
						_result[j]=l;
					}
				}
				delete G_prob;
			}
			for(int i=0;i<num_of_item_cluster;i++) prob[i]=(float)count(_result,i,num_of_item)/(float)num_of_item;
			for(int i=0;i<num_of_item_cluster;i++) m[i]=mean(type,_result,i,num_of_item);
			for(int i=0;i<num_of_item_cluster;i++) d[i]=deviation(type,_result,i,num_of_item);
		}

		for(int i=0;i<num_of_item;i++){
			I_Cluster_Table[i]=_result[i];
			//cout<<I_Cluster_Table[i];
		}
		
		for(int i=0;i<num_of_item_cluster;i++){
			Item_Cluster_Info_list[i].mean=m[i];
			Item_Cluster_Info_list[i].s_deviation=d[i];
			Item_Cluster_Info_list[i].prob=prob[i];
		}

		delete _result;
		delete []m;
		delete []d;
		delete []prob;

		for(int i=0;i<num_of_item;i++) delete []*(pd+i);
		delete []pd;

	}
	delete mv;

	return;
}

int* Cluster_Manager::make_cluster(bool type,const int num_of_cluster,int num){
	int* _result=new int[num];
	for(int i=0;i<num;i++) _result[i]=0;
	float* prob=new float[num_of_cluster];
	for(int i=0;i<num_of_cluster;i++) prob[i]=1/(float)num_of_cluster;

	MemberVector* mv = new MemberVector;

	/*
	if(type){
		MemberVector* m=new MemberVector[num_of_cluster];
		//for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(_con,1,i,mv)->Element[j];
		for(int i=0;i<num_of_cluster;i++) m[i]=*get_MemberVector(_con,type,i,mv);
		MemberVector* d=new MemberVector[num_of_cluster];
		//for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(_con,1,i,mv)->Element[j];
		for(int i=0;i<num_of_cluster;i++) d[i]=*get_MemberVector(_con,type,i,mv);

		float** pd=new float*[num];
		for(int i=0;i<num;i++) *(pd+i)=new float[num_of_cluster];

		for(int i=0;i<100;i++){
			for(int j=0;j<num_of_cluster;j++) for(int l=0;l<num;l++) *(*(pd+l)+j)=probabillity_density(type,l,m[j],d[j])*prob[j];
			for(int j=0;j<num;j++){
				float max=0;
				//int cluster;
				for(int l=0;l<num_of_cluster;l++){
					if(*(*(pd+j)+l)>max){
						max=*(*(pd+j)+l);
						_result[j]=l;
					}
				}
			}
			for(int i=0;i<num_of_cluster;i++) prob[i]=(float)count(_result,i,num)/(float)num;
			for(int i=0;i<num_of_cluster;i++) m[i]=mean(type,_result,i,num);
			for(int i=0;i<num_of_cluster;i++) d[i]=deviation(type,_result,i,num);
		}

		delete []m;
		delete []d;
		delete []prob;

		for(int i=0;i<num;i++) delete []*(pd+i);
		delete []pd;
	}
	else{
		MemberVector* m=new MemberVector[num_of_cluster];
		//for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(_con,0,i,mv)->Element[j];
		for(int i=0;i<num_of_cluster;i++) m[i]=*get_MemberVector(_con,type,i,mv);
		MemberVector* d=new MemberVector[num_of_cluster];
		//for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(_con,0,i,mv)->Element[j];
		for(int i=0;i<num_of_cluster;i++) d[i]=*get_MemberVector(_con,type,i,mv);

		float** pd=new float*[num];
		for(int i=0;i<num;i++) *(pd+i)=new float[num_of_cluster];

		for(int i=0;i<100;i++){
			for(int j=0;j<num_of_cluster;j++) for(int l=0;l<num;l++) *(*(pd+l)+j)=probabillity_density(type,l,m[j],d[j])*prob[j];
			for(int j=0;j<num;j++){
				float max=0;
				//int cluster;
				for(int l=0;l<num_of_cluster;l++){
					if(*(*(pd+j)+l)>max){
						max=*(*(pd+j)+l);
						_result[j]=l;
					}
				}
			}
			for(int i=0;i<num_of_cluster;i++) prob[i]=(float)count(_result,i,num)/(float)num;
			for(int i=0;i<num_of_cluster;i++) m[i]=mean(type,_result,i,num);
			for(int i=0;i<num_of_cluster;i++) d[i]=deviation(type,_result,i,num);
		}

		delete []m;
		delete []d;
		delete []prob;

		for(int i=0;i<num;i++) delete []*(pd+i);
		delete []pd;
	}
	*/

	MemberVector* m=new MemberVector[num_of_cluster];
	//for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) m[i].Element[j]=get_MemberVector(_con,0,i,mv)->Element[j];
	for(int i=0;i<num_of_cluster;i++) m[i]=*get_MemberVector(_con,type,i,mv);
	MemberVector* d=new MemberVector[num_of_cluster];
	//for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=get_MemberVector(_con,0,i,mv)->Element[j];
	for(int i=0;i<num_of_cluster;i++) for(int j=0;j<5;j++) d[i].Element[j]=1;

	float** pd=new float*[num];
	for(int i=0;i<num;i++) *(pd+i)=new float[num_of_cluster];

	for(int i=0;i<10;i++){
		for(int j=0;j<num_of_cluster;j++)
			for(int l=0;l<num;l++) *(*(pd+l)+j)=probabillity_density(type,l,m[j],d[j]);
		
		for(int j=0;j<num;j++){
			float mom=0,max=0;
			float* G_prob=new float[num_of_cluster];

			for(int l=0;l<num_of_cluster;l++) mom=mom+*(*(pd+j)+l)*prob[l];
			for(int l=0;l<num_of_cluster;l++) G_prob[l]=*(*(pd+j)+l)*prob[l]/mom;
			
			//int cluster;
			for(int l=0;l<num_of_cluster;l++){
				if(G_prob[l]>max){
					max=G_prob[l];
					_result[j]=l;
				}
			}
			delete G_prob;
		}
		for(int i=0;i<num_of_cluster;i++) prob[i]=(float)count(_result,i,num)/(float)num;
		for(int i=0;i<num_of_cluster;i++) m[i]=mean(type,_result,i,num);
		for(int i=0;i<num_of_cluster;i++) d[i]=deviation(type,_result,i,num);

		/*
		for(int i=0;i<num_of_cluster;i++){
			prob[i]=(float)count(_result,i,num)/(float)num;
			cout<<prob[i]<<" \n";
		}
		for(int i=0;i<num_of_cluster;i++){
			m[i]=mean(type,_result,i,num);
			cout<<m[i].Element[0]<<" "<<m[i].Element[1]<<" "<<m[i].Element[2]<<" "<<m[i].Element[3]<<" "<<m[i].Element[4]<<" \n";
		}
		for(int i=0;i<num_of_cluster;i++){
			d[i]=deviation(type,_result,i,num);
			cout<<d[i].Element[0]<<" "<<d[i].Element[1]<<" "<<d[i].Element[2]<<" "<<d[i].Element[3]<<" "<<d[i].Element[4]<<" \n";
		}
		cout<<"\n";
		*/
	}

	delete []m;
	delete []d;
	delete []prob;

	for(int i=0;i<num;i++) delete []*(pd+i);
	delete []pd;

	delete mv;

	return _result;
}

int Cluster_Manager::adjust_user_cluster(int user_id){
	float* pd=new float[num_of_user_cluster];
	float* prob=new float[num_of_user_cluster];
	MemberVector* m=new MemberVector[num_of_user_cluster];
	MemberVector* d=new MemberVector[num_of_user_cluster];
	int cluster;

	for(int i=0;i<num_of_user_cluster;i++){
		m[i]=User_Cluster_Info_list[i].mean;
		d[i]=User_Cluster_Info_list[i].s_deviation;
		prob[i]=User_Cluster_Info_list[i].prob;
		pd[i]=probabillity_density(true,user_id,m[i],d[i]);
	}

	float mom=0,max=0;
	float* G_prob=new float[num_of_user_cluster];

	for(int l=0;l<num_of_user_cluster;l++) mom=mom+pd[l]*prob[l];
	for(int l=0;l<num_of_user_cluster;l++) G_prob[l]=pd[l]*prob[l]/mom;

	for(int l=0;l<num_of_user_cluster;l++){
		if(G_prob[l]>max){
			max=G_prob[l];
			cluster=l;
		}
	}

	delete pd;
	delete prob;
	delete m;
	delete d;
	delete G_prob;

	return cluster;
}

int Cluster_Manager::adjust_item_cluster(int item_id){
	float* pd=new float[num_of_item_cluster];
	float* prob=new float[num_of_item_cluster];
	MemberVector* m=new MemberVector[num_of_item_cluster];
	MemberVector* d=new MemberVector[num_of_item_cluster];
	int cluster;

	for(int i=0;i<num_of_item_cluster;i++){
		m[i]=Item_Cluster_Info_list[i].mean;
		d[i]=Item_Cluster_Info_list[i].s_deviation;
		prob[i]=Item_Cluster_Info_list[i].prob;
		pd[i]=probabillity_density(false,item_id,m[i],d[i]);
	}

	float mom=0,max=0;
	float* G_prob=new float[num_of_item_cluster];

	for(int l=0;l<num_of_item_cluster;l++) mom=mom+pd[l]*prob[l];
	for(int l=0;l<num_of_item_cluster;l++) G_prob[l]=pd[l]*prob[l]/mom;

	for(int l=0;l<num_of_item_cluster;l++){
		if(G_prob[l]>max){
			max=G_prob[l];
			cluster=l;
		}
	}

	delete pd;
	delete prob;
	delete m;
	delete d;
	delete G_prob;

	return cluster;
}

float Cluster_Manager::probabillity_density(bool type,int id,MemberVector m,MemberVector d){
	float _result=1;
	//UserVector m=mean(target,cluster_num,num),d=deviation(target,cluster_num,num);

	MemberVector* mv = new MemberVector;
	get_MemberVector(_con,type,id,mv);

	/*
	if(type){
		for(int i=0;i<5;i++)
			_result=(float)(_result*(1/sqrt(2*3.14159265)*d.Element[i])*exp(-1*(mv->Element[i]-m.Element[i])*(mv->Element[i]-m.Element[i])/(2*d.Element[i]*d.Element[i])));
	}
	else{
		for(int i=0;i<5;i++)
			_result=(float)(_result*(1/sqrt(2*3.14159265)*d.Element[i])*exp(-1*(mv->Element[i]-m.Element[i])*(mv->Element[i]-m.Element[i])/(2*d.Element[i]*d.Element[i])));
	}
	*/

	for(int i=0;i<5;i++){
		double _devi=d.Element[i];
		double _haed=1/(sqrt(2*3.14159265)*_devi)*10;
		double _tail=exp(-1*(mv->Element[i]-m.Element[i])*(mv->Element[i]-m.Element[i])/(2*d.Element[i]*d.Element[i]))*10;

		//cout<<_devi<<" ";
		//cout<<_haed<<" ";
		//cout<<_tail<<" ";

		_result=_result*_haed*_tail;

		//cout<<_result<<"\n";
	}

	delete mv;

	//if(_result<=0) cout<<"pd ERROR \n";
	//cout<<" "<<_result<<"\n";

	return _result;
}

MemberVector Cluster_Manager::sum(bool type,int* target,int cluster_num,int num){
	MemberVector _result;
	for(int i=0;i<5;i++) _result.Element[i]=0;

	MemberVector* mv = new MemberVector;

	/*
	if(type){
		for(int i=0;i<num;i++){
			if(target[i]==cluster_num){
				get_MemberVector(_con,type,i,mv);
				for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+mv->Element[j];
			}
		}
	}
	else{
		for(int i=0;i<num;i++){
			if(target[i]==cluster_num){
				get_MemberVector(_con,type,i,mv);
				for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+mv->Element[j];
			}
		}
	}
	*/

	for(int i=0;i<num;i++){
		if(target[i]==cluster_num){
			get_MemberVector(_con,type,i,mv);
			for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+mv->Element[j];
		}
	}

	delete mv;

	return _result;
}

MemberVector Cluster_Manager::mean(bool type,int* target,int cluster_num,int num){
	MemberVector _result;
	for(int i=0;i<5;i++) _result.Element[i]=0;

	/*
	if(type){
		int cnt=count(target,cluster_num,num);
		if(cnt==0) cnt=1;
		for(int i=0;i<5;i++) _result.Element[i]=sum(type,target,cluster_num,num).Element[i]/cnt;
	}
	else{
		int cnt=count(target,cluster_num,num);
		if(cnt==0) cnt=1;
		for(int i=0;i<5;i++) _result.Element[i]=sum(type,target,cluster_num,num).Element[i]/cnt;
	}
	*/

	int cnt=count(target,cluster_num,num);
	if(cnt==0) cnt=1;

	for(int i=0;i<5;i++) _result.Element[i]=sum(type,target,cluster_num,num).Element[i]/cnt;
	
	return _result;
}

MemberVector Cluster_Manager::deviation(bool type,int* target,int cluster_num,int num){
	MemberVector _result;
	for(int i=0;i<5;i++) _result.Element[i]=0;

	MemberVector* mv = new MemberVector;

	/*
	if (type){ 
		MemberVector m=mean(type,target,cluster_num,num);
		for(int i=0;i<num;i++){
			if(target[i]==cluster_num){
				get_MemberVector(_con,type,i,mv);
				for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+((m.Element[j]-mv->Element[j])*(m.Element[j]-mv->Element[j]));
			}
		}
		int cnt=count(target,cluster_num,num);
		if(cnt==0) cnt=1;
		for(int i=0;i<5;i++) _result.Element[i]=sqrt(_result.Element[i]/cnt);
	}
	else{
		MemberVector m=mean(type,target,cluster_num,num);
		for(int i=0;i<num;i++){
			if(target[i]==cluster_num){
				get_MemberVector(_con,type,i,mv);
				for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+((m.Element[j]-mv->Element[j])*(m.Element[j]-mv->Element[j]));
			}
		}
		int cnt=count(target,cluster_num,num);
		if(cnt==0) cnt=1;
		for(int i=0;i<5;i++) _result.Element[i]=sqrt(_result.Element[i]/cnt);
	}
	*/

	MemberVector m=mean(type,target,cluster_num,num);
	for(int i=0;i<num;i++){
		if(target[i]==cluster_num){
			get_MemberVector(_con,type,i,mv);
			for(int j=0;j<5;j++) _result.Element[j]=_result.Element[j]+((m.Element[j]-mv->Element[j])*(m.Element[j]-mv->Element[j]));
		}
	}

	int cnt=count(target,cluster_num,num);
	if(cnt==0){
		cnt=1;
		//cout<<"C ERROR";
	}

	for(int i=0;i<5;i++) _result.Element[i]=sqrt(_result.Element[i]/cnt);

	for(int i=0;i<5;i++){
		if(_result.Element[i]<=0) //cout<<"d ERROR \n";
		if(_result.Element[i]==0) _result.Element[i]=1;
	}

	delete mv;

	return _result;
}

int Cluster_Manager::count(int* target,int cluster_num,int num){
	int _result=0;
	for(int i=0;i<num;i++) if(target[i]==cluster_num) _result++;

	return _result;
}

void Cluster_Manager::add_User(int User_id){
	num_of_user++;
	int* temp=new int[num_of_user];

	for(int i=0;i<num_of_user-1;i++) temp[i]=U_Cluster_Table[i];
	temp[num_of_user-1]=adjust_user_cluster(num_of_user-1);

	delete U_Cluster_Table;
	U_Cluster_Table=temp;

	return;
}

void Cluster_Manager::add_Item(int Item_id){
	num_of_item++;
	int* temp=new int[num_of_item];

	for(int i=0;i<num_of_item-1;i++) temp[i]=I_Cluster_Table[i];
	temp[num_of_item-1]=adjust_item_cluster(num_of_item-1);

	delete I_Cluster_Table;
	I_Cluster_Table=temp;

	return;
}