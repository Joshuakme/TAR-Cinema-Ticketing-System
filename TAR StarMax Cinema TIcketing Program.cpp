#include <iostream>
#include <iomanip>      // used for formatting the code
#include <string>

using namespace std;

// Functions Definition
    // Start Screen
void logoDisplay(string movie_date);
void movieDetail_display(string movie[], string movie_time[]);
int movie_detail_get(int n, string movie[], string movie_time_user[]);
    // Ticket Selling
int select_class();
void seat_display(int n, int movie_code, int class_num, struct display displayTable[]);

int seatNum_get(int n, int movie_code, int class_selected, struct seatMovie movie_seat[]);
int *seatno_booking(int n, int movie_code, int class_num, int number_of_seat, struct seatMovie movie_seat[],
                struct display displayTable[]);
double cal_totalPrice(int movie_code, int class_selected, double student_price[], double cat_price[], struct catMovie movie_categ[]);
double payment_method(double total_price);
void bill_display(int n, string movie_date, int movie_code, int class_selected, double student_price[], double cat_price[], 
            double total_price, struct seatMovie movie_seat[], struct catMovie movie_categ[], double cash_paid, double change_due);
void ticket_display(int n, string movie_date, string movie[], string movie_time[], int movie_code, int class_selected,
            double student_price[], double cat_price[], struct seatMovie movie_seat[], struct catMovie movie_categ[]);
void salesReport_display(int n, string movie_date, string movie[], string movie_time[], int movie_code,struct seatMovie movie_seat[], 
                struct catMovie movie_categ[]);

// Structure Definition
struct detail_movie {
    string name;
    string time;
};
struct seatMovie {
    int slot1 = 30;
    int slot2 = 30;
    int slot3 = 30;
    int *ptr_slot1 = &slot1;
    int *ptr_slot2 = &slot2;
    int *ptr_slot3 = &slot3;
    int seat_no1[30] = {};
    int seat_no2[30] = {};
    int seat_no3[30] = {};
    int bookedSeat1[30] = {};    // Check if seats are booked
    int bookedSeat2[30] = {};
    int bookedSeat3[30] = {};
    int *current_bookedSeat1 = NULL;
    int *current_bookedSeat2 = NULL;
    int *current_bookedSeat3 = NULL;
};
struct display {
    int seatTable1[30] = {};
    int seatTable2[30] = {};
    int seatTable3[30] = {};
};
struct catMovie {
    int child_num = 0;
    // Student - df / bd / phd
    int student_num = 0;
    int df_num = 0;         // Diploma/Foundation
    int bd_num = 0;         // Bachelor Degree
    int phd_num = 0;        // phD
    int adult_num = 0;
    int sCitizen_num = 0;
    int total_cat = 0;
    double total_sales[4] = {};
    int total_seat[4] = {};
};


