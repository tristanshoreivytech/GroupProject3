// Rock-Paper-Scissors Early Version

#include <iostream>
using namespace std;

// Set ID for choices
const int rock = 0;
const int paper = 1;
const int scissors = 2;

// Set variables player choice is initialized outside valid choice
int choice = 4;
int com;
bool picked = 0;

// Set computer player choice
int getComChoice() {
    int move;
    // get random number for choice
    srand(time(NULL));
    move = rand() % 3;
    return move;
}

// Declare Results
void results(int player, int computer) {
    // Tie condition
    if (player == computer) {
        cout << "The game is a tie." << endl;
    }
    // Win and lose conditions
    // Player picks paper and computer picks scissors
    if (player == rock && computer == scissors) {
        cout << "You Win: Rock smashes scissors!" << endl;
    }
    // Player picks rock and computer picks paper
    if (player == rock && computer == paper) {
        cout << "Computer won: Paper covers rock!" << endl;
    }
    // Player picks paper and computer picks rock
    if (player == paper && computer == rock) {
        cout << "You Win: Paper covers rock!" << endl;
    }
    // Player picks paper and computer picks scissors
    if (player == paper && computer == scissors) {
        cout << "Computer Won: Scissors cut paper!" << endl;
    }
    // Player picks scissors and computer picks paper
    if (player == scissors && computer == paper) {
        cout << "You Win: Scissors cut paper!" << endl;
    }
    // Player picks scissors and computer picks rock
    if (player == scissors && computer == rock) {
        cout << "Computer Won: Rock smashes scissors!" << endl;
    }
}

// Main
int main() {
	// Introduction and instructions
    cout << "Rock, Paper, Scissors!" << endl;
    cout << "Rock smashes scissors, Paper covers rock, and Scissors cut paper." << endl;
    // Prompt to enter choice
    cout << "Enter 0 for Rock, 1 for paper, or 2 for scissors" << endl;
    // While loop to ensure that a valid choice is made
    while (picked == 0) {
        cin >> choice;
        if (choice == 0) {
        	cout << "You chose Rock" << endl;
        	picked = 1;
        } else if (choice == 1) {
        	cout << "You chose Paper" << endl;
        	picked = 1;
        } else if (choice == 2) {
            cout << "You chose Scissors" << endl;
            picked = 1;
        } else {
        	// Condition if invalid choice is made
            cout << "Invalid choice! Enter 0 for rock, 1 for paper, or 2 for scissors." << endl;
        }
    }
    // Get random choice from computer
    com = getComChoice();
    // Call results function
    results(choice, com);

    return 0;
}