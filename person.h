#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>

using namespace std;
struct person
{
    //Attribute for CA1
    long long key;              //The priority level of person
    long long id;               //Identification number of the person
    long long registration;     
    string name;                //Name of the person
    string CONTACT_DETAIL[4];   //The array for storing the address, phone, Wechat and email of the person
    long long profession;       //The profession of the person(I to VIII)
                                //1 stands for most important, 8 stands for least important
    long long birth[3];         //The date of birth of the person. The format is [year(yyyy),month(mm),day(dd)]
    long long register_time[5]; //The timestamp each person receive
                                //The time when the patient register [year(yyyy),month(mm),day(dd),hour(hh),minute(mm)]
    long long risk;             //The medical risk level of the person
    int hospital;               //The nearest hospital
    int regist_halfday;         //The number of halfday the patient goes into the queue, i.e. the information went into the system
    int treat_halfday;          //The number of halfday the patient get treated
    int withdraw;
    int withdraw_halfday;
    int reinsert_halfday;
    int risk2halfday;
    long long ddlletter;        //The letter witten by the patient for improving priority   

    //New added part for CA2
    long long treatment;
    int Euclidean_address[2];
    int health_care_card;

    //Some member functions
    void calculate_key();
    void calculate_registertime(long long startyear, long long startmonth, long long startday, long long starthour, long long halfday);
    void mark_regist_halfday(int halfday);
    void mark_treat_halfday(int halfday);
    void print_person();
    int calculate_waiting_halfday(int halfday);
    int calculate_age_category(void);
};

class Key_tuple
{
public:
    Key_tuple() = default;
    Key_tuple(int health_care_card, int id)
    {
        this->m_health_care_card = health_care_card;
        this->m_id = id;
    }
    Key_tuple(int health_care_card)
    {
        this->m_health_care_card = health_care_card;
        this->m_id = 0;
    }

    bool operator<(const Key_tuple &kt1) const
    {
        return m_health_care_card < kt1.m_health_care_card;
    }

    bool operator>(const Key_tuple &kt1) const
    {
        return m_health_care_card > kt1.m_health_care_card;
    }

    bool operator==(const Key_tuple &kt1) const
    {
        return m_health_care_card == kt1.m_health_care_card;
    }

    friend ostream &operator<<(ostream &os, Key_tuple &t1)
    {
        cout << "(health card: " << t1.m_health_care_card << ", id: " << t1.m_id << ")" << endl;
        return os;
    }

    int m_health_care_card;
    int m_id;
};

struct PERSON
{

    //some attributes
    int id;
    string name;                //Name of the person
    string CONTACT_DETAIL[4];   //The array for storing the address, phone, WeChat and email of the person
    long long profession;       //The profession of the person(I to VIII)
                                //1 stands for most important, 8 stands for least important
    long long birth[3];         //The date of birth of the person. The format is [year(yyyy),month(mm),day(dd)]
    int health_care_card;       //9 digits
    
};

struct MEDICAL_STATUS
{
    int id;
    long long risk;
};

struct REGISTRATION
{
    int id;
    long long registration;
    long long register_time[5]; //The timestamp each person receive
                                //The time when the patient register [year(yyyy),month(mm),day(dd),hour(hh),minute(mm)]
    int hospital;               //The nearest hospital
    int regist_halfday;         //The number of halfday the patient goes into the queue, i.e. the information went into the system
    int withdraw;
    int withdraw_halfday;
    int reinsert_halfday;
    int risk2halfday;
};

struct TREATMENT
{
    int id;
    long long treatment;
    int treat_halfday;          //The number of halfday the patient get treated
};

struct DISTANCE
{
    int id;
    long long treatment;
    int Euclidean_address[2];
    int neighbour_number;
    int same_treat_number;
};