int main() {
// Declaration & Initialization of variables & constants
    // Movie Details
    const string movie_date = "10-12-2021 (Friday)";
    const int n = 3;            // movie number
    detail_movie movie_detail[n];
    movie_detail[0] = {"The Interstella", "11:00 a.m."};
    movie_detail[1] = {"Free Guy", "5:30 p.m."};
    movie_detail[2] = {"Shazam!", "8:00 p.m."};

    string movie[n] = {};
    string movie_time[n] = {};
    for (int i = 0; i < n; i++) {
        movie[i] = movie_detail[i].name;          //string movie[3] = { "The Interstella", "Free Guy", "Shazam!" };
        movie_time[i] = movie_detail[i].time;     //string movie_time[3] = {"11:00 a.m.", "5:30 p.m.", "8:00 p.m."};
    }
    // User input to get movie detail
    string movie_time_user[n] = { "1100", "1730", "2000" };     // For user input purposes
    int movie_code = 0;

    // Seat Booking
    seatMovie movie_seat[n];        // n = 3
    int class_selected;
    display displayTable[n];          // Display Seat Table for Class 1,2,3 for each movie[n]

    // Obtain number of tickets in each cat
    catMovie movie_categ[n];        // n = 3;
    int num_of_seat = 0;             // number of seat of current user want to book

    // Price Table
        /* Price Class Table REF
            {[0][0],  [0][1], [0][2]}
            {[1][0],  [1][1], [1][2]}
            {[2][0],  [2][1], [2][2]}   */
    const double price_class[3][3] = {{ 14, 18, 9 },
                                      { 16, 20, 10 },
                                      { 20, 26, 13 }};        
    const double discount_percent[3] = { 0.95, 0.93, 0.91 };       // {Diploma/Foundation, Degree, PhD}

    // Calculation
    double total_price = 0;     // Total price to pay
    double amount_paid = 0;     // Amount paid by user
    double change_due = 0;      // amount change due to the user
    // Options
    char loop = ' ';


// Start Screen 
    do {
        cout << setfill(' ') << setw(100) << left;
        logoDisplay(movie_date);       //Display Logo

        cout << setfill(' ') << setw(11) << ' ';
        cout << setfill(' ') << setw(30) << "Welcome to TAR StarMax Cinema!" << endl;
        cout << setfill(' ') << setw(11) << ' ' << setfill('-') << setw(30) << '-' << endl << endl;
        cout << "Movies available today : " << endl << endl;

        // Display availavble movies
        movieDetail_display(movie, movie_time);
        cout << endl << endl;
        cout << "Counter is now Open for Selling of Tickets" << endl;
        cout << "[Note: Each movie currently has only 1 time slot due to SOP]" << endl << endl << 
            "Please enter the movie name & time slot :" << endl;
        // Get user input for movie name & time
        movie_code = movie_detail_get(n, movie, movie_time_user);  //Ask movie_name & time from user, identify which movie is selected   
        cout << endl;


        // Display number of seats left & Graphics
        for (int i = 0; i < n; i++) {
            if (movie_code == i) {
                cout << "Seats available in\n";
                cout << left << setfill(' ') << setw(18) << "Class 1 (Front)" << ": " << movie_seat[i].slot1 << " seats" << endl;
                cout << left << setfill(' ') << setw(18) << "Class 2 (Middle)" << ": " << movie_seat[i].slot2 << " seats" << endl;
                cout << left << setfill(' ') << setw(18) << "Class 3 (Sofa bed)" << ": " << movie_seat[i].slot3 << " seats" << endl;
                cout << endl;
                break;
            }
        }

// Ticket Selling                  
        class_selected = select_class();        // Select Class and display them
        seat_display(n, movie_code, class_selected, displayTable);
        cout << endl;
        // Ask user to enter which seats they wanted to buy
        num_of_seat = seatNum_get(n, movie_code, class_selected, movie_seat);

        // Get an array which store the current user's booked seat & store them in a total booked seat of each movie
        int n1 = 0, n2 = 0, n3 = 0;
        for (int y = 0; y < n; y++) {
            if (movie_code == y) {
                switch (class_selected) {
                    case 1: {
                        movie_seat[y].current_bookedSeat1 = seatno_booking(n, movie_code, class_selected, num_of_seat, movie_seat, displayTable);
                        for (int i = 0; i < num_of_seat; i++) {
                            movie_seat[y].bookedSeat1[n1] = *(movie_seat[y].current_bookedSeat1 + i);                                                                                                                                                                      i;
                            if (n1 >= 0 && n1 <= 30)
                                n1++;
                            else                                                                                           
                                break;
                        }
                        break;
                    }
                    case 2: {
                    movie_seat[y].current_bookedSeat2 = seatno_booking(n, movie_code, class_selected, num_of_seat, movie_seat, displayTable);
                    for (int i = 0; i < num_of_seat; i++) {
                        movie_seat[y].bookedSeat2[n2] = *(movie_seat[y].current_bookedSeat2 + i);
                        if (n2 >= 0 && n2 <= 30)
                            n2++;
                        else
                            break;
                    }
                    break;
                }
                    case 3: {
                    movie_seat[y].current_bookedSeat3 = seatno_booking(n, movie_code, class_selected, num_of_seat, movie_seat,
                        displayTable);
                    for (int i = 0; i < num_of_seat; i++) {
                        movie_seat[y].bookedSeat3[n3] = *(movie_seat[y].current_bookedSeat3 + i);
                        if (n3 >= 0 && n3 <= 30)
                            n3++;
                        else
                            break;
                    }
                    break;
                }
                    default:
                        cout << "Error!" << endl;
                }
            }
        }

        // Ask user to enter no. of ppl according to their categ
        do {
            for (int i = 0; i < n; i++) {
                if (movie_code == i) {
                    cat_num_get:
                    cout << "\nEnter the number of tickets required for each categories\n" << endl;

                    cout << left << setfill(' ') << setw(29) << "Children (Below 12 years old)" << ": ";
                    cin >> movie_categ[i].child_num;

                    if (movie_categ[i].child_num >= 0 && movie_categ[i].child_num <= num_of_seat) {
                        cout << left << setfill(' ') << setw(29) << "Student (With Student Card)" << ": ";
                        cin >> movie_categ[i].student_num;

                        if (movie_categ[i].student_num >= 0 && movie_categ[i].student_num <= num_of_seat) {
                            cout << left << setfill(' ') << setw(29) << "Adult" << ": ";
                            cin >> movie_categ[i].adult_num;

                            if (movie_categ[i].adult_num >= 0 && movie_categ[i].adult_num <= num_of_seat) {
                                cout << left << setfill(' ') << setw(29) << "Senior Citizen (Above 60 years old)" << ": ";
                                cin >> movie_categ[i].sCitizen_num;
                            }
                            else
                                goto cat_error;
                        }
                        else
                            goto cat_error;
                    }
                    else
                        goto cat_error;
                    

                    if (movie_categ[i].sCitizen_num >= 0 && movie_categ[i].sCitizen_num <= num_of_seat) {
                        movie_categ[i].total_cat = movie_categ[i].child_num + movie_categ[i].student_num + movie_categ[i].adult_num 
                                                    + movie_categ[i].sCitizen_num;
                    }             
                    else
                        goto cat_error;
                    
                    if (movie_categ[i].total_cat != num_of_seat) {
                    cat_error:
                        cout << "\nInvalid input of number of ticket. Please try again\n";
                        cin.clear();
                        cin.ignore();
                        goto cat_num_get;
                    }        
                    else 
                        break;
                }
            }  
            break;
        } while (movie_categ[movie_code].total_cat != num_of_seat);

        // Calculating the total number of customer in each cat
        movie_categ[movie_code].total_seat[0] += movie_categ[movie_code].child_num;
        movie_categ[movie_code].total_seat[1] += movie_categ[movie_code].student_num;
        movie_categ[movie_code].total_seat[2] += movie_categ[movie_code].adult_num;
        movie_categ[movie_code].total_seat[3] += movie_categ[movie_code].sCitizen_num;
   
        //Get the number of student in each category
        bool isEqual = false;           // flag variable
    stud_num_get:
        if (movie_categ[movie_code].student_num > 0) { 
            do {
                cout << endl << "Enter the number of student in the respective category :" << endl;
                cout << left << setfill(' ') << setw(15) << "Diploma/Foundation" << ": ";
                cin >> movie_categ[movie_code].df_num;
                if (movie_categ[movie_code].df_num >= 0 && movie_categ[movie_code].df_num <= num_of_seat) {
                    cout << left << setfill(' ') << setw(15) << "Degree" << ": ";
                    cin >> movie_categ[movie_code].bd_num;

                    if (movie_categ[movie_code].bd_num >= 0 && movie_categ[movie_code].bd_num <= num_of_seat) {
                        cout << left << setfill(' ') << setw(15) << "PhD" << ": ";
                        cin >> movie_categ[movie_code].phd_num;

                        if (movie_categ[movie_code].phd_num >= 0 && movie_categ[movie_code].phd_num <= num_of_seat)
                            break;
                    }
                    else
                        goto stud_error;
                }    
                else
                    goto stud_error;    

                if ((movie_categ[movie_code].df_num + movie_categ[movie_code].bd_num + movie_categ[movie_code].phd_num) == movie_categ[movie_code].student_num)
                    isEqual = true;
                stud_error:
                if (!isEqual) {
                    cout << "\nInvalid input. Please try again.\n";
                    cin.clear();
                    cin.ignore();
                    goto stud_num_get;
                }     
            } while (!isEqual);
        }


    // Calculation
        // Price of each category
        double student_price[3] = { price_class[class_selected - 1][1] * discount_percent[0],           // df
                                          price_class[class_selected - 1][1] * discount_percent[1],     // bd
                                          price_class[class_selected - 1][1] * discount_percent[2] };   // phd
        double cat_price[3] = { price_class[class_selected - 1][2],                                     // Child
                                      price_class[class_selected - 1][1],                               // Adult
                                      price_class[class_selected - 1][0] };                             // Senior Citizens
        
// Cash Payment
        if (movie_categ[movie_code].total_cat > 0) {
            total_price = cal_totalPrice(movie_code, class_selected, student_price, cat_price, movie_categ);
            amount_paid = payment_method(total_price);
            if (amount_paid >= total_price) {
               change_due = amount_paid - total_price;
            }
            cout << "\n\n";

            // Calculating the total sales of each cat
            for (int i = 0; i < n; i++) {
               if (movie_code == i) {
                    movie_categ[i].total_sales[0] += movie_categ[i].child_num * cat_price[0];
                    movie_categ[i].total_sales[1] += (movie_categ[i].df_num * student_price[0]) + 
                                                     (movie_categ[i].bd_num * student_price[1]) +
                                                     (movie_categ[i].phd_num * student_price[2]);
                    movie_categ[i].total_sales[2] += movie_categ[i].adult_num * cat_price[1];
                    movie_categ[i].total_sales[3] += movie_categ[i].sCitizen_num * cat_price[2];
                }
            } 

            // Display Bill Breakdown
            bill_display(n, movie_date, movie_code, class_selected, student_price, cat_price, total_price, movie_seat, 
                        movie_categ, amount_paid, change_due);

            // Print Movie Ticket
            ticket_display(n, movie_date, movie, movie_time, movie_code, class_selected, student_price, cat_price, 
                            movie_seat, movie_categ);
        }

// Looping Options
        cout << endl << endl << endl;
        cout << left << setfill(' ') << setw(5) << ' ';
        cout << "Seats Still Available :" << endl;
        cout << left << setfill(' ') << setw(6) << ' ' << "Class 1 - " << movie_seat[movie_code].slot1 << endl;
        cout << left << setfill(' ') << setw(6) << ' ' << "Class 2 - " << movie_seat[movie_code].slot2 << endl;
        cout << left << setfill(' ') << setw(6) << ' ' << "Class 3 - " << movie_seat[movie_code].slot3 << endl << endl << endl;

        cout << "Next Customer (Y=Yes / N=No) : ";
        cin >> loop;
        cout << endl;


        int seat_left = 0;
        for (int j = 0; j < n; j++) {
            seat_left += *movie_seat[j].ptr_slot1 + *movie_seat[j].ptr_slot2 + *movie_seat[j].ptr_slot3;
        }

        if (toupper(loop) == 'N' || seat_left == 0) {
            // salesReport:
            salesReport_display(n,movie_date, movie, movie_time, movie_code, movie_seat, movie_categ);
            cout << "Program Exited...";
            break;
        }
        else if (toupper(loop) == 'Y') {
            cout << setfill('-') << setw(102) << '-' << endl;
            cin.ignore(10000, '\n');
        }         

    } while (toupper(loop) != 'N');  
    return 0;
}


