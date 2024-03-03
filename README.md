### CS253 - Car Rental System

This assignment implements a Car Rental System. It has been implemented in C++ language. File handling is for storing the data. The program has three CSV files namely, cars.csv, rented_cars.csv and users.csv.

The file [main.cpp](https://github.com/SmartCheese22/CS253A/blob/main/main.cpp) contains the source code, the classes and their functions.

[cars.csv](https://github.com/SmartCheese22/CS253A/blob/main/cars.csv) maintains the car database keeping a record of every car in the system. The database is structured as follows:
{'Name of the Car','License Number','Condition of Car', 'Per Day Rent Cost', 'IsRented'}
Here Condition of a car is expressed as percentage i.e 0 to 100. New cars are assumed to be in best condition with value 100. As the car is rented over time, its condition decreases depending on the damage that is cause by user. More details are given in the below sections.
IsRented represents a boolean value. If the car is not currently rented, it is 0. If it is rented it is 1.

[rented_cars.csv](https://github.com/SmartCheese22/CS253A/blob/main/rented_cars.csv) keeps track of the cars that are currently rented and to which user. The database is structured as follows:
{'Customer Id', 'Car Name', 'License Number', 'Rent Date', 'Days rented'}

[users.csv](https://github.com/SmartCheese22/CS253A/blob/main/users.csv) contains the users of the system. Users are differentiated in three sub classes, namely Customer, Employee and Manager. The structure of the user database is as follows:
{'Name of User', 'User Id', 'User Password', 'User Record', 'Fine Due'}
User Record is expressed as percentage i.e. from 0 to 100. Any new User(Customer or Employee) is assigned a default value of 50.0. This metric is used as a credit score. Users(Customer or Employee) can rent a limited number of cars as per their record. The record is updated when a User returs a rented Car. The factors taken into consideration are delayed return of rented car and the damage caused to car. The User record is negatively affected in case of delayed return or damage to the car. If the car is return On/Before Due Date, the record in positively impacted.
Fine Due is update when a User returns a car. Accordingly, a fine of ₹1000 is imposed per day, which is reflected in the Fine Due.
Employees are given 15% discount on the cars per day rent.

Various Functionalities are provided to Each User of the System:

## Customer and Employee can:
* See all the Cars
* See cars rented by him
* Check if car available to rent
* Rent Car
* Return Car
* View Fine Due
* Clear Due

# Manager can:
* Add user
* Modify User
* Delete User
* Add Car
* Modify Car
* Delete Car
* See rented Cars
* Who has rented Which car
* Some Additional functions

## Description of Working and Assumptions:

Initially the system prompts for login. Depending upon the users listed in the user database, the corresponding input Id of the user may login as Customer, Employee or Manager(For this case, a Default Manager is already present in the database who can add the other Users). The Users proceed according to the interface the are shown (which depends on their UserType i.e. Customer, Employee or Manager).

The User Record is used to set a limit to the maximum car he can rent simultaneously. The User Record is Update on return of a rented Car. The damage caused to rented car is assumed proportional to the number of days the car was rented and inversely proportional to the user record. (This is an assumption. There can be different ways to calculate or find the damage caused to car). This damage leads to a decrease in the User Record and so Delayed Return of the Car.
The damage caused to the car is reflected in the condition of the car, and after a certain point, the car cannot be rented due to high damaged(poor condition of the car). For such cases, the Manager has been provided functionality of repairing the car to its best condition. Also a user cannot rent a car if his record falls below a threshold. For such cases, Manager has the authority to update the User Record for such users.
Customers and Employees can see all the cars as it shows the entire collection of cars that the company has to rent. The manager can see the details of cars and users.
