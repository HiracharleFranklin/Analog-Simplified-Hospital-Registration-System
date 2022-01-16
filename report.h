#pragma once
#include "alist.cpp"
#include <iostream>
#include <fstream>
#include "person.h"
#include "fifo.h"
#include "fifo.cpp"
#include "fibheap.h"
#include "alist.cpp"
#include "alist.h"
#include "BPlus_tree.cpp"
#include "B_Tree.h"
#include "hashset.cpp"

using namespace std;

void weekly_report(AList<person *> &archive, person ***hospital, FibHeap<long long> &Fib, AList<person *> &middle_risk_archive, AList<person *> &withdraw_reinsertion_archive, int halfday, string filename);
void FibHeap_report(FibHeap<long long> &Fib, int halfday, string filename);
void hospital_report(person ***hospital, int halfday, string filename);
int hospital_total_halfday(person ***hospital, int halfday);
void hospital_display(person ***hospital, int halfday, string filename);
// CA2
void B_report_a_person(B_Tree<Key_tuple> &B_tree, CBPlusTree<int, PERSON> &person_tree, int health_care_card);
void B_report_a_registration(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, REGISTRATION> &registration_tree, int health_care_card);
void B_report_a_medical_status(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, MEDICAL_STATUS> &medical_tree, int health_care_card);
void B_report_a_treatment(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, TREATMENT> &treatment_tree, int health_care_card);
int B_find_id(B_Tree<Key_tuple> &B_tree, int health_care_card);
void risk_status_report_week(int week, CBPlusTree<int, PERSON> &person_tree, CBPlusTree<int, MEDICAL_STATUS> &medical_tree, CBPlusTree<int, REGISTRATION> &registration_tree,  CBPlusTree<int, TREATMENT> &treatment_tree, hashset<MEDICAL_STATUS> &hash_medical);
void risk_status_report_month(int month, CBPlusTree<int, PERSON> &person_tree, CBPlusTree<int, MEDICAL_STATUS> &medical_tree, CBPlusTree<int, REGISTRATION> &registration_tree,  CBPlusTree<int, TREATMENT> &treatment_tree, hashset<MEDICAL_STATUS> &hash_medical);
void weekly_report(AList<person *> &archive, person ***hospital, FibHeap<long long> &Fib, AList<person *> &middle_risk_archive, AList<person *> &withdraw_reinsertion_archive, int halfday, string filename)
{
    ofstream outfile;
    outfile.open(filename, ios::app);
    outfile << " ------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile << "            The people who have been treated during this week\n";
    outfile << endl;
    outfile << " ------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile.close();

    archive.report(halfday, filename);

    outfile.open(filename, ios::app);
    outfile << "\n";
    outfile.close();

    outfile.open(filename, ios::app);
    outfile << " ------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile << "       The registered people with a set appointment during this week\n";
    outfile << endl;
    outfile << " ------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile.close();

    hospital_report(hospital, halfday, filename);

    outfile.open(filename, ios::app);
    outfile << "\n";
    outfile.close();

    outfile.open(filename, ios::app);
    outfile << " ------------------------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile << "   The people stored in the Fib_heap without a set appointment during this week\n";
    outfile << endl;
    outfile << " ------------------------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile.close();

    FibHeap_report(Fib, halfday, filename);

    outfile.open(filename, ios::app);
    outfile << "\n";
    outfile.close();

    outfile.open(filename, ios::app);
    outfile << " ------------------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile << "   The queueing people with middle risk without a set appointment during this week\n";
    outfile << endl;
    outfile << " ------------------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile.close();

    middle_risk_archive.risk_2_report(halfday, filename);

    outfile.open(filename, ios::app);
    outfile << "\n";
    outfile.close();

    outfile.open(filename, ios::app);
    outfile << " ------------------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile << "   The queueing people reinserted without a set appointment during this week\n";
    outfile << endl;
    outfile << " ------------------------------------------------------------------------------------ " << endl;
    outfile << endl;
    outfile.close();

    withdraw_reinsertion_archive.reinsert_report(halfday, filename);

    outfile.open(filename, ios::app);
    outfile << "\n";
    outfile.close();

    return;
}