// Fucntions 

void logoDisplay(string movie_date) {
    cout << endl;
    cout << left << setw(42);
    cout << "*************       **         *********    " << right << setfill(' ') << setw(70) << movie_date << endl;   //Display current Date/Time
    cout << "     **            ****        **       *   " << setfill(' ') << setw(70) << system("time/t") << endl;
    cout << "     **           **  **       *********    " << endl;
    cout << "     **          **    **      ****         " << endl;
    cout << "     **         **********     **  ****     " << endl;
    cout << "     **        **        **    **     ***   " << endl;
    cout << "     **       **          **   **      ***  " << endl;                            
    //cout << setfill(' ') << setw(11) << ' ';

    cout << endl << endl;
    cout << "  *********    *************       **        *********           **          **        **       ***         ***" << endl;
    cout << " ***      ***       **            ****       **       *          ****      ****       ****        ***      *** " << endl;
    cout << "***                 **           **  **      *********           ** **    ** **      **  **         **    **   " << endl;
    cout << "  *********         **          **    **     ****                ** ***  *** **     **    **          ****     " << endl;
    cout << "          ***       **         **********    **  ****            **   ****   **    **********       **    **   " << endl;
    cout << "***      ***        **        **        **   **     ***          **    **    **   **        **     ***     *** " << endl;
    cout << "  ********          **       **          **  **      ***         **          **  **          **  ***        ***" << endl;
    cout << endl << endl;
}

void movieDetail_display(string movie[], string movie_time[]) {
    for (int n = 0; n < 3; n++) {
        if (n == 0) {
            cout << left << setfill(' ') << setw(4) << ' ' << setfill(' ') << setw(7) << ' ' << setfill(' ') << setw(13) << 
                "Movies" << " |  " << ' ' << setfill(' ') << setw(10) << "Time Slot" << endl;
            cout << "   " << setfill('-') << setw(39) << '-' << endl;
        }
            

        cout << n + 1 << ". | ";
        cout << left << setfill(' ') << setw(19) << movie[n];
        cout << " |  ";
        cout << right << setfill(' ') << setw(11) << movie_time[n] << endl;
    }
}

//Movie_details_get()
int movie_detail_get(int n, string movie[], string movie_time_user[]) {
    bool found = false;         //Use flag to determine if the condition is met 
    string movie_get;
    string movie_arr;
    string movie_time_get;

    do {
        cout << "Movie Name : ";
        getline(cin, movie_get);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < movie_get.length(); j++) {
                movie_get[j] = tolower(movie_get[j]);
                if (j == 0)
                    movie_get[j] = toupper(movie_get[j]);
                else if (movie_get[j - 1] == ' ')
                    movie_get[j] = toupper(movie_get[j]);
            }
            if (movie_get == movie[i]) {         
                do {
                    //Ask Date/Time from user
                    cout << "Time (In 24hour) : ";
                    cin >> movie_time_get;
                    cout << endl;
                    if (movie_time_get == movie_time_user[i]) {
                        found = true;
                        return i;
                        break;                      //So the loop ends at the current index. 
                    }
                    if (!found)
                        cout << "You might enter an invalid movie time. \nPlease try again!" << endl << endl;
                } while (!found);
            }     
        }
        if (!found)
            cout << endl << "You might enter an invalid movie time. \nPlease try again!" << endl << endl;
    } while (!found);
}