/*
 *
 * This function calculate the age category of the aimed person
 * 
 * Function name: calculate_age_category
 * Input: none
 * Output: int
 * Side effect: calculate the age category of a person
 * 
*/
int person::calculate_age_category(void)
{
    //Since we need to know the keep
    long long age_value = register_time[0] - birth[0];    //Calulate the age of a patient by subtracting the year
                                                        //Note that this is just a very rough time
    long long age_range;  //Used to store the code of age for key calculation

    // Now this sub_block is used to calculate the age range
    // Since the smaller the age code age, the earlier the appointment time will be
    // We design the arrangement of age as the followings:
    // 1st: young children with age <= 12
    if (age_value <= 12)
    {
        age_range = 1;
    }
    // 2nd: the eldly with age > 75
    else if (age_value > 75)
    {
        age_range = 2;
    }
    // 3rd: the eldly with age (65,75]
    else if (65 < age_value && age_value <= 75)
    {
        age_range = 3;
    }
    // 4th: old mid_age person with age (50,65]
    else if (50 < age_value && age_value <= 65)
    {
        age_range = 4;
    }
    // 5th: teens with age (12,18]
    else if (12 < age_value && age_value <= 18)
    {
        age_range = 5;
    }
    // 6th young adults with age (18,35]
    else if (18 < age_value && age_value <= 35)
    {
        age_range = 6;
    }
    // the last adults with age (35,50]
    else
    {
        age_range = 7;
    }
    return age_range;
}

/*
 *
 * This function print all the information of the aimed person
 * 
 * Function name: print_person
 * Input: a person struct
 * Output: none
 * Side effect: print all the information of a person 
 * 
 * 
*/
void print_person(person target)
{
    cout << "Key: "<< target.key<<endl;
    cout << "Register time: " << target.register_time[0] << "/" << target.register_time[1] << "/" << target.register_time[2] << "/" << target.register_time[3] << ":" << target.register_time[4] <<endl;
    cout << "ID: " << target.id << endl;
    cout << "Name: " << target.name << endl;
    cout << "Address: " << target.CONTACT_DETAIL[0] << endl;
    cout << "Phone: " << target.CONTACT_DETAIL[1] << endl;
    cout << "Wechat: " << target.CONTACT_DETAIL[2] << endl;
    cout << "Email: " << target.CONTACT_DETAIL[3] << endl;
    cout << "Profession: " << target.profession << endl;
    cout << "Date of Birth: " << target.birth[0] << "/" << target.birth[1] << "/" << target.birth[2] << endl;
    cout << "Risk status: " << target.risk << endl;
    cout << "Registration: " << target.registration << endl;
    cout << "Appointment location: " << target.hospital << endl;
}

void print_person_s1mple(person target)
{
    cout << "Name: " << target.name << "; " << "ID " << target.id << "; "<< "Profession " << target.profession << "; " << "Age category: " << target.calculate_age_category() << "; " 
        << "Risk status: " << target.risk << ".";
}

void file_print_person_s1mple(person target, string filename)
{
    ofstream outfile;
    outfile.open(filename,ios::app);
    outfile << "Name: " << target.name << "; " << "ID " << target.id << "; "<< "Profession " << target.profession << "; " << "Age category: " << target.calculate_age_category() << "; " 
        << "Risk status: " << target.risk << ".";
    outfile.close();
}



/*
 *
 * Function name: calculate_key
 * Functionality: This function calculates the key of one person(priority level)
 * Input: a person struct
 * Output: none
 * Side Effect: mark the key value in the input person structure with the key value
 *
*/