void monthly_report(int reginum, int waitnum, int waitnum_total, int appointnum, int withdrawnum, int averagetime, string filename)
{
    ofstream outfile;
    outfile.open(filename, ios::app);
    outfile << "The number of people registered:   ";
    outfile << reginum << "." << endl;
    outfile << "The number of registered people who are waiting:   ";
    outfile << waitnum << "." << endl;
    outfile << "Total number of people who are waiting:    ";
    outfile << waitnum_total << "." << endl;
    outfile << "Number of treatment appointments been made:    ";
    outfile << (appointnum) << "." << endl;
    outfile << "The average waiting time:  ";
    outfile << (averagetime / 2) << " days." << endl;
    outfile << "The number of people who withdrew: ";
    outfile << (withdrawnum) << "." << endl;
    outfile.close();
    return;
}

void FibHeap_report(FibHeap<long long> &Fib, int halfday, string filename)
{
    Fib.Display_all(halfday, filename); //Pass the variable halfday in main function into the display function of the Fibonacci heap
}

void hospital_report(person ***hospital, int halfday, string filename)
{

    ofstream outfile;
    outfile.open(filename, ios::app);
    for (int i = 0; i < 3; i++)
    {
        int morning = 9;
        int afternoon = 1;
        outfile << "People who has a set appointment in hospital" << i << "." << endl;
        for (int j = 0; j < 4; j++)
        {
            outfile << "The person with an appointment at " << morning + j << " a.m. at day" << halfday / 2 << " ." << endl;
            if (hospital[i][j] != NULL)
            {
                file_print_person_s1mple(*hospital[i][j], filename);
                outfile << "Waiting time: " << halfday - hospital[i][j]->regist_halfday << " halfday(s)." << endl;
            }
            else
            {
                outfile << "No appointment this time period." << endl;
            }
        }
        for (int j = 0; j < 4; j++)
        {

            outfile << "The person with an appointment at " << afternoon + j << " p.m. at day" << halfday / 2 << " ." << endl;
            if (hospital[i][j + 4] != NULL)
            {
                file_print_person_s1mple(*hospital[i][4 + j], filename);
                outfile << "Waiting time: " << halfday - hospital[i][4 + j]->regist_halfday << " halfday(s)." << endl;
            }
            else
            {
                outfile << "No appointment this time period." << endl;
            }
        }
    }
    outfile.close();
}

int hospital_total_halfday(person ***hospital, int halfday)
{
    int total_halfdays;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (hospital[i][j] != NULL)
            {
                int time = halfday - (*hospital[i][j]).regist_halfday;
                total_halfdays += time;
            }
        }
    }
    return total_halfdays;
}

void hospital_display_id(person ***hospital, int halfday)
{
    for (int i = 0; i < 3; i++)
    {
        cout << "People who has a set appointment in hospital" << i << "." << endl;
        for (int j = 0; j < 4; j++)
        {
            if (hospital[i][j] != NULL)
            {
                cout << "ID: " << hospital[i][j]->id << endl;
            }
            else
            {
                cout << "No appointment this time period." << endl;
            }
        }
        for (int j = 0; j < 4; j++)
        {

            if (hospital[i][j + 4] != NULL)
            {
                cout << "ID: " << hospital[i][j + 4]->id << endl;
            }
            else
            {
                cout << "No appointment this time period." << endl;
            }
        }
    }
}

//CA2
void B_report_a_person(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, PERSON> &person_tree, int health_care_card)
{
    int id = B_find_id(b_tree, health_care_card);
    // cout << "HERE1" << endl;
    PERSON *person = person_tree.getDataHandle(id);
    // cout << "\"person\": " << person << endl;
    // cout << "HERE2" << endl;
    if (person == NULL)
    {
        // cout << "Oh no" << endl;
        cout << "This person does not exists.";
        return;
    }
    PERSON person_to_print = *person;
    cout << "   Personal information about this person: " << endl;
    cout << "   id: " << person_to_print.id << endl;
    cout << "   name: " << person_to_print.name << endl;
    cout << "   address(registration): " << person_to_print.CONTACT_DETAIL[0] << endl;
    cout << "   phone number: " << person_to_print.CONTACT_DETAIL[1] << endl;
    cout << "   WeChat: " << person_to_print.CONTACT_DETAIL[2] << endl;
    cout << "   email: " << person_to_print.CONTACT_DETAIL[3] << endl;
    cout << "   profession: " << person_to_print.profession << endl;
    cout << "   birthday: " << person_to_print.birth[0] << "." << person_to_print.birth[1] << "." << person_to_print.birth[2] << endl;
    cout << "   health care card: " << person_to_print.health_care_card << endl;
}