int select_class() {
    bool isEqual = false;
    int class_get;
    do {
        cout << "Please select the class you want (Class 1/2/3) : ";
        cin >> class_get;
        for (int i = 1; i < 4; i++) {
            if (class_get == i) {
                isEqual = true;
                cout << endl;
                return class_get;
            }
        }
        
        if (!isEqual) {
            cout << "You might enter an invalid class number.\nPlease try again\n" << endl;
        }
    } while (!isEqual);
}

void seat_display(int n, int movie_code, int class_num, display displayTable[]) {
    for (int y = 0; y < n; y++) {
        if (movie_code == y) {
            if (class_num == 1) {
                //Class 1
                cout << endl << "Class 1\n\n";

                // Seats 101-110
                cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
                for (int i = 101; i <= 110; i++) {      // Seats number
                    cout << i << "   ";
                }
                cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
                for (int i = 0; i < 10; i++) {      // Seats availability
                    if (displayTable[y].seatTable1[i] == 1) {
                        cout << setfill(' ') << setw(3) << " X" << "   ";
                    }
                    else
                        cout << setfill(' ') << setw(3) << " O" << "   ";
                }
                cout << endl << endl;


                // Seats 111-120
                cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
                for (int i = 111; i <= 120; i++) {
                    cout << i << "   ";
                }
                cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
                for (int i = 10; i < 20; i++) {      // Seats availability
                    if (displayTable[y].seatTable1[i] == 1) {
                        cout << setfill(' ') << setw(3) << " X" << "   ";
                    }
                    else
                        cout << setfill(' ') << setw(3) << " O" << "   ";
                }
                cout << endl << endl;


                // Seats 121-130
                cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
                for (int i = 121; i <= 130; i++) {
                    cout << i << "   ";
                }
                cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
                for (int i = 20; i < 30; i++) {      // Seats availability
                    if (displayTable[y].seatTable1[i] == 1) {
                        cout << setfill(' ') << setw(3) << " X" << "   ";
                    }
                    else
                        cout << setfill(' ') << setw(3) << " O" << "   ";
                }
                cout << endl << endl;
            }

            else if (class_num == 2) {
                //Class 2
                cout << endl << "Class 2\n\n";

                // Seats 201-210
                cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
                for (int i = 201; i <= 210; i++) {      // Seats number
                    cout << i << "   ";
                }
                cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
                for (int i = 0; i < 10; i++) {      // Seats availability
                    if (displayTable[y].seatTable2[i] == 1) {
                        cout << setfill(' ') << setw(3) << " X" << "   ";
                    }
                    else
                        cout << setfill(' ') << setw(3) << " O" << "   ";
                }
                cout << endl << endl;


                // Seats 211-220
                cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
                for (int i = 211; i <= 220; i++) {
                    cout << i << "   ";
                }
                cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
                for (int i = 10; i < 20; i++) {      // Seats availability
                    if (displayTable[y].seatTable2[i] == 1) {
                        cout << setfill(' ') << setw(3) << " X" << "   ";
                    }
                    else
                        cout << setfill(' ') << setw(3) << " O" << "   ";
                }
                cout << endl << endl;


                // Seats 221-230
                cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
                for (int i = 221; i <= 230; i++) {
                    cout << i << "   ";
                }
                cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
                for (int i = 20; i < 30; i++) {      // Seats availability
                    if (displayTable[y].seatTable2[i] == 1) {
                        cout << setfill(' ') << setw(3) << " X" << "   ";
                    }
                    else
                        cout << setfill(' ') << setw(3) << " O" << "   ";
                }
                cout << endl << endl;
            }

            else if (class_num == 3) {
            //Class 3
            cout << endl << "Class 3\n\n";

            // Seats 301-310
            cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
            for (int i = 301; i <= 310; i++) {      // Seats number
                cout << i << "   ";
            }
            cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
            for (int i = 0; i < 10; i++) {      // Seats availability
                if (displayTable[y].seatTable3[i] == 1) {
                    cout << setfill(' ') << setw(3) << " X" << "   ";
                }
                else
                    cout << setfill(' ') << setw(3) << " O" << "   ";
            }
            cout << endl << endl;


            // Seats 311-320
            cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
            for (int i = 311; i <= 320; i++) {
                cout << i << "   ";
            }
            cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
            for (int i = 10; i < 20; i++) {      // Seats availability
                if (displayTable[y].seatTable3[i] == 1) {
                    cout << setfill(' ') << setw(3) << " X" << "   ";
                }
                else
                    cout << setfill(' ') << setw(3) << " O" << "   ";
            }
            cout << endl << endl;


            // Seats 321-330
            cout << left << setfill(' ') << setw(12) << "Seats No." << " : ";
            for (int i = 321; i <= 330; i++) {
                cout << i << "   ";
            }
            cout << endl << left << setfill(' ') << setw(12) << "Availability" << " : ";
            for (int i = 20; i < 30; i++) {      // Seats availability
                if (displayTable[y].seatTable3[i] == 1) {
                    cout << setfill(' ') << setw(3) << " X" << "   ";
                }
                else
                    cout << setfill(' ') << setw(3) << " O" << "   ";
            }
            cout << endl << endl;
            }
        }
    }
}

int seatNum_get(int n, int movie_code, int class_selected, seatMovie movie_seat[]) {
    int number_of_seat = 0;
    bool valid = false;
    do {
        cout << "Enter the number of seats required (1 - 30) : ";
        cin >> number_of_seat;
        cout << endl;
        for (int i = 0; i < n; i++) {
            if (movie_code == i) {
                switch (class_selected) {
                    case 1: 
                        if (number_of_seat >= 1 && number_of_seat <= movie_seat[i].slot1) {
                            return number_of_seat;
                            valid = true;
                        }
                        break;
                    case 2: 
                        if (number_of_seat >= 1 && number_of_seat <= movie_seat[i].slot2) {
                            return number_of_seat;
                            valid = true;
                        }
                        break;
                    case 3: 
                        if (number_of_seat >= 1 && number_of_seat <= movie_seat[i].slot3) {
                            return number_of_seat;
                            valid = true;
                        }
                        break;
                    default :
                        cout << "Error" << endl;
                }
            }
        }
        if (!valid)
            cout << "Number out of range. Please try again. \n\n";

    } while (!valid);
}

