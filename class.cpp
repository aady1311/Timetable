#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {

    int numClasses, numTeachers, numRooms, numLabs;

    cout << "Enter Number of Classes: ";
    cin >> numClasses;

    vector<string> classNames(numClasses);
    cout << "Enter Class Names:\n";
    for(int i=0;i<numClasses;i++){
        cout << "Class " << i+1 << ": ";
        cin >> classNames[i];
    }

    cout << "\nEnter Number of Teachers: ";
    cin >> numTeachers;

    vector<string> teachers(numTeachers);
    cout << "Enter Teacher Names:\n";
    for(int i=0;i<numTeachers;i++){
        cout << "Teacher " << i+1 << ": ";
        cin >> teachers[i];
    }

    cout << "\nEnter Number of Rooms: ";
    cin >> numRooms;

    cout << "Enter Number of Labs: ";
    cin >> numLabs;

    vector<string> days = {"Monday","Tuesday","Wednesday","Thursday","Friday"};

    vector<string> timeSlots = {
        "9-10","10-11","11-12","12-1",
        "1-2(BREAK)",
        "2-3","3-4"
    };

    int totalSlots = 7;

    // timetable[class][day][slot]
    vector<vector<vector<string>>> timetable(numClasses,
        vector<vector<string>>(5,
            vector<string>(totalSlots)));

    // Teacher busy tracker
    vector<vector<bool>> teacherBusy(numTeachers,
        vector<bool>(5*totalSlots,false));

    // Teacher workload tracker
    vector<int> teacherHours(numTeachers, 0);

    const int MAX_HOURS = 18;
    const int LECTURE_HOURS = 6;
    const int PRACTICAL_HOURS = 10;

    // BASIC RESOURCE CHECK
    if(numTeachers < numClasses){
        cout << "\n❌ Deadlock Possible! Not enough teachers.\n";
        return 0;
    }

    if(numRooms < numClasses){
        cout << "\n❌ Deadlock Possible! Not enough rooms.\n";
        return 0;
    }

    // TIMETABLE GENERATION
    for(int c=0;c<numClasses;c++){
        for(int d=0;d<5;d++){
            for(int s=0;s<totalSlots;s++){

                if(s==4){
                    timetable[c][d][s] = "BREAK";
                    continue;
                }

                bool assigned = false;

                for(int t=0;t<numTeachers;t++){

                    int timeKey = d*totalSlots + s;

                    if(!teacherBusy[t][timeKey] && teacherHours[t] < MAX_HOURS){

                        // Practical slots (afternoon)
                        if(s >= 5 && teacherHours[t] < PRACTICAL_HOURS){
                            timetable[c][d][s] = teachers[t] + "(Lab)";
                        }
                        else{
                            timetable[c][d][s] = teachers[t] + "(Lec)";
                        }

                        teacherBusy[t][timeKey] = true;
                        teacherHours[t]++;
                        assigned = true;
                        break;
                    }
                }

                if(!assigned){
                    cout << "\n❌ Deadlock Detected! Teachers Overloaded.\n";
                    return 0;
                }
            }
        }
    }

    // 🎯 ACTIVITY SLOT FEATURE
    char choice;
    cout << "\nDo you want to add Activity Slot? (y/n): ";
    cin >> choice;

    if(choice == 'y' || choice == 'Y'){
        string activityClass;
        cout << "Enter Class Name for Activity: ";
        cin >> activityClass;

        for(int c=0;c<numClasses;c++){
            if(classNames[c] == activityClass){

                // Add activity on Friday last slot
                timetable[c][4][6] = "Activity";

                cout << "✅ Activity added to " << activityClass << " on Friday 3-4\n";
            }
        }
    }

    // PRINT TIMETABLE
    for(int c=0;c<numClasses;c++){

        cout << "\n\n==============================================\n";
        cout << "Class " << classNames[c] << " Timetable\n";
        cout << "==============================================\n";

        cout << setw(12) << "Time";
        for(int d=0; d<5; d++)
            cout << setw(18) << days[d];
        cout << endl;

        for(int s=0;s<totalSlots;s++){
            cout << setw(12) << timeSlots[s];
            for(int d=0;d<5;d++){
                cout << setw(18) << timetable[c][d][s];
            }
            cout << endl;
        }
    }

    cout << "\n✅ Smart Deadlock-Free Timetable Generated!\n";

    return 0;
}