void B_report_a_medical_status(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, MEDICAL_STATUS> &medical_tree, int health_care_card)
{
    int id = B_find_id(b_tree, health_care_card);
    MEDICAL_STATUS *status = medical_tree.getDataHandle(id);
    if (status == NULL)
    {
        cout << "This person does not exists.";
        return;
    }
    MEDICAL_STATUS medical_status_to_print = *status;
    cout << "Information about this person's medical status: " << endl;
    cout << "   id: " << medical_status_to_print.id << endl;
    cout << "   risk_status: " << medical_status_to_print.risk << endl;
}

void B_report_a_registration(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, REGISTRATION> &registration_tree, int health_care_card)
{
    int id = B_find_id(b_tree, health_care_card);
    REGISTRATION *regist = registration_tree.getDataHandle(id);
    if (regist == NULL)
    {
        
        cout << "This person does not exists.";
        return;
    }
    REGISTRATION registration_to_print = *regist;
    cout << "Information about this person's registration: " << endl;
    cout << "   id: " << registration_to_print.id << endl;
    cout << "   registration: " << registration_to_print.registration << endl;
    cout << "   register_time(year, month, day, hour, minute): " << registration_to_print.register_time[0] << "." << registration_to_print.register_time[1] << "."
         << registration_to_print.register_time[2] << "." << registration_to_print.register_time[3] << "." << registration_to_print.register_time[4] << endl;
    cout << "   hospital: " << registration_to_print.hospital << endl;
    cout << "   registration halfday: " << registration_to_print.regist_halfday << " halfday(s)." << endl;
    if (registration_to_print.withdraw == 1)
    {
        cout << "   whether withdrawn: yes." << endl;
    }
    else
    {
        cout << "   whether withdrawn: no." << endl;
    }
    cout << "   whether withdrawn: " << registration_to_print.withdraw << " halfday(s)." << endl;
    if (registration_to_print.withdraw == 1)
    {
        cout << "   The day he/she withdraw: " << registration_to_print.withdraw_halfday << " halfday(s)." << endl;
    }
    cout << "   The following 2 halfday may not be meaningful, depending on whether he or she is reinserted or moved to risk2 archive." << endl;
    cout << "   The day he/she reinserted: " << registration_to_print.reinsert_halfday << " halfday(s)." << endl;
    cout << "   The day he/she is moved to risk2 archive: " << registration_to_print.risk2halfday << " halfday(s)." << endl;
}

void B_report_a_treatment(B_Tree<Key_tuple> &b_tree, CBPlusTree<int, TREATMENT> &treatment_tree, int health_care_card)
{
    int id = B_find_id(b_tree, health_care_card);
    TREATMENT *treat = treatment_tree.getDataHandle(id);
    if (treat == NULL)
    {
        cout << "This person does not exists.";
        return;
    }
    TREATMENT treatment_to_print = *treat;
    cout << "Information about this person's treatment: " << endl;
    cout << "   id: " << treatment_to_print.id << endl;
    cout << "   treatment type(code): " << treatment_to_print.treatment << endl;
    cout << "   when is he/she treated: " << treatment_to_print.treat_halfday << " halfday(s)." << endl;
    return;
}

int B_find_id(B_Tree<Key_tuple> &b_tree, int health_care_card)
{
    Key_tuple *address = b_tree.getHandle(Key_tuple(health_care_card));
    cout << "in function B_find_id(), get handle success." << endl;
    if (address == NULL)
    {
        cout << "This card number does not exist." << endl;
        return -1;
    }
    else
    {
        return address->m_id;
    }
}