int *seatno_booking(int n, int movie_code, int class_num, int number_of_seat, seatMovie movie_seat[], display displayTable[]) {
    int seat_no = 0;
    int c = 0;          // count

    cout << endl << "Enter the seat no. you wanted to book\n";
    do {
        question:
            cout << c + 1 << " : ";
            cin >> seat_no;

        for (int i = 0; i < n; i++) {
            if (movie_code == i) {
                switch (class_num) {
                    case 1: {
                        if (seat_no >= 101 && seat_no <= 130) {
                            for (int y = 0; y < 30; y++) {
                                if (seat_no == movie_seat[i].bookedSeat1[y]) {
                                    cout << "Seat " << seat_no << " has already been booked.\n";
                                    cout << "Please try again!\n\n";
                                    goto question;
                                }
                            }
                            if (seat_no != movie_seat[i].bookedSeat1[c]) {
                                movie_seat[i].bookedSeat1[c] = seat_no;
                                *movie_seat[i].ptr_slot1 -= 1;
                                displayTable[i].seatTable1[seat_no - 101] = 1;
                                c++;
                            }
                        }
                        else {
                            cout << "Invalid seat no. Please try again!\n\n";
                            cin.ignore();
                            goto question;
                        }
                        break;
                    }
                    case 2: {
                        if (seat_no >= 201 && seat_no <= 230) {
                            for (int y = 0; y < 30; y++) {
                                if (seat_no == movie_seat[i].bookedSeat2[y] && c > 0) {
                                    cout << "Seat " << seat_no << " has already been booked.\n";
                                    cout << "Please try again!\n\n";
                                    goto question;
                                }
                            }
                            if (seat_no != movie_seat[i].bookedSeat2[c]) {
                                movie_seat[i].bookedSeat2[c] = seat_no;
                                *movie_seat[i].ptr_slot2 -= 1;
                                displayTable[i].seatTable2[seat_no - 201] = 1;
                                c++;
                            }
                        }
                        else {
                            cout << "Invalid seat no. Please try again!\n\n";
                            cin.ignore();
                            goto question;
                        }
                        break;
                    }
                    case 3: {
                        if (seat_no >= 301 && seat_no <= 330) {
                            for (int y = 0; y < 30; y++) {
                                if (seat_no == movie_seat[i].bookedSeat3[y] && c > 0) {
                                    cout << "Seat " << seat_no << " has already been booked.\n";
                                    cout << "Please try again!\n\n";
                                    goto question;
                                }
                            }
                            if (seat_no != movie_seat[i].bookedSeat3[c]) {
                                movie_seat[i].bookedSeat3[c] = seat_no;
                                *movie_seat[i].ptr_slot3 -= 1;
                                displayTable[i].seatTable3[seat_no - 301] = 1;
                                c++;
                            }
                        }
                        else {
                            cout << "Invalid seat no. Please try again!\n\n";
                            cin.ignore();
                            goto question;
                        }
                        break;
                    }
                }
            } 
        }
    } while (c < number_of_seat);

    // Return array of current booked seat
    for (int i = 0; i < n; i++) {
        if (movie_code == i) {
            switch (class_num) {
            case 1:
                return movie_seat[i].bookedSeat1;
                break;
            case 2:
                return movie_seat[i].bookedSeat2;
                break;
            case 3:
                return movie_seat[i].bookedSeat3;
                break;
            default:
                cout << "Error!";
            } 
        }
    }
    
}

// Calculations
double cal_totalPrice(int movie_code, int class_selected, double student_price[], double cat_price[], catMovie movie_categ[]) {
    double total_price;
    double price_class[3][3] = { { 14, 18, 9 }, //Define prices of 3 classes of seats into arrays
                                { 16, 20, 10 },
                                { 20, 26, 13 } };  

    //Calculate total price 
    total_price = movie_categ[movie_code].child_num * cat_price[0] + 
                     (movie_categ[movie_code].df_num * student_price[0] + 
                      movie_categ[movie_code].bd_num * student_price[1] +
                      movie_categ[movie_code].phd_num * student_price[2]) +
                   movie_categ[movie_code].adult_num * cat_price[1] + 
                   movie_categ[movie_code].sCitizen_num * cat_price[2];

    return total_price;
}

double payment_method(double total_price) {
    // Declaration & Initialization of variables 
    double cash_paid = 0, change_due = 0;
    bool paid = false, valid = false;
    string pay_mtd[3] = { "Cash", "Student Card", "Touch n Go" };
    double card_balance = 200, tng_balance = 200;
    int pay_select = 0, student_id = 0;
    char qr_scanned = ' ';

    // Codes
    cout << endl << setfill(' ') << setw(15) << "Total Price" << "= RM " << fixed << setprecision(2) << showpoint <<
        total_price << endl;
    do {
        cout << endl << "Payment method : " << endl;
        for (int j = 0; j < 3; j++) {
            cout << j + 1 << ". | ";
            cout << left << setfill(' ') << setw(12) << pay_mtd[j];
            if (j == 1)
                cout << "(Recommended)";
            cout << endl;
        }
        cout << endl;

        do {
            cout << "Select one payment method to pay (1/2/3): ";
            cin >> pay_select;
            switch (pay_select - 1) {
            case 0: {
                cout << left << setfill(' ') << setw(15) << "Cash Payment" << "= RM ";
                cin >> cash_paid;
                if (cash_paid >= total_price) {
                    return cash_paid;
                    valid = true;
                    paid = true;
                }
                break;
            }

            case 1: {
            student_id_get:
                cout << left << setfill(' ') << setw(15) << "TARUC Student ID (7 digits) : ";
                cin >> student_id;

                if (to_string(student_id).length() != 7) {
                    cout << endl << "Invalid student ID. Please try again!" << endl;
                    goto student_id_get;
                }

                if (card_balance >= total_price) {
                    cout << "Card Balance : " << card_balance - total_price << endl;
                    return card_balance;
                    valid = true;
                    paid = true;
                }
                break;
            }

            case 2: {
            qr_status_get:
                // QR Code Image
                cout << left << setfill(' ') << setw(15) << "Please scan the QR code to pay (Y/N) : ";
                cin >> qr_scanned;

                if (toupper(qr_scanned) == 'N') {
                    cout << endl << "Failed to get the information. Please try again!" << endl;
                    goto qr_status_get;
                }
                if (tng_balance >= total_price) {
                    cout << "TnG Balance : " << tng_balance - total_price << endl;
                    return tng_balance;
                    valid = true;
                    paid = true;
                }
                break;
            }
            }
        } while (!valid);

        if (paid) {
            cout << endl << "Thank you for selecting TAR StarMax Cinema" << endl;
            cout << "You have succesfully purchased the tickets" << endl;
            return change_due;
        }
        else {
            cout << "Your Cash Payment is not enough.\nPlease try again!!" << endl << endl;
            cin.clear();
            cin.ignore();
        }
    } while (!paid);
}

