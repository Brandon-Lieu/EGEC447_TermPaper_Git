/*
	@author: Matthew Etesham, Huy Lieu
	@date: Fall 2024
	@purpose: Tristate key logic locking simulation
	@compile: g++ logic_locking_paper_simulation.cpp -o test

*/
#include <iostream>
#include <bitset>
#include <random>

#define BITS 8 // Define the number of bits for multi-bit values
#define LOGIC_LOCKING_COMBINATIONS  4 // defines a constant for four combinations of keys (00, 01, 10, 11).
#define RANDOM_TEST_CASES 10
// Define an enum to represent the four possible output states
enum OutputState {
	BUFFER,      // Buffer mode: output equals input
	INVERTER,    // Inverter mode: output is the complement of input
	HI_Z,        // High-impedance state
	UNK          // Unknown state
};


// Tristate key logic locking implementation and simulation
class TristateGate
{
public:
	// constructor
	// TristateGate(bool key) : key(key), output(false) {}
	// constructor accepting two keys instead of only one key for easy manipulation
	// Constructor with default values for bufferKey and inverterKey
	TristateGate(bool bufferKey = true, bool inverterKey = true)
		: bufferKey(bufferKey), inverterKey(inverterKey), outputState(UNK), outputValue(0) {}
	// Setter to update keys
	void setKeys(bool bufferKey, bool inverterKey) {
		this->bufferKey = bufferKey;
		this->inverterKey = inverterKey;
	}
	// Tristate logic implementation based on key configuration
	OutputState tristateLogic(const std::bitset<BITS>& input)
	{
		if (bufferKey && inverterKey) {
			outputState = UNK;  // Unknown state
			outputValue.reset(); // Set all bits to 0 for undefined
		}
		else if (bufferKey) {
			outputState = BUFFER;  // Buffer mode
			outputValue = input;  // Pass input as output
		}
		else if (inverterKey) {
			outputState = INVERTER;  // Inverter mode
			outputValue = ~input;  // Invert input
		}
		else {
			outputState = HI_Z;  // High-impedance state
		}
		return outputState;
	}

	// Print the current state and output based on the mode
	void printState(const std::bitset<BITS>& input)
	{
		OutputState currentState = tristateLogic(input);

		std::cout << "Input: " << input
				  << "\nBuffer Key: " << bufferKey
				  << "\tInverter Key: " << inverterKey
				  << "\nActual State: ";

		switch (currentState)
		{
		case BUFFER:
			std::cout << "BUFFER\nActual Output: " << outputValue << "\n";
			break;
		case INVERTER:
			std::cout << "INVERTER\nActual Output: " << outputValue << "\n";
			break;
		case HI_Z:
			std::cout << "HI-Z\nActual Output (Retained): " << outputValue << "\n";
			break;
		case UNK:
            std::cout << "UNK\tActual Output: " << outputValue << " (All bits set to 0 for undefined)\n";
			break;
		}
	}

private:
	bool bufferKey;
	bool inverterKey;
	OutputState outputState;
	std::bitset<BITS> outputValue;  // Stores the multi-bit output for BUFFER/INVERTER modes
};

// test harness for logic locking
int main()
{
	bool bufferKey;
	bool inverterKey;

	// Initialize random number generator
	std::random_device rd;  // Seed for random number engine
	std::mt19937 gen(rd()); // Mersenne Twister engine
	std::uniform_int_distribution<> dist(0, (1 << BITS) - 1); // Distribution for random 8-bit numbers

	// Initialize the logic gate outside the loop
	TristateGate tsg;

	for (int i = 0; i < RANDOM_TEST_CASES; i++)
	{
		// Generate a random 8-bit input
		std::bitset<BITS> input(dist(gen));

		std::cout << "Random Input " << i + 1 << ": " << input << "\n\n";
		for (int j = 0; j < LOGIC_LOCKING_COMBINATIONS; j++) {
			// Set bufferKey and inverterKey for each state combination
			bufferKey = (j & 0x2) >> 1; // Get the second bit (MSB)
			inverterKey = (j & 0x1);    // Get the first bit (LSB)

			// Update keys using the setKeys method
			tsg.setKeys(bufferKey, inverterKey);
			tsg.printState(input);
			std::cout << "\n";
		}

	}
	return 0;
}