void risk_status_report_week(int week, CBPlusTree<int, PERSON> &person_tree, CBPlusTree<int, MEDICAL_STATUS> &medical_tree, CBPlusTree<int, REGISTRATION> &registration_tree,  CBPlusTree<int, TREATMENT> &treatment_tree, hashset<MEDICAL_STATUS> &hash_medical)
{
    ofstream hashfile; 
    hashfile.open("risk report of week"+ to_string(week-1) +".txt",ios::app);

    hashfile << "+------------------------------------------------------------------------+" << endl
            << "|                                                                        |" << endl
            << "|                          Risk Report of Week "<< week - 1 <<"                         |" << endl
            << "|       CITY MEDICINE AUTO-REGISTRATION AND APPOINTMENT SYSTEM           |" << endl
            << "|                                                                        |" << endl
            << "+------------------------------------------------------------------------+" << endl
            << endl;
            
    vector<vector<int>> risks;
    int risk2_empty = 1;
    vector<int> risk2;
    vector<int> risk0 = *(hash_medical.output_same_risk_file(0)) ;
    vector<int> risk1 = *(hash_medical.output_same_risk_file(1)) ;
    vector<int>* risk2_fake = hash_medical.output_same_risk_file(2);
    if (risk2_fake != NULL)
    {
        risk2 = *risk2_fake;
        risk2_empty = 0;
    }
    vector<int> risk3 = *(hash_medical.output_same_risk_file(3)) ;
    risks.push_back(risk0);
    risks.push_back(risk1);
    risks.push_back(risk2);
    risks.push_back(risk3);
    int counters[4];
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    counters[3] = 0;
    
    for (int ii = 0; ii < 4; ii++)
    {

        hashfile << "======================================================================================="<<endl
            << "                                                                                       "<<endl
            << "                                People with Risk " << ii <<"                                     "<<endl
            << "                                                                                       "<<endl
            << "======================================================================================="<<endl
            <<endl;

        for(int i=0; i<(int)risks[ii].size();i++)
        {
            if(i == 2 && risk2_empty==1){
                hashfile<<"   No person has risk status 2\n";
                continue;
            }
            int id = risks[ii][i];
            PERSON *person = person_tree.getDataHandle(id);
            if (person != NULL)
            {
                PERSON person_to_print = *person;
                hashfile << "   id: " << person_to_print.id << endl;
                hashfile << "   name: " << person_to_print.name << endl;
                hashfile << "   address(registration): " << person_to_print.CONTACT_DETAIL[0] << endl;
                hashfile << "   phone number: " << person_to_print.CONTACT_DETAIL[1] << endl;
                hashfile << "   WeChat: " << person_to_print.CONTACT_DETAIL[2] << endl;
                hashfile << "   email: " << person_to_print.CONTACT_DETAIL[3] << endl;
                hashfile << "   profession: " << person_to_print.profession << endl;
                hashfile << "   birthday: " << person_to_print.birth[0] << "." << person_to_print.birth[1] << "." << person_to_print.birth[2] << endl;
                hashfile << "   health care card: " << person_to_print.health_care_card << endl;
            }
            
            MEDICAL_STATUS *status = medical_tree.getDataHandle(id);
            if (status != NULL)
            {
                MEDICAL_STATUS medical_status_to_print = *status;
                hashfile << "   risk_status: " << medical_status_to_print.risk << endl;
            }

            REGISTRATION *regist = registration_tree.getDataHandle(id);
            if (regist != NULL)
            {
                REGISTRATION registration_to_print = *regist;
                hashfile << "   registration: " << registration_to_print.registration << endl;
                hashfile << "   register_time(year, month, day, hour, minute): " << registration_to_print.register_time[0] << "." << registration_to_print.register_time[1] << "."
                    << registration_to_print.register_time[2] << "." << registration_to_print.register_time[3] << "." << registration_to_print.register_time[4] << endl;
                hashfile << "   hospital: " << registration_to_print.hospital << endl;
                hashfile << "   registration halfday: " << registration_to_print.regist_halfday << " halfday(s)." << endl;
                if (registration_to_print.withdraw == 1)
                {
                    hashfile << "   whether withdrawn: yes." << endl;
                }
                else
                {
                    hashfile << "   whether withdrawn: no." << endl;
                }
                hashfile << "   whether withdrawn: " << registration_to_print.withdraw << " halfday(s)." << endl;
                if (registration_to_print.withdraw == 1)
                {
                    hashfile << "   The day he/she withdraw: " << registration_to_print.withdraw_halfday << " halfday(s)." << endl;
                }
                hashfile << "   The following 2 halfday may not be meaningful, depending on whether he or she is reinserted or moved to risk2 archive." << endl;
                hashfile << "   The day he/she reinserted: " << registration_to_print.reinsert_halfday << " halfday(s)." << endl;
                hashfile << "   The day he/she is moved to risk2 archive: " << registration_to_print.risk2halfday << " halfday(s)." << endl;
            }
            TREATMENT *treat = treatment_tree.getDataHandle(id);
            if (treat != NULL)
            {
                TREATMENT treatment_to_print = *treat;
                hashfile << "   treatment type(code): " << treatment_to_print.treatment << endl;
                hashfile << "   when is he/she treated: " << treatment_to_print.treat_halfday << " halfday(s)." << endl << endl;
            }

            counters[ii] ++;
        }
    }
    hashfile << "The number of people with risk 0: "<< counters[0] << endl;
    hashfile << "The number of people with risk 1: "<< counters[1] << endl;
    hashfile << "The number of people with risk 2: "<< counters[2] << endl;
    hashfile << "The number of people with risk 3: "<< counters[3] << endl;
    double total = counters[0]+counters[1]+counters[2]+counters[3];
    double risk_0_num = counters[0];
    double risk_0_rate = risk_0_num/total*100;
    double risk_1_num = counters[1];
    double risk_1_rate = risk_1_num/total*100;
    double risk_2_num = counters[2];
    double risk_2_rate = risk_2_num/total*100;
    double risk_3_num = counters[3];
    double risk_3_rate = risk_3_num/total*100;
    hashfile << "The rate of people with risk 0: " << risk_0_rate << "%" << endl;
    hashfile << "The rate of people with risk 1: " << risk_1_rate << "%" << endl;
    hashfile << "The rate of people with risk 2: " << risk_2_rate << "%" << endl;
    hashfile << "The rate of people with risk 3: " << risk_3_rate << "%" << endl;
    hashfile << "+------------------------------------------------------------------------+" << endl
        << "|                                                                        |" << endl
        << "|                     Risk Report of Week "<< week - 1 <<" Ends                         |" << endl
        << "|       CITY MEDICINE AUTO-REGISTRATION AND APPOINTMENT SYSTEM           |" << endl
        << "|                                                                        |" << endl
        << "+------------------------------------------------------------------------+" << endl
        << endl;
    hashfile.close();
}


