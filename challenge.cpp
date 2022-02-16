#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

struct Reservation {
  string name;
  vector <string> seats;
};

class Theater {
  private: int stage;
  private: int curRow;
  private: int curCol;
  private: bool seating[10][20];
  private: int numOccupied; // occupancy
  private: const int CAPACITY = 200;

  public: Theater() {
    stage = 1;
    numOccupied = 0;
    memset(seating, false, sizeof seating);
    curRow = 4;
    curCol = 0;
  }

  private: string convToString(int row, int col) {
    string ret;
    ret += (row + 'A');
    if(col/10)
      ret += (1 + '0');
    ret += (col%10) + '0';
    return ret;
  }

  private: void stageOne(int numOfPeople, Reservation *currentReservation) {

    // Stage one: Starting at the center and going up leaving a buffer of 3 seats

    // attempting to fit entire group in one row
    if(curRow > 0 && (20-curCol) < numOfPeople && numOfPeople <= 20) {
      curRow--;
      curCol = 0;
    }

    while(numOfPeople--) {
      numOccupied++;
      seating[curRow][curCol] = true;
      currentReservation -> seats.push_back(convToString(curRow, curCol));
      curCol++;

      if(curCol == 20) {
        curRow--;
        curCol=0;
      }

      if(curRow == -1) {
        curRow = 5;
        curCol = 0;
        stage++;
        stageTwo(numOfPeople, currentReservation);
        return;
      }
    }

    curCol+=3;
    if(curCol >= 20) {
      curRow--;
      curCol = 0;
    }

    if(curRow == -1) {
      curRow = 5;
      curCol = 0;
      stage++;
    }
  }

  private: void stageTwo(int numOfPeople, Reservation *currentReservation) {

    // Stage two: Starting at (center+1) and going down leaving a buffer of 3 seats

    // attempting to fit entire group in one row

    if(curRow > 0 && (20-curCol) < numOfPeople && numOfPeople <= 20) {
      curRow++;
      curCol = 0;
    }

    while(numOfPeople--) {
      numOccupied++;
      seating[curRow][curCol] = true;
      currentReservation -> seats.push_back(convToString(curRow, curCol));
      curCol++;

      if(curCol == 20) {
        curRow++;
        curCol=0;
      }

      if(curRow == 10) {
        curRow = 5;
        curCol = 0;
        stage++;
        stageThree(numOfPeople, currentReservation);
        return;
      }
    }

    curCol+=3;
    if(curCol >= 20) {
      curRow++;
      curCol = 0;
    }

    if(curRow == 10) {
      curRow = 5;
      curCol = 0;
      stage++;
    }
  }

  private: void stageThree(int numOfPeople, Reservation *currentReservation) {
    // Stage three: Starting at center and filling up all the gaps

    while(numOfPeople) {
      if(!seating[curRow][curCol]) {
        numOccupied++;
        seating[curRow][curCol] = true;
        currentReservation -> seats.push_back(convToString(curRow, curCol));
        numOfPeople--;
      }

      curCol++;

      if(curCol == 20) {
        curRow--;
        curCol=0;
      }

      if(curRow == -1) {
        curRow = 5;
        curCol = 0;
        stage++;
        stageFour(numOfPeople, currentReservation);
        break;
      }
    }
  }

  private: void stageFour(int numOfPeople, Reservation *currentReservation) {
    // Stage Four: Starting at (center+1) and going down, filling up all the gaps

    while(numOfPeople) {
      if(!seating[curRow][curCol]) {
        numOccupied++;
        seating[curRow][curCol] = true;
        currentReservation -> seats.push_back(convToString(curRow, curCol));
        numOfPeople--;
      }

      curCol++;

      if(curCol == 20) {
        curRow++;
        curCol=0;
      }

      if(curRow == 10) {
        break;
      }
    }
  }

  public: bool assign(int numOfPeople, Reservation *currentReservation) {
    if(CAPACITY - numOfPeople < 0)
      return false;
    if(stage == 1)
      stageOne(numOfPeople, currentReservation);
    else if(stage == 2)
      stageTwo(numOfPeople, currentReservation);
    else if(stage == 3)
      stageThree(numOfPeople, currentReservation);
    else if(stage == 4)
      stageFour(numOfPeople, currentReservation);
    return true;
  }

  public: void helper() { // helps print the state of the theater at any point. o: occupied, e: empty
    for(int i=0; i<10; i++) {
      for(int j=0; j<20; j++) {
        if(seating[i][j])
          cout << 'o';
        else
          cout << 'e';
      }

      cout << endl;
    }
  }
};

int convToInt(string S) {
  int ret = 0;
  for(int i=0; i<S.length(); i++) {
    ret *= 10;
    ret += (S[i] - '0');
  }

  return ret;
}

bool solve(string S, vector<Reservation*>&Reservations, Theater &t) {
  string A="", B="";
  bool doneA=false;

  for(int i=0; i<S.length(); i++) {
    if(doneA)
      B+=S[i];
    else if(S[i] == ' ')
      doneA=true;
    else
      A+=S[i];
  }

  Reservation *cur = new Reservation();
  cur -> name = A;

  int get = convToInt(B);
  if(!t.assign(convToInt(B), cur)) {
    return false;
  }

  Reservations.push_back(cur);
  return true;
}

int main(int argc, char *argv[]) {
  Theater x;
  fstream newfile;

  vector<Reservation*>Reservations;

  newfile.open(argv[1], ios::in);
  string tp;

  while(getline(newfile, tp)) {
    solve(tp, Reservations, x);
  }

  newfile.close();
  ofstream outputFile;

  outputFile.open("output.txt");

  for(Reservation* r: Reservations) {
    outputFile << r -> name << " ";

    for(string s: r -> seats) {
      outputFile << s << " ";
    }

    outputFile << endl;
  }

  x.helper();
  cout << "/output.txt" << endl;
  return 0;
}
