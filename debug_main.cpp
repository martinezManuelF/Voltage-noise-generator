// g++ -Wall -O3 debug_main.cpp ./src/RNG.cpp ./src/ListMath.cpp ./src/NoiseSimulation.cpp -I ./include/ -o main.exe -lp
// thread -fopenmp

// or go/create bin directory:
// WIN:
// cmake -G "MinGW Makefiles" ../
// mingw32-make.exe
// LINUX:
// cmake ../
// make

#include "DataFrame.h"
#include "RNG.h"
#include "ListMath.h"
#include "NoiseSimulation.h"
#include "Options.h"

#include <time.h>
#include <iostream>
#include <memory>

int main(int argc, char const *argv[])
{
	NoiseSimulation Generator;
	DataFrame<double> df;
	time_t timer0, timer1;

	Options::ReadParameters(argc, argv);

	df.from_csv( Options::InputFileName() );
	
	// df.print();
	std::cout << "Finished reading data file with " 
		<< df.getLength() << " points." << std::endl;

	std::string first_col = df.getCols()[0];
	List2D data(df[first_col], df.getLength());

	time(&timer0);
	Generator.Init(data);

	// std::unique_ptr<List2D> waveForm ( Generator.GenNoiseWaveForm(-80) );
	// std::unique_ptr<List2D> waveForm ( Generator.GenNoiseWaveFormSpeed() );
	std::unique_ptr<List2D> waveForm ( Generator.GenNoiseWaveFormPrecision() );

	std::cout << "Finished simulation" << std::endl;
	time(&timer1);
	double secs = difftime(timer1, timer0);
	std::cout << "Lasted: " << secs << " seconds." << std::endl;

	DataFrame<double> output_df;
	std::string cols[2] = {"voltage", "time"};
	if(!output_df.from_array(waveForm->get_X(), cols, waveForm->get_Length(), 2)) {
		// output_df.print();
		std::cout << "Failed to copy array" << std::endl;
	}

	// output_df.print();
	output_df.to_csv( Options::OutputFileName() , ',', 6);
	RNG::Clear();


	return 0;
}