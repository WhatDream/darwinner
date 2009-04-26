///////////////////////////////////////////////////////////
//  Job.cpp
//  Implementation of the Class Job
//  Created on:      17-����-2009 14:02:03
//  Original author: Tim Kuo
///////////////////////////////////////////////////////////

#include "Job.h"


Job::Job(){
	m_description.clear();
	m_id = 0;
	m_name.clear();
	m_profit = 0;
}



Job::~Job(){

}





string Job::description(){

	return  m_description;
}


uint32 Job::id(){

	return  m_id;
}


string Job::name(){

	return  m_name;
}


short Job::profit(){

	return m_profit;
}


void Job::setDescription(string descr){
	m_description = descr;
}


void Job::setId(uint32 id){
	m_id = id;
}


void Job::setName(string name){
	m_name = name;
}


void Job::setProfit(uint32 profit){
	m_profit = profit;
}