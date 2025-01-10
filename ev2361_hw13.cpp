#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

class Organism {
   public:
    bool hasMoved() { return animalHasMoved; };
    virtual char getType() = 0;
    virtual bool handleDeath(Organism* (&antFarm)[20][20], int row,
                             int col) = 0;
    virtual void breed(Organism* (&antFarm)[20][20], int row, int col) = 0;
    virtual void move(Organism* (&antFarm)[20][20], int currentRow,
                      int currentCol) = 0;
    void operator--(int) { numberOfMovesTillBreed--; }
    virtual void speak() = 0;
    int getNumberOfMovesTillBreed() { return numberOfMovesTillBreed; }
    void setIsMoved(bool isMoved) { animalHasMoved = isMoved; }
    void setNumberOfMovesTillBreed(int numberOfMoves) {
        numberOfMovesTillBreed = numberOfMoves;
    };

   protected:
    bool animalHasMoved = false;
    int numberOfMovesTillBreed = 0;
};

class Ant : public Organism {
   public:
    Ant() { numberOfMovesTillBreed = 3; }
    void move(Organism* (&antFarm)[20][20], int currentRow,
              int currentCol) override;

    bool handleDeath(Organism* (&antFarm)[20][20], int row, int col) {
        return true;  // No special death logic for ants
    }
    void speak() override;
    void breed(Organism* (&antFarm)[20][20], int row, int col) override;
    char getType() override;
};

class Doodlebug : public Organism {
   public:
    Doodlebug() { numberOfMovesTillBreed = 8; }
    bool handleDeath(Organism* (&antFarm)[20][20], int row, int col) override {
        if (numberOfMovesTillDeath <= 0) {
            antFarm[row][col] = nullptr;  // Remove the Doodlebug from the grid
            delete this;
            return true;
        }
        return false;
    }
    void move(Organism* (&antFarm)[20][20], int currentRow,
              int currentCol) override;
    void speak() override;
    void breed(Organism* (&antFarm)[20][20], int row, int col) override;
    int getNumberOfMovesTillDeath();
    char getType() override;

   private:
    int numberOfMovesTillDeath = 3;
};

bool isEmptyCell(Organism* (&antFarm)[20][20], int row, int col) {
    if (row < 0 || row >= 20 || col < 0 || col >= 20) {
        return false;  // Out of bounds
    }
    return antFarm[row][col] == nullptr;
}

bool cellContainsAnt(Organism* (&antFarm)[20][20], int row, int col) {
    if (row < 0 || row >= 20 || col < 0 || col >= 20) {
        return false;  // Out of bounds
    }
    return antFarm[row][col] != nullptr && antFarm[row][col]->getType() == 'o';
}

