#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

// ------------------- Train Class -------------------
class Train
{
public:
    int trainNumber;
    string trainName;
    string source, destination;
    int availableseats;

    Train(int tNum, string tName, string src, string dest, int seats = 30)
        : trainNumber(tNum), trainName(tName), source(src), destination(dest), availableseats(seats) {}

    void displaytraindetails() const
    {
        cout << "Train Number: " << trainNumber
             << "\nTrain Name: " << trainName
             << "\nSource: " << source
             << "\nDestination: " << destination
             << "\nAvailable Seats: " << availableseats << "\n"
             << endl;
    }

    Train() : trainNumber(0), trainName(""), source(""), destination(""), availableseats(0) {}
};

// ------------------- Passenger Base Class -------------------
class Passenger
{
public:
    string Name;
    int age;
    string type;

    Passenger(string pName, int pAge, string tp)
        : Name(pName), age(pAge), type(tp) {}

    virtual float calculatefare(float basefare)
    {
        return basefare;
    }

    virtual void displaypassengerdetails()
    {
        cout << "Passenger Name: " << Name
             << "\nPassenger Age: " << age
             << "\nPassenger Category: " << type << endl;
    }

    virtual ~Passenger() {}
};

// ------------------- Normal Passenger -------------------
class NormalPassenger : public Passenger
{
public:
    NormalPassenger(string pName, int pAge)
        : Passenger(pName, pAge, "Normal") {}

    float calculatefare(float basefare) override
    {
        return basefare;
    }
};

// ------------------- Senior Citizen Passenger -------------------
class SeniorCitizenPassenger : public Passenger
{
public:
    SeniorCitizenPassenger(string pName, int pAge)
        : Passenger(pName, pAge, "Senior") {}

    float calculatefare(float basefare) override
    {
        return basefare * 0.8f;
    }
};

// ------------------- Ticket Class -------------------
class Ticket
{
public:
    int ticketId;
    Train train;
    Passenger *passenger;
    float price;

    Ticket(int id, Train t, Passenger *p, float fare)
        : ticketId(id), train(t), passenger(p), price(fare) {}

    Ticket() : ticketId(0), train(), passenger(nullptr), price(0.0f) {}

    void displayticketdetails()
    {
        cout << "\n------ Ticket ID: " << ticketId << "\nTrainNo. -" << train.trainNumber
             << "\nTrain Name: " << train.trainName
             << "\nPassenger: " << passenger->Name
             << "\nPrice: " << price << "\n";
        passenger->displaypassengerdetails();
        cout << "Route: " << train.source << " -> " << train.destination << "\n"
             << endl;
    }

    void saveToFile(ofstream &outFile)
    {
        outFile << ticketId << "," << passenger->Name << "," << passenger->age << "," << passenger->type << ","
                << price << "," << train.trainNumber << "," << train.trainName << ","
                << train.source << "," << train.destination << "\n";
    }
};

// ------------------- Global Data -------------------
int ticketcounter = 1;
vector<Passenger *> passengerlist;
map<int, Ticket> ticketmap;

// ------------------- File Handling -------------------
void loadTicketsFromFile(vector<Passenger *> &passengerList, map<int, Ticket> &ticketMap)
{
    ifstream inFile("reservations.txt");
    if (!inFile)
        return;

    string line;
    while (getline(inFile, line))
    {
        stringstream ss(line);
        string name, type, temp, trainName, source, destination;
        int age, id, trainID;
        float price;

        getline(ss, temp, ',');
        id = stoi(temp);
        getline(ss, name, ',');
        getline(ss, temp, ',');
        age = stoi(temp);
        getline(ss, type, ',');
        getline(ss, temp, ',');
        price = stof(temp);
        getline(ss, temp, ',');
        trainID = stoi(temp);
        getline(ss, trainName, ',');
        getline(ss, source, ',');
        getline(ss, destination, ',');

        Passenger *p = (type == "Normal") ? static_cast<Passenger *>(new NormalPassenger(name, age))
                                          : static_cast<Passenger *>(new SeniorCitizenPassenger(name, age));

        Train tr(trainID, trainName, source, destination, 0);
        passengerList.push_back(p);
        ticketMap[id] = Ticket(id, tr, p, price);

        if (id >= ticketcounter)
            ticketcounter = id + 1;
    }
    inFile.close();
}