// Display
void bill_display(int n, string movie_date, int movie_code, int class_selected, double student_price[], double cat_price[], 
    double total_price, struct seatMovie movie_seat[], struct catMovie movie_categ[], double cash_paid, double change_due) {
    
    double child_sales = 0, student_sales = 0, adult_sales = 0, sCitizen_sales = 0;
    
    // Display Bill
    cout << setfill('-') << setw(50) << '-' << endl << endl;
    for (int i = 0; i < n; i++) {
        if (movie_code == i) {
            cout << "Number of Seats Booked : " << endl;
            cout << left << setfill(' ') << setw(30) << "Children (Below 12 years old)" << ": " << movie_categ[i].child_num << endl;
            cout << left << setfill(' ') << setw(30) << "Student (With Student Card)" << ": " << movie_categ[i].student_num << endl;
            cout << left << setfill(' ') << setw(30) << "Adult" << ": " << movie_categ[i].adult_num << endl;
            cout << left << setfill(' ') << setw(30) << "Senior Citizens" << ": " << movie_categ[i].sCitizen_num << endl;
            cout << endl << endl;


            // Calculation of price
            child_sales = movie_categ[i].child_num * cat_price[0];
            student_sales = (movie_categ[i].df_num * student_price[0]) +
                            (movie_categ[i].bd_num * student_price[1]) +
                            (movie_categ[i].phd_num * student_price[2]);
            adult_sales = movie_categ[i].adult_num * cat_price[1];
            sCitizen_sales = movie_categ[i].sCitizen_num * cat_price[2];

            // Bill Breakdown
            cout << "Bill Breakdown - Class " << class_selected << endl;
            cout << endl;
            cout << "Number of customer in each category :" << endl;
            // Children
            cout << left << setfill(' ') << setw(20) << "Children" << "= " << movie_categ[i].child_num <<
                " @ RM " << left << setfill(' ') << setw(7) << fixed << setprecision(2) << showpoint <<
                cat_price[0] << " = RM " << right << setfill(' ') << setw(6) << fixed <<
                setprecision(2) << showpoint << child_sales << endl;
            // Students
            if (movie_categ[i].student_num > 0) {
                cout << left << setfill(' ') << setw(20) << "Students" << endl;
                if (movie_categ[i].df_num > 0) {
                    cout << left << setfill(' ') << setw(5) << ' ';
                    cout << left << setfill(' ') << setw(15) << "D/F (5%) " << "= " << movie_categ[i].df_num << " @ RM " <<
                        setfill(' ') << setw(7) << student_price[0] << " = RM " <<
                        right << setfill(' ') << setw(6) << fixed << setprecision(2) << showpoint <<
                        movie_categ[i].df_num * student_price[0] << endl;
                }
                else if (movie_categ[i].bd_num > 0) {
                    cout << left << setfill(' ') << setw(5) << " ";
                    cout << left << setfill(' ') << setw(15) << "Degree (7%) " << "= " << movie_categ[i].bd_num << " @ RM " <<
                        setfill(' ') << setw(7) << student_price[1] << " = RM " <<
                        right << setfill(' ') << setw(6) << fixed << setprecision(2) << showpoint <<
                        movie_categ[i].bd_num * student_price[1] << endl;
                }
                else if (movie_categ[i].phd_num > 0) {
                    cout << left << setfill(' ') << setw(5) << " ";
                    cout << left << setfill(' ') << setw(15) << "PhD (9%) " << "= " << movie_categ[i].phd_num << " @ RM " <<
                        setfill(' ') << setw(7) << student_price[2] << " = RM " << right << setfill(' ') << setw(6) << fixed <<
                        setprecision(2) << showpoint << movie_categ[i].phd_num * student_price[2] << endl;
                }
            }
            // Adults        
            cout << left << setfill(' ') << setw(20) << "Adults" << "= " << movie_categ[i].adult_num << " @ RM " << left <<
                setfill(' ') << setw(7) << cat_price[1] << " = RM " << right << setfill(' ') << setw(6) << fixed << setprecision(2)
                << showpoint << movie_categ[i].adult_num * cat_price[1] << endl;
            // Senior Citizens
            cout << left << setfill(' ') << setw(20) << "Senior Citizens" << "= " << movie_categ[i].sCitizen_num <<
                " @ RM " << left << setfill(' ') << setw(7) << cat_price[2] << " = RM " <<
                right << setfill(' ') << setw(6) << fixed << setprecision(2) << showpoint << movie_categ[i].sCitizen_num * cat_price[2]
                << endl;


            cout << left << setfill(' ') << setw(5) << ' ';
            cout << left << setfill(' ') << setw(32) << left << "Total to Pay" << "= RM " << right << setfill(' ') << setw(6) <<
                fixed << setprecision(2) << showpoint << total_price << endl;
            cout << left << setfill(' ') << setw(5) << ' ';
            // Here
            cout << left << setfill(' ') << setw(32) << left << "CASH PAYMENT" << "  RM " << right << setfill(' ') << setw(6) <<
                fixed << setprecision(2) << showpoint << cash_paid << endl;
            cout << left << setfill(' ') << setw(5) << ' ';
            cout << left << setfill(' ') << setw(32) << left << "CHANGE DUE" << "  RM " << right << setfill(' ') << setw(6) <<
                fixed << setprecision(2) << showpoint << change_due << endl;
            cout << endl;

            cout << setfill('-') << setw(50) << '-' << endl << endl << endl;
            break;
        }
    }      
}

