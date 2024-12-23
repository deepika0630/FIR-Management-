#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

const string filename = "fir.txt";
const string indexFilename = "index.txt";

struct FIRRecord {
    string FIRNumber;
    string victimName;
    string accusedName;
    string date;
    string time;
    string description;
};

// Function to write FIR record to file
void writeFIRRecord(const FIRRecord& record) {
    ofstream file(filename, ios::app);
    if (file.is_open()) {
        file << record.FIRNumber << "|" << record.victimName << "|" << record.accusedName << "|"
             << record.date << "|" << record.time << "|" << record.description << "|" << endl;
        file.close();
    } else {
        cout << "Error: Unable to open file for writing." << endl;
    }
}

// Function to create index based on FIR numbers
void createIndex() {
    ifstream file(filename);
    ofstream indexFile(indexFilename);
    unordered_map<string, long long> index;

    if (file.is_open() && indexFile.is_open()) {
        string line;
        long long lineNum = 0;
        while (getline(file, line)) {
            ++lineNum;
            size_t pos = line.find('|');
            if (pos != string::npos) {
                string FIRNumber = line.substr(0, pos);
                index[FIRNumber] = lineNum;
            }
        }

        for (const auto& entry : index) {
            indexFile << entry.first << " " << entry.second << endl;
        }

        file.close();
        indexFile.close();
    } else {
        cout << "Error: Unable to open files for indexing." << endl;
    }
}

// Function to search and retrieve FIR record by FIR number
void searchFIRRecord(const string& FIRNumber) {
    ifstream indexFile(indexFilename);
    ifstream file(filename);

    if (indexFile.is_open() && file.is_open()) {
        unordered_map<string, long long> index;
        string indexLine;
        while (getline(indexFile, indexLine)) {
            size_t spacePos = indexLine.find(' ');
            if (spacePos != string::npos) {
                string FIR = indexLine.substr(0, spacePos);
                long long lineNum = stoll(indexLine.substr(spacePos + 1));
                index[FIR] = lineNum;
            }
        }

        if (index.count(FIRNumber) > 0) {
            long long lineNum = index[FIRNumber];
            string line;
            long long currentLineNum = 0;
            while (getline(file, line)) {
                ++currentLineNum;
                if (currentLineNum == lineNum) {
                    FIRRecord record;
                    size_t pos = 0;
                    for (int i = 0; i < 7; ++i) {
                        size_t nextPos = line.find('|', pos);
                        if (nextPos != string::npos) {
                            string value = line.substr(pos, nextPos - pos);
                            switch (i) {
                                case 0: record.FIRNumber = value; break;
                                case 1: record.victimName = value; break;
                                case 2: record.accusedName = value; break;
                                case 3: record.date = value; break;
                                case 4: record.time = value; break;
                                case 5: record.description = value; break;
                            }
                            pos = nextPos + 1;
                        }
                    }

                    cout << "FIR Number: " << record.FIRNumber << endl;
                    cout << "Victim Name: " << record.victimName << endl;
                    cout << "Accused Name: " << record.accusedName << endl;
                    cout << "Date: " << record.date << endl;
                    cout << "Time: " << record.time << endl;
                    cout << "Description: " << record.description << endl;
                    break;
                }
            }
        } else {
            cout << "No FIR record found with the given FIR number." << endl;
        }

        indexFile.close();
        file.close();
    } else {
        cout << "Error: Unable to open files for searching." << endl;
    }
}

// Function to view all FIR records
void viewFIRRecords() {
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            FIRRecord record;
            size_t pos = 0;
            for (int i = 0; i < 7; ++i) {
                size_t nextPos = line.find('|', pos);
                if (nextPos != string::npos) {
                    string value = line.substr(pos, nextPos - pos);
                    switch (i) {
                        case 0: record.FIRNumber = value; break;
                        case 1: record.victimName = value; break;
                        case 2: record.accusedName = value; break;
                        case 3: record.date = value; break;
                        case 4: record.time = value; break;
                        case 5: record.description = value; break;
                    }
                    pos = nextPos + 1;
                }
            }

            cout << "----- FIR Record -----" << endl;
            cout << "FIR Number: " << record.FIRNumber << endl;
            cout << "Victim Name: " << record.victimName << endl;
            cout << "Accused Name: " << record.accusedName << endl;
            cout << "Date: " << record.date << endl;
            cout << "Time: " << record.time << endl;
            cout << "Description: " << record.description << endl;
            cout << endl;
        }

        file.close();
    } else {
        cout << "Error: Unable to open file for viewing records." << endl;
    }
}

// Function to delete FIR record by FIR number
void deleteFIRRecord(const string& FIRNumber) {
    ifstream indexFile(indexFilename);
    ifstream file(filename);
    ofstream tempFile("temp.txt");

    if (indexFile.is_open() && file.is_open() && tempFile.is_open()) {
        unordered_map<string, long long> index;
        string indexLine;
        while (getline(indexFile, indexLine)) {
            size_t spacePos = indexLine.find(' ');
            if (spacePos != string::npos) {
                string FIR = indexLine.substr(0, spacePos);
                long long lineNum = stoll(indexLine.substr(spacePos + 1));
                index[FIR] = lineNum;
            }
        }

        if (index.count(FIRNumber) > 0) {
            long long lineNum = index[FIRNumber];
            string line;
            long long currentLineNum = 0;
            while (getline(file, line)) {
                ++currentLineNum;
                if (currentLineNum != lineNum) {
                    tempFile << line << endl;
                }
            }

            cout << "FIR record with FIR number " << FIRNumber << " has been deleted." << endl;

            indexFile.close();
            file.close();
            tempFile.close();

            remove(filename.c_str());
            rename("temp.txt", filename.c_str());

            // Recreate the index after deletion
            createIndex();
        } else {
            cout << "No FIR record found with the given FIR number." << endl;
            indexFile.close();
            file.close();
            tempFile.close();
            remove("temp.txt");
        }
    } else {
        cout << "Error: Unable to open files for deleting records." << endl;
    }
}

int main() {
    int choice;
    do {
        cout << "FIR Management System" << endl;
        cout << "1. Add FIR Record" << endl;
        cout << "2. View FIR Records" << endl;
        cout << "3. Search FIR Record" << endl;
        cout << "4. Delete FIR Record" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                FIRRecord record;
                cout << "Enter FIR Number: ";
                cin >> record.FIRNumber;
                cout << "Enter Victim Name: ";
                cin.ignore();
                getline(cin, record.victimName);
                cout << "Enter Accused Name: ";
                getline(cin, record.accusedName);
                cout << "Enter Date: ";
                getline(cin, record.date);
                cout << "Enter Time: ";
                getline(cin, record.time);
                cout << "Enter Description: ";
                getline(cin, record.description);

                writeFIRRecord(record);
                createIndex();

                cout << "FIR record added successfully." << endl;
                break;
            }
            case 2:
                viewFIRRecords();
                break;
            case 3: {
                string FIRNumber;
                cout << "Enter FIR Number: ";
                cin >> FIRNumber;
                searchFIRRecord(FIRNumber);
                break;
            }
            case 4: {
                string FIRNumber;
                cout << "Enter FIR Number: ";
                cin >> FIRNumber;
                deleteFIRRecord(FIRNumber);
                break;
            }
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        cout << endl;
    } while (choice != 5);

    return 0;
}