void risk_status_report_month(int month, CBPlusTree<int, PERSON> &person_tree, CBPlusTree<int, MEDICAL_STATUS> &medical_tree, CBPlusTree<int, REGISTRATION> &registration_tree,  CBPlusTree<int, TREATMENT> &treatment_tree, hashset<MEDICAL_STATUS> &hash_medical)
{
    ofstream hashfile; 
    hashfile.open("risk report of month"+ to_string(month-1) +".txt",ios::app);
    hashfile << "+------------------------------------------------------------------------+" << endl
            << "|                                                                        |" << endl
            << "|                         Risk Report of Month "<< month - 1 <<"                         |" << endl
            << "|       CITY MEDICINE AUTO-REGISTRATION AND APPOINTMENT SYSTEM           |" << endl
            << "|                                                                        |" << endl
            << "+------------------------------------------------------------------------+" << endl
            << endl;
            
    vector<vector<int>> risks;
    int risk2_empty = 1;
    vector<int> risk2;
    vector<int> risk0 = *(hash_medical.output_same_risk_file(0)) ;
    vector<int> risk1 = *(hash_medical.output_same_risk_file(1)) ;
    vector<int>* risk2_fake = hash_medical.output_same_risk_file(2);
    if (risk2_fake != NULL)
    {
        risk2 = *risk2_fake;
        risk2_empty = 0;
    }
    vector<int> risk3 = *(hash_medical.output_same_risk_file(3)) ;
    risks.push_back(risk0);
    risks.push_back(risk1);
    risks.push_back(risk2);
    risks.push_back(risk3);
    int counters[4];
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    counters[3] = 0;
    
    for (int ii = 0; ii < 4; ii++)
    {

        hashfile << "======================================================================================="<<endl
            << "                                                                                       "<<endl
            << "                                People with Risk " << ii <<"                                     "<<endl
            << "                                                                                       "<<endl
            << "======================================================================================="<<endl
            <<endl;

        for(int i=0; i<(int)risks[ii].size();i++)
        {
            if(i == 2 && risk2_empty==1){
                hashfile<<"   No person has risk status 2\n";
                continue;
            }
            int id = risks[ii][i];
            PERSON *person = person_tree.getDataHandle(id);
            if (person != NULL)
            {
                PERSON person_to_print = *person;
                hashfile << "   id: " << person_to_print.id << endl;
                hashfile << "   name: " << person_to_print.name << endl;
                hashfile << "   address(registration): " << person_to_print.CONTACT_DETAIL[0] << endl;
                hashfile << "   phone number: " << person_to_print.CONTACT_DETAIL[1] << endl;
                hashfile << "   WeChat: " << person_to_print.CONTACT_DETAIL[2] << endl;
                hashfile << "   email: " << person_to_print.CONTACT_DETAIL[3] << endl;
                hashfile << "   profession: " << person_to_print.profession << endl;
                hashfile << "   birthday: " << person_to_print.birth[0] << "." << person_to_print.birth[1] << "." << person_to_print.birth[2] << endl;
                hashfile << "   health care card: " << person_to_print.health_care_card << endl;
            }
            
            MEDICAL_STATUS *status = medical_tree.getDataHandle(id);
            if (status != NULL)
            {
                MEDICAL_STATUS medical_status_to_print = *status;
                hashfile << "   risk_status: " << medical_status_to_print.risk << endl;
            }

            REGISTRATION *regist = registration_tree.getDataHandle(id);
            if (regist != NULL)
            {
                REGISTRATION registration_to_print = *regist;
                hashfile << "   registration: " << registration_to_print.registration << endl;
                hashfile << "   register_time(year, month, day, hour, minute): " << registration_to_print.register_time[0] << "." << registration_to_print.register_time[1] << "."
                    << registration_to_print.register_time[2] << "." << registration_to_print.register_time[3] << "." << registration_to_print.register_time[4] << endl;
                hashfile << "   hospital: " << registration_to_print.hospital << endl;
                hashfile << "   registration halfday: " << registration_to_print.regist_halfday << " halfday(s)." << endl;
                if (registration_to_print.withdraw == 1)
                {
                    hashfile << "   whether withdrawn: yes." << endl;
                }
                else
                {
                    hashfile << "   whether withdrawn: no." << endl;
                }
                hashfile << "   whether withdrawn: " << registration_to_print.withdraw << " halfday(s)." << endl;
                if (registration_to_print.withdraw == 1)
                {
                    hashfile << "   The day he/she withdraw: " << registration_to_print.withdraw_halfday << " halfday(s)." << endl;
                }
                hashfile << "   The following 2 halfday may not be meaningful, depending on whether he or she is reinserted or moved to risk2 archive." << endl;
                hashfile << "   The day he/she reinserted: " << registration_to_print.reinsert_halfday << " halfday(s)." << endl;
                hashfile << "   The day he/she is moved to risk2 archive: " << registration_to_print.risk2halfday << " halfday(s)." << endl;
            }
            TREATMENT *treat = treatment_tree.getDataHandle(id);
            if (treat != NULL)
            {
                TREATMENT treatment_to_print = *treat;
                hashfile << "   treatment type(code): " << treatment_to_print.treatment << endl;
                hashfile << "   when is he/she treated: " << treatment_to_print.treat_halfday << " halfday(s)." << endl << endl;
            }

            counters[ii] ++;
        }
    }
    hashfile << "The number of people with risk 0: "<< counters[0] << endl;
    hashfile << "The number of people with risk 1: "<< counters[1] << endl;
    hashfile << "The number of people with risk 2: "<< counters[2] << endl;
    hashfile << "The number of people with risk 3: "<< counters[3] << endl;
    double total = counters[0]+counters[1]+counters[2]+counters[3];
    double risk_0_num = counters[0];
    double risk_0_rate = risk_0_num/total*100;
    double risk_1_num = counters[1];
    double risk_1_rate = risk_1_num/total*100;
    double risk_2_num = counters[2];
    double risk_2_rate = risk_2_num/total*100;
    double risk_3_num = counters[3];
    double risk_3_rate = risk_3_num/total*100;
    hashfile << "The rate of people with risk 0: " << risk_0_rate << "%" << endl;
    hashfile << "The rate of people with risk 1: " << risk_1_rate << "%" << endl;
    hashfile << "The rate of people with risk 2: " << risk_2_rate << "%" << endl;
    hashfile << "The rate of people with risk 3: " << risk_3_rate << "%" << endl;
    hashfile << "+------------------------------------------------------------------------+" << endl
        << "|                                                                        |" << endl
        << "|                    Risk Report of Month "<< month - 1 <<" Ends                         |" << endl
        << "|       CITY MEDICINE AUTO-REGISTRATION AND APPOINTMENT SYSTEM           |" << endl
        << "|                                                                        |" << endl
        << "+------------------------------------------------------------------------+" << endl
        << endl;
    hashfile.close();
}