void saveTicketsToFile(map<int, Ticket> &ticketMap)
{
    ofstream outFile("reservations.txt");
    for (auto &[id, ticket] : ticketMap)
    {
        ticket.saveToFile(outFile);
    }
    outFile.close();
}

// ------------------- Main Program -------------------
int main()
{
    float baseFare = 1345.0;

    vector<Train> trainList = {
    //    Train(12155, "Bhopal Pune Express", "Bhopal", "Pune", 52),
        Train(12001, "Habibganj New Delhi Shatabdi Express", "Bhopal", "Delhi", 52),
     //   Train(12953, "August Kranti Rajdhani Express", "Bhopal", "Mumbai", 52),
      //  Train(12920, "Malwa Express", "Bhopal", "Katra", 52),
      //  Train(12485, "Nanda Devi Express", "Bhopal", "Dehradun", 52),
    //    Train(12442, "Bilaspur Rajdhani Express", "Bhopal", "Delhi", 52),
      //  Train(12627, "Karnataka Express", "Bhopal", "Bangalore", 52),
       // Train(12782, "Swarna Jayanti Express", "Bhopal", "Mysuru", 52),
       // Train(12437, "Rajdhani Express", "Bhopal", "Secunderabad", 52),
        Train(12457, "Delhi Kalka Shatabdi Express", "Delhi", "Kalka", 52),
    };

    loadTicketsFromFile(passengerlist, ticketmap);

    int choice;
    do
    {
        cout << "\n----Welcome To Railway Reservation System by Sumit Choudhari---- \n" ;
        cout << "\n===== Railway Reservation Menu =====\n";
        cout << "1. Book Ticket\n2. View All Bookings\n3. Cancel Ticket\n4. Exit\nEnter your choice: ";
        while (!(cin >> choice))
        {
            cout << "Invalid input. Enter a valid choice: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if (choice == 1)
        {
            string name;
            int age;
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);

            cout << "Enter age: ";
            while (!(cin >> age) || age <= 0)
            {
                cout << "Invalid age. Please enter a valid number: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            cout << "\nAvailable Trains:\n";
            for (const Train &tr : trainList)
                tr.displaytraindetails();

            int trainNum;
            cout << "Enter Train Number to book: ";
            while (!(cin >> trainNum))
            {
                cout << "Invalid input. Enter valid Train Number: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            Train *selectedTrain = nullptr;
            for (Train &tr : trainList)
            {
                if (tr.trainNumber == trainNum)
                {
                    selectedTrain = &tr;
                    break;
                }
            }

            if (!selectedTrain)
            {
                cout << "Train not found or invalid train number entered.\n";
                continue;
            }

            // === Seat Availability Check ===
            if (selectedTrain->availableseats <= 0)
            {
                cout << "No seats available on this train.\n";
                continue;
            }

            selectedTrain->availableseats--; // Decrement seat count

            Passenger *p = (age >= 60)
                               ? static_cast<Passenger *>(new SeniorCitizenPassenger(name, age))
                               : static_cast<Passenger *>(new NormalPassenger(name, age));

            float fare = p->calculatefare(baseFare);
            Ticket t(ticketcounter++, *selectedTrain, p, fare);
            passengerlist.push_back(p);
            ticketmap[t.ticketId] = t;

            cout << "Ticket Booked Successfully! Ticket ID: " << t.ticketId << endl;
        }

        else if (choice == 2)
        {
            if (ticketmap.empty())
            {
                cout << "No bookings available.\n";
            }
            else
            {
                for (auto &[id, ticket] : ticketmap)
                {
                    ticket.displayticketdetails();
                }
            }
        }

        else if (choice == 3)
        {
            int id;
            cout << "Enter Ticket ID to cancel: ";
            while (!(cin >> id))
            {
                cout << "Invalid ID. Please enter a number: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            auto it = ticketmap.find(id);
            if (it != ticketmap.end())
            {
                it->second.train.availableseats++; // Restore seat count
                delete it->second.passenger;
                ticketmap.erase(it);
                cout << "Ticket Cancelled Successfully!\n";
            }
            else
            {
                cout << "Ticket ID not found.\n";
            }
        }

        else if (choice == 4)
        {
            saveTicketsToFile(ticketmap);
            for (Passenger *p : passengerlist)
                delete p;
            cout << "Data saved. Exiting...\n";
        }

        else
        {
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}