void ticket_display(int n, string movie_date, string movie[], string movie_time[], int movie_code, int class_selected, 
            double student_price[], double cat_price[], seatMovie movie_seat[], catMovie movie_categ[]) {
    
    int count = 0;
    double price_class[3][3] = { { 14, 18, 9 },
                                { 16, 20, 10 },
                                { 20, 26, 13 } };  //Define prices of 3 classes of seats into arrays


    cout << "Here are your tickets : " << endl;

    for (int i = 0; i < n; i++) {
        if (movie_code == i) {
            // Print Children Tickets 
            if (movie_categ[i].child_num > 0) {
                for (int j = 0; j < movie_categ[i].child_num; j++) {
                    // Header
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    cout << setfill(' ') << setw(23) << "TAR StarMax Ticket";
                    cout << "CHILD - Class " << class_selected << endl << endl;
                    // Details
                    cout << setfill(' ') << setw(7) << "Movie" << ": " << setfill(' ') << setw(6) << ' ' <<
                        setfill(' ') << setw(25) << movie[i] << endl;
                    cout << setfill(' ') << setw(7) << "Date" << ": " << setfill(' ') << setw(6) << ' ' << movie_date << endl;
                    cout << setfill(' ') << setw(7) << "Time" << ": " << setfill(' ') << setw(6) << ' ' << movie_time[i] << endl;
                    
                    if (class_selected == 1)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat1[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 2)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat2[count] << setfill(' ')
                            << setw(3) << ' ';
                    else if (class_selected == 3)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat3[count] << setfill(' ')
                            << setw(3) << ' ';
                    cout << "Price : RM " << cat_price[0] << endl << endl;
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    count++;
                }
            }

            // Student
            if (movie_categ[i].df_num > 0) {
                for (int j = 0; j < movie_categ[i].df_num; j++) {
                    // Header
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    cout << setfill(' ') << setw(23) << "TAR StarMax Ticket";
                    cout << "STUDENT(D/F) - Class " << class_selected << endl << endl;
                    // Details
                    cout << setfill(' ') << setw(7) << "Movie" << ": " << setfill(' ') << setw(6) << ' ' <<
                        setfill(' ') << setw(25) << movie[i] << endl;
                    cout << setfill(' ') << setw(7) << "Date" << ": " << setfill(' ') << setw(6) << ' ' << movie_date << endl;
                    cout << setfill(' ') << setw(7) << "Time" << ": " << setfill(' ') << setw(6) << ' ' << movie_time[i] << endl;
                    
                    if (class_selected == 1)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat1[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 2)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat2[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 3)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat3[count] << setfill(' ') 
                            << setw(3) << ' ';
                    cout << "Price : RM " << student_price[0] << endl << endl;
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    count++;
                }
            }
            if (movie_categ[i].bd_num > 0) {
                for (int j = 0; j < movie_categ[i].bd_num; j++) {
                    // Header
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    cout << setfill(' ') << setw(23) << "TAR StarMax Ticket";
                    cout << "STUDENT(Degree) - Class " << class_selected << endl << endl;
                    // Details
                    cout << setfill(' ') << setw(7) << "Movie" << ": " << setfill(' ') << setw(6) << ' ' <<
                        setfill(' ') << setw(25) << movie[i] << endl;
                    cout << setfill(' ') << setw(7) << "Date" << ": " << setfill(' ') << setw(6) << ' ' << movie_date << endl;
                    cout << setfill(' ') << setw(7) << "Time" << ": " << setfill(' ') << setw(6) << ' ' << movie_time[i] << endl;
                    
                    if (class_selected == 1)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat1[count] << setfill(' ') 
                            << setw(3) << ' '; // X done
                    else if (class_selected == 2)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat2[count] << setfill(' ') 
                            << setw(3) << ' ';
                    if (class_selected == 3)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat3[count] << setfill(' ') 
                            << setw(3) << ' ';
                    cout << "Price : RM " << student_price[0] << endl << endl;
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    count++;
                }
            }
            if (movie_categ[i].phd_num > 0) {
                for (int j = 0; j < movie_categ[i].phd_num; j++) {
                    // Header
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    cout << setfill(' ') << setw(23) << "TAR StarMax Ticket";
                    cout << "STUDENT(PhD) - Class " << class_selected << endl << endl;
                    // Details
                    cout << setfill(' ') << setw(7) << "Movie" << ": " << setfill(' ') << setw(6) << ' ' <<
                        setfill(' ') << setw(25) << movie[i] << endl;
                    cout << setfill(' ') << setw(7) << "Date" << ": " << setfill(' ') << setw(6) << ' ' << movie_date << endl;
                    cout << setfill(' ') << setw(7) << "Time" << ": " << setfill(' ') << setw(6) << ' ' << movie_time[i] << endl;
                    
                    if (class_selected == 1)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat1[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 2)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat2[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 3)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat3[count] << setfill(' ') 
                            << setw(3) << ' ';
                    cout << "Price : RM " << student_price[2] << endl << endl;
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    count++;
                }
            }
            // Adult
            if (movie_categ[i].adult_num > 0) {
                for (int j = 0; j < movie_categ[i].adult_num; j++) {
                    // Header
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    cout << setfill(' ') << setw(23) << "TAR StarMax Ticket";
                    cout << "ADULT - Class " << class_selected << endl << endl;
                    // Details
                    cout << setfill(' ') << setw(7) << "Movie" << ": " << setfill(' ') << setw(6) << ' ' <<
                        setfill(' ') << setw(25) << movie[i] << endl;
                    cout << setfill(' ') << setw(7) << "Date" << ": " << setfill(' ') << setw(6) << ' ' << movie_date << endl;
                    cout << setfill(' ') << setw(7) << "Time" << ": " << setfill(' ') << setw(6) << ' ' << movie_time[i] << endl;
                    
                    if (class_selected == 1)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat1[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 2)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat2[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 3)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat3[count] << setfill(' ') 
                            << setw(3) << ' ';
                    cout << "Price : RM " << cat_price[1] << endl << endl;
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    count++;
                }
            }
            // Senior Citizen
            if (movie_categ[i].sCitizen_num > 0) {
                for (int j = 0; j < movie_categ[i].sCitizen_num; j++) {
                    // Header
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    cout << setfill(' ') << setw(23) << "TAR StarMax Ticket";
                    cout << "S/C - Class " << class_selected << endl << endl;
                    // Details
                    cout << setfill(' ') << setw(7) << "Movie" << ": " << setfill(' ') << setw(6) << ' ' <<
                        setfill(' ') << setw(25) << movie[i] << endl;
                    cout << setfill(' ') << setw(7) << "Date" << ": " << setfill(' ') << setw(6) << ' ' << movie_date << endl;
                    cout << setfill(' ') << setw(7) << "Time" << ": " << setfill(' ') << setw(6) << ' ' << movie_time[i] << endl;
                    
                    if (class_selected == 1)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat1[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 2)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat2[count] << setfill(' ') 
                            << setw(3) << ' ';
                    else if (class_selected == 3)
                        cout << setfill(' ') << setw(7) << "Seat No" << ": " << movie_seat[i].bookedSeat3[count] << setfill(' ') 
                            << setw(3) << ' ';
                    cout << "Price : RM " << price_class[class_selected - 1][0] << endl << endl;
                    cout << left << setfill('=') << setw(50) << '=' << endl;
                    count++;
                }
            }
            break;
        }
    }   
}