void person::calculate_key()
{
    long long age_range;  //Used to store the code of age for key calculation
    age_range = calculate_age_category();
    long long ddl;
    if (ddlletter == 0) { ddl = 2;} 
    else {ddl = 1;}
    //This sub-block calculates the key value to used for sort
        // The design of the key is: we combine the indications used for sort into a 10^14 scale number
        // The less the scale is, the higher the priority will be.
        // This first signaficant place is the risk, if the risk is 0,1, it is treated as usual, for 2, add one month, for 3, mark
        // it by 2 to leave it till it is empty
        // The second significant place is the profession, 1 for the most important, 8 for the least important
        // The third is the age code, which is declared above
        // The followings are the registration time from year to minute

    if (risk == 0 || risk == 1)
    {
        key = ddl * (long long)pow(10,16)                  //ddl
            + (long long)pow(10, 14)                       //risk code
            + profession * (long long)pow(10, 13)          //profession
            + age_range * (long long)pow(10,12)            //age code
            + register_time[0] * (long long)pow(10,8)      //year
            + register_time[1] * (long long)pow(10,6)      //month
            + register_time[2] * (long long)pow(10,4)      //day
            + register_time[3] * (long long)pow(10,2)      //hour
            + register_time[4];                            //minute
    }
    else if (risk == 2)         //The 1 month delay must be considered isolatedly, there is a problem, which must be corrected latter on!!!
    {
        key = ddl * (long long)pow(10,16)                   //ddl
            + (long long)pow(10, 14)                        //risk code
            + profession * (long long)pow(10, 13)           //profession
            + age_range * (long long)pow(10,12)             //age code
            + register_time[0] * (long long)pow(10,8)       //year
            + register_time[1] * (long long)pow(10,6)   //month
            + register_time[2] * (long long)pow(10,4)       //day
            + register_time[3] * (long long)pow(10,2)       //hour
            + register_time[4];                             //minute
    }
    else if (risk == 3)
    {
        key = ddl * (long long)pow(10,16)                   //ddl
            + 2 * (long long)pow(10, 14)                    //risk code
            + profession * (long long)pow(10,13)            //profession
            + age_range * (long long)pow(10,12)             //age code
            + register_time[0] * (long long)pow(10,8)       //year
            + register_time[1] * (long long)pow(10,6)       //month
            + register_time[2] * (long long)pow(10,4)       //day
            + register_time[3] * (long long)pow(10,2)       //hour
            + register_time[4];                             //minute
    }
    key += treatment * (long long)pow(10,15);
}




/*
 *
 * Function name: calculate_registertime
 * Functionality: This function takes a start time point and calculates register time by generate a random
 * hour and minutes and calculating year/month/day
 * Input: a person struct, a startyear, a startmonth, a startday, a starthour, and halfdaies that have passed
 * Output: none
 * Side Effect: mark the register_time[] in the input person structure with the registeration time
 *
 * 
*/

void person::calculate_registertime(long long startyear, long long startmonth, long long startday, long long starthour, long long halfday)
{
    long long randminute = rand() % 60;     //generate a random minute
    long long added_hour = rand() % 12;     //generate a random hour
    long long hour = (starthour + 12 * halfday + added_hour) % 24;      //calculate the hour
    long long day = (startday + (starthour + halfday * 12 / 2) - 1) % 30 + 1;     //calculate the day
    long long month = ((startmonth + (((startday + halfday / 2) - 1) % 30) + 1) - 1) % 12 + 1;    //calculate the month
    long long year = startyear + ((startmonth + ((startday + halfday / 2) - 1) % 30) + 1) % 12 - 1; //calculate the year; note this is only valid after 2000 and before 3000
    //Input the times in
    register_time[0] = year;
    register_time[1] = month;
    register_time[2] = day;
    register_time[3] = hour;
    register_time[4] = randminute;
}

/*
 *
 * Function name: calculate_waiting_halfday
 * Functionality: This function calculates how many halfdays a person has waited
 * Input: the current halfday
 * Output: the halfdays waited
 * Side Effect: none
 *
 * 
*/
int person::calculate_waiting_halfday(int halfday)
{
    int time = halfday - regist_halfday;
    return time;
}


/*
 *
 * Function name: mark_regist_halfday
 * Functionality: This function the current time in units half_day and mark it into regist_halfday member object
 * Input: a person struct, halfdaies that have passed
 * Output: none
 * Side Effect: mark the regist_halfday
 *
 * 
*/

void person::mark_regist_halfday(int halfday)
{
    regist_halfday = halfday;
}




/*
 *
 * Function name: mark_treat_halfday
 * Functionality: This function the treated time in units half_day and mark it into treat_halfday member object
 * Input: a person struct, halfdaies that have passed
 * Output: none
 * Side Effect: mark the treat_halfday
 *
 * 
*/

void person::mark_treat_halfday(int halfday)
{
    treat_halfday = halfday;
}