void Ant::move(Organism* (&antFarm)[20][20], int currentRow, int currentCol) {
    int leftRow = currentRow, leftCol = currentCol - 1;
    int rightRow = currentRow, rightCol = currentCol + 1;
    int aboveRow = currentRow - 1, aboveCol = currentCol;
    int belowRow = currentRow + 1, belowCol = currentCol;

    if (isEmptyCell(antFarm, leftRow, leftCol)) {
        antFarm[leftRow][leftCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
    } else if (isEmptyCell(antFarm, rightRow, rightCol)) {
        antFarm[rightRow][rightCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
    } else if (isEmptyCell(antFarm, aboveRow, aboveCol)) {
        antFarm[aboveRow][aboveCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
    } else if (isEmptyCell(antFarm, belowRow, belowCol)) {
        antFarm[belowRow][belowCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
    }
}

void Ant::speak() { cout << "I AM AN ANT"; }

void Ant::breed(Organism* (&antFarm)[20][20], int row, int col) {
    cout << "ANT BREEDING";
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& dir : directions) {
        int newRow = row + dir[0], newCol = col + dir[1];
        if (isEmptyCell(antFarm, newRow, newCol)) {
            antFarm[newRow][newCol] = new Ant();
            return;
        }
    }
}

char Ant::getType() { return 'o'; }

void Doodlebug::speak() { cout << "I AM A DOODLE BUG"; }

void Doodlebug::breed(Organism* (&antFarm)[20][20], int row, int col) {
    cout << "doodle bug breeding";
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (const auto& dir : directions) {
        int newRow = row + dir[0], newCol = col + dir[1];
        if (isEmptyCell(antFarm, newRow, newCol)) {
            cout << "row" << newRow << "col" << newCol;
            antFarm[newRow][newCol] = new Doodlebug();
            return;
        }
    }
}

int Doodlebug::getNumberOfMovesTillDeath() { return numberOfMovesTillDeath; }

void Doodlebug::move(Organism* (&antFarm)[20][20], int currentRow,
                     int currentCol) {
    int leftRow = currentRow, leftCol = currentCol - 1;
    int rightRow = currentRow, rightCol = currentCol + 1;
    int aboveRow = currentRow - 1, aboveCol = currentCol;
    int belowRow = currentRow + 1, belowCol = currentCol;

    if (cellContainsAnt(antFarm, leftRow, leftCol)) {
        delete antFarm[leftRow][leftCol];
        antFarm[leftRow][leftCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath = 3;
        return;
    } else if (cellContainsAnt(antFarm, rightRow, rightCol)) {
        delete antFarm[rightRow][rightCol];
        antFarm[rightRow][rightCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath = 3;
        return;
    } else if (cellContainsAnt(antFarm, aboveRow, aboveCol)) {
        delete antFarm[aboveRow][aboveCol];
        antFarm[aboveRow][aboveCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath = 3;
        return;
    } else if (cellContainsAnt(antFarm, belowRow, belowCol)) {
        delete antFarm[belowRow][belowCol];
        antFarm[belowRow][belowCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath = 3;
        return;
    }

    if (isEmptyCell(antFarm, leftRow, leftCol)) {
        antFarm[leftRow][leftCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath--;
    } else if (isEmptyCell(antFarm, rightRow, rightCol)) {
        antFarm[rightRow][rightCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath--;
    } else if (isEmptyCell(antFarm, aboveRow, aboveCol)) {
        antFarm[aboveRow][aboveCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath--;
    } else if (isEmptyCell(antFarm, belowRow, belowCol)) {
        antFarm[belowRow][belowCol] = this;
        antFarm[currentRow][currentCol] = nullptr;
        numberOfMovesTillDeath--;
    }
}

char Doodlebug::getType() { return 'X'; }

void initializeAntFarm(Organism* (&antFarm)[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            antFarm[i][j] = nullptr;
        }
    }

    int numberOfAnts = 100, numberOfDoodleBugs = 5;

    while (numberOfAnts > 0) {
        int row = rand() % 20, col = rand() % 20;
        if (antFarm[row][col] == nullptr) {
            antFarm[row][col] = new Ant();
            numberOfAnts--;
        }
    }

    while (numberOfDoodleBugs > 0) {
        int row = rand() % 20, col = rand() % 20;
        if (antFarm[row][col] == nullptr) {
            antFarm[row][col] = new Doodlebug();
            numberOfDoodleBugs--;
        }
    }
}

void renderAntFarm(Organism* (&antFarm)[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (antFarm[i][j] != nullptr) {
                cout << antFarm[i][j]->getType() << " ";
            } else {
                cout << "- ";
            }
        }
        cout << endl;
    }
}
void tick(Organism* (&antFarm)[20][20]) {
    renderAntFarm(antFarm);

    // Step 1: Move Doodlebugs first
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Organism* current = antFarm[i][j];
            if (current && !current->hasMoved() && current->getType() == 'X') {
                current->move(antFarm, i, j);
                current->setIsMoved(true);
                current->setNumberOfMovesTillBreed(
                    current->getNumberOfMovesTillBreed() - 1);
            }
        }
    }

    // Step 2: Move Ants
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Organism* current = antFarm[i][j];
            if (current && !current->hasMoved() && current->getType() == 'o') {
                current->move(antFarm, i, j);
                current->setIsMoved(true);
                current->setNumberOfMovesTillBreed(
                    current->getNumberOfMovesTillBreed() - 1);
            }
        }
    }

    // Step 3: Handle Doodlebug starvation and breeding
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Organism* current = antFarm[i][j];
            if (current && current->getType() == 'X') {
                bool hasDied = current->handleDeath(antFarm, i, j);
                if (!hasDied && current->getNumberOfMovesTillBreed() == 0) {
                    current->breed(antFarm, i, j);
                    current->setNumberOfMovesTillBreed(
                        8);  // Reset breeding counter
                }
            }
        }
    }

    // Step 4: Handle Ant breeding
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            Organism* current = antFarm[i][j];
            if (current && current->getType() == 'o') {
                if (current->getNumberOfMovesTillBreed() == 0) {
                    current->breed(antFarm, i, j);
                    current->setNumberOfMovesTillBreed(
                        3);  // Reset breeding counter
                }
            }
        }
    }

    // Step 5: Reset movement flags for all organisms
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (antFarm[i][j]) {
                antFarm[i][j]->setIsMoved(false);  // Reset the moved flag
            }
        }
    }
}

void waitForEnter() {
    cout << "Press ENTER to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    srand(time(0));
    Organism* antFarm[20][20];
    initializeAntFarm(antFarm);
    int turns = 0;
    while (true) {
        cout << "Turn " << turns + 1 << ":\n";
        tick(antFarm);
        waitForEnter();
        cout << endl;
        turns++;
    }
    return 0;
}
