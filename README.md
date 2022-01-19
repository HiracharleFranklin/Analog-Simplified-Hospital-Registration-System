# City Medicine Auto-registration & Appointment System
This project is the CS225 computing assignment 2, which analogs a simplified hospital registration system using structures like list, fifo, B tree, B+ tree, hash table and fibonacci heap. 

## Coworker's Information
Song Yifei - ZJU-UIUC Institution

Qi Zhenting - ZJU-UIUC Institution

Li Zihao - ZJU-UIUC Institution

## Program Usage
Download all the files and run makefile, all the information to display will be shown on the console.

## Functionality
### Registration
A person can register for medical treatment at a local registry. For the registration the following information is required:

> – the identification number of the person,
> 
> – the name and contact details (address, phone, WeChat, email) of the person,
> 
> – the profession of the person,
> 
> – the date of birth of the person,
> 
> – the medical risk status of the person.

Each registration record will receive a timestamp, i.e. the date and the time of the registration will be stored. The professions are classified into eight profession categories I to VIII. There are seven age groups: children (≤ 12 years), adolescents (≤ 18 years), young adults (≤ 35 years), adults (≤ 50 years), seniors (≤ 65 years), elderly people (≤ 75 years), and old people (> 75 years). There are four different risk status: no risk (0), low risk (1), medium risk(2), and high risk (3). 

Registration records are stored permanently. In addition, records with relevant information such as identification, profession category, age category, risk status, registration date and time are kept in a queue, which twice daily is forwarded to the centralised treatment queue. Data from different local registries is added to the centralised queue in random order, but the order from the local queues is preserved.

### Queueing and assignment of appointments. 
The assignment of an appointment for receiving treatment is organised according to priority. The most important criterion is the profession category followed by a ranking of age groups, and further followed by data and time of the registration. Only people with no or low risk are considered. For people with medium risk a one month extension is added; people with high risk are only considered when there are no others waiting in the queue.

A person may withdraw from the treatment at any time. Those who have withdrawn and then registered again are considered with additional two weeks waiting time, unless they are in the medium or high risk group.

Furthermore, the profession category and the risk status of a queueing person may change. If this affects increases the priority in the queue, the relevant attributes are updated. A person may also present a priority letter with a deadline for the treatment. At the latest a person is assigned an appointment before the deadline regardless of the other selection criteria.

### Appointment processing
When a person has highest priority in the priority queue, an appointment location, date and time (usually the next day) is assigned and registered. With this the person leaves the queue. There are several locations for treatment processing each with a daily capacity and associated time slots for the treatment. Usually, the closest location is selected for a person.

The right to withdraw is reserved. When the treatment has been processed at the designated location at the reserved day and time, this is also registered