void salesReport_display(int n, string movie_date, string movie[], string movie_time[], int movie_code, seatMovie movie_seat[], 
                catMovie movie_categ[]) {
  // Declaration & Initialization of variables
    int total_num = 0;
    int select = 0;
    double total_sales = 0;

    do {
        // Codes
        for (int j = 0; j < 4; j++) {
            if (j == 0) {
                cout << left << setfill(' ') << setw(4) << ' ' << setfill(' ') << setw(7) << ' ' << setfill(' ') << setw(7) <<
                    "Sales Report System" << endl;
            }
            cout << j + 1 << ". | ";
            if (j == 3) {
                cout << left << setfill(' ') << setw(19) << "Exit" << endl;
                break;
            }
            cout << left << setfill(' ') << setw(19) << movie[j] << endl;
        }

        // Sales Report
        cout << endl;
        cout << "Please select the movie you would like to check (1/2/3) : ";
        cin >> select;
        cout << endl;

        int seat1_sold = 30 - movie_seat[select-1].slot1;
        int seat2_sold = 30 - movie_seat[select-1].slot2;
        int seat3_sold = 30 - movie_seat[select-1].slot3;
        int total_seat = seat1_sold + seat2_sold + seat3_sold;

        for (int i = 0; i < 3; i++) {
            if (select-1 == i) {
                // Printing Sales Report
                cout << left << setfill('=') << setw(50) << '=' << endl;
                cout << setfill(' ') << setw(15) << ' ' << "TICKET SALES REPORT" << endl << endl;

                // Title
                cout << setfill(' ') << setw(12) << "Movie Title" << ":" << setfill(' ') << setw(4) << ' ' << movie[i] << endl;
                cout << setfill(' ') << setw(12) << "Date / Time" << ":" << setfill(' ') << setw(4) << ' ' << movie_date <<
                    " (" << movie_time[i] << ')' << endl << endl;

                // Class Occupancy
                cout << "SEAT OCCUPANCY FOR EACH CLASS :" << endl << endl;
                // Table Header
                cout << left << setfill(' ') << setw(10) << ' ';
                cout << left << setfill(' ') << setw(13) << "Total Seats";
                cout << left << setfill(' ') << setw(15) << "Quantity Sold";
                cout << left << setfill(' ') << setw(7) << "Sold %" << endl;
                // Table Row 1
                cout << ' ' << left << setfill(' ') << setw(9) << "Class 1";
                cout << right << setfill(' ') << setw(6) << 30;
                cout << setfill(' ') << setw(13) << seat1_sold;
                cout << setfill(' ') << setw(12) << int(round(double(seat1_sold * 100 / 30))) << '%';
                cout << endl;
                // Table Row 2
                cout << ' ' << left << setfill(' ') << setw(9) << "Class 2";
                cout << right << setfill(' ') << setw(6) << 30;
                cout << setfill(' ') << setw(13) << seat2_sold;
                cout << setfill(' ') << setw(12) << int(round(double(seat2_sold * 100 / 30))) << '%';
                cout << endl;
                // Table Row 3
                cout << ' ' << left << setfill(' ') << setw(9) << "Class 3";
                cout << right << setfill(' ') << setw(6) << 30;
                cout << setfill(' ') << setw(13) << seat3_sold;
                cout << setfill(' ') << setw(12) << int(round(double(seat3_sold * 100 / 30))) << '%';
                // Row Overall
                cout << endl << setfill(' ') << setw(13) << ' ' << "----" <<
                    setfill(' ') << setw(13) << "----" << setfill(' ') << setw(13) << "------" << endl;
                cout << ' ' << left << setfill(' ') << setw(13) << "OVERALL" << 90;
                cout << right << setfill(' ') << setw(13) << seat1_sold + seat2_sold + seat3_sold;
                cout << setfill(' ') << setw(12) << int(round(double(total_seat) * 100 / 90)) << '%';
                cout << endl << endl;

                // Sales Detail
                cout << "SALES FOR EACH CATEGORY TYPE" << endl << endl;
                cout << left << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(12) << "Category" << setfill(' ') << setw(8) << "Seats" <<
                    "Sales (RM)" << endl;
                cout << left << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(12) << "--------" << setfill(' ') << setw(8) << "-----" <<
                    "----------" << endl;
                // Child
                cout << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(14) << "CHILD" << setfill(' ') << setw(7) <<
                    setfill(' ') << setw(5) << movie_categ[i].total_seat[0] << right << setfill(' ') << setw(11)
                    << movie_categ[i].total_sales[0] << endl;
                // Student
                cout << left << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(14) << "STUDENT" << setfill(' ') << setw(7) <<
                    setfill(' ') << setw(5) << movie_categ[i].total_seat[1] << right << setfill(' ') << setw(11)
                    << movie_categ[i].total_sales[1] << endl;
                // Adult
                cout << left << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(14) << "ADULT" << setfill(' ') << setw(7) <<
                    setfill(' ') << setw(5) << movie_categ[i].total_seat[2] << right << setfill(' ') << setw(11)
                    << movie_categ[i].total_sales[2] << endl;
                // Senior Citizen
                cout << left << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(14) << "S/C" << setfill(' ') << setw(7) <<
                    setfill(' ') << setw(5) << movie_categ[i].total_seat[3] << right << setfill(' ') << setw(11)
                    << movie_categ[i].total_sales[3] << endl;
                // Total
                for (int x = 0; x < n; x++) {
                    total_num += movie_categ[i].total_seat[x];
                    total_sales += movie_categ[i].total_sales[x];
                }
                cout << left << setfill(' ') << setw(20) << ' ' << "-----" << right << setfill(' ') << setw(13) << "--------" << endl;
                cout << left << setfill(' ') << setw(8) << ' ' << setfill(' ') << setw(14) << "TOTAL" << setfill(' ') << setw(7) <<
                    setfill(' ') << setw(5) << total_num << right << setfill(' ') << setw(11) << total_sales << endl;

                // End
                cout << left << setfill('=') << setw(50) << '=' << endl << endl;
                break;
            }
        }
    } while (select != 